#pragma comment(lib, "winmm.lib")

#include "WiiRemoteYourselfPCH.h"
#include "WiiRemote.h"
#include "Acceleration.h"
#include "Device.h"
#include <mmsystem.h>

bool WiiRemote::ParseInput(unsigned char* buffer)
{
	auto changed = 0;
	auto lock = new std::lock_guard<std::mutex>(stateLock);
	switch (static_cast<InputReport>(buffer[0]))
	{
	case InputReport::Buttons:
		changed |= ParseButtons(buffer);
		break;
	case InputReport::ButtonsAcceleration:
		changed |= ParseButtons(buffer);
		if (!IsBalanceBoard())
			changed |= ParseAcceleration(buffer);
		break;
	case InputReport::ButtonsAccelerationExtension:
		changed |= ParseButtons(buffer);
		changed |= ParseExtension(buffer, 6);
		if (!IsBalanceBoard())
			changed |= ParseAcceleration(buffer);
		break;
	case InputReport::ButtonsAccelerationIR:
		changed |= ParseButtons(buffer);
		if (!IsBalanceBoard())
		{
			changed |= ParseAcceleration(buffer);
			changed |= ParseIR(buffer);
		}
		break;
	case InputReport::ButtonsAccelerationIRExtension:
		changed |= ParseButtons(buffer);
		changed |= ParseExtension(buffer, 16);
		if (!IsBalanceBoard())
		{
			changed |= ParseAcceleration(buffer);
			changed |= ParseIR(buffer);
		}
		break;
	case InputReport::ButtonsBalanceBoard:
		changed |= ParseButtons(buffer);
		changed |= ParseExtension(buffer, 3);
		break;
	case InputReport::ReadAddress:
		changed |= ParseButtons(buffer);
		changed |= ParseReadAddress(buffer);
		break;
	case InputReport::Status:
		changed |= ParseButtons(buffer);
		changed |= ParseStatus(buffer);
		bStatusReceived = true;
		break;
	default:
		delete lock;
		return false;
	}
	if (recording.Enabled && (changed & recording.TriggerFlags))
	{
		WiiRemoteStateEvent event;
		event.TimeMs = timeGetTime();
		event.State = *static_cast<WiiRemoteState*>(this);
		recording.StateHistory->push_back(event);
	}
	internalChanged = static_cast<StateChangeFlags>(static_cast<int>(internalChanged) | changed);
	delete lock;
	if (changed & static_cast<int>(CallbackTriggerFlags))
	{
		ChangedNotifier(static_cast<StateChangeFlags>(changed), internal);
		if (ChangedCallback)
			ChangedCallback(*this, static_cast<StateChangeFlags>(changed), internal);
	}
	return true;
}

void WiiRemote::InitializeExtension()
{
	bInitInProgress = true;
	if (!bEnablingMotionPlus)
	{
		WriteData(static_cast<int>(RegisterDevice::ExtensionInitialize1), 0x55);
		WriteData(static_cast<int>(RegisterDevice::ExtensionInitialize2), 0x00);
	}
	else
	{
		bEnablingMotionPlus = false;
	}
	ReadAddress(static_cast<int>(RegisterDevice::ExtensionType), 6);
}

int WiiRemote::ParseStatus(unsigned char* buffer)
{
	auto changed = 0;
	const auto batteryRaw = buffer[6];
	if (internal.BatteryRaw != batteryRaw)
		changed |= static_cast<int>(StateChangeFlags::BatteryChanged);
	internal.BatteryRaw = batteryRaw;
	internal.BatteryPercent = batteryRaw / 2;
	const auto drained = static_cast<bool>(buffer[3] & 0x01);
	if (drained != bBatteryDrained)
	{
		bBatteryDrained = drained;
		if (drained)
			changed |= static_cast<int>(StateChangeFlags::BatteryDrained);
	}
	const auto leds = static_cast<unsigned char>(buffer[3] >> 4);
	if (leds != internal.LED.Bits)
		changed |= static_cast<int>(StateChangeFlags::LEDsChanged);
	internal.LED.Bits = leds;
	const auto extension = ((buffer[3] & 0x02) != 0);
	if (extension != internal.bExtension)
	{
		if (!internal.bExtension)
		{
			internal.bExtension = true;
			InitializeExtension();
		}
		else
		{
			internal.bExtension = false;
			internal.ExtensionType = ExtensionType::None;
			bMotionPlusEnabled = false;
			bMotionPlusExtension = false;
			bMotionPlusDetected = false;
			bInitInProgress = false;
			bEnablingMotionPlus = false;
			changed |= static_cast<int>(StateChangeFlags::ExtensionDisconnected);
		}
	}
	return changed;
}

int WiiRemote::ParseButtons(unsigned char* buffer)
{
	auto changed = 0;
	auto bits = *reinterpret_cast<unsigned short*>(buffer + 1) & 
		static_cast<unsigned short>(WiiRemoteButtons::Mask::All);
	if (bits != internal.Buttons.Bits)
		changed |= static_cast<int>(StateChangeFlags::ButtonsChanged);
	internal.Buttons.Bits = bits;
	return changed;
}

int WiiRemote::ParseAcceleration(unsigned char* buffer)
{
	auto changed = 0;
	Vector<unsigned char> raw(buffer[3], buffer[4], buffer[5]);
	if ((raw.X != internal.Acceleration.RawData.X) ||
		(raw.Y != internal.Acceleration.RawData.Y) ||
		(raw.Z != internal.Acceleration.RawData.Z))
		changed |= static_cast<int>(StateChangeFlags::AccelerationChanged);
	internal.Acceleration.RawData = raw;
	if (internal.CalibrationInformation.Data.X)
	{
		internal.Acceleration.Data.X =
			(static_cast<float>(internal.Acceleration.RawData.X) - static_cast<float>(internal.CalibrationInformation.Data.X)) /
			(static_cast<float>(internal.CalibrationInformation.DataWithGravity.X) - static_cast<float>(internal.CalibrationInformation.Data.X));
		internal.Acceleration.Data.Y =
			(static_cast<float>(internal.Acceleration.RawData.Y) - static_cast<float>(internal.CalibrationInformation.Data.Y)) /
			(static_cast<float>(internal.CalibrationInformation.DataWithGravity.Y) - static_cast<float>(internal.CalibrationInformation.Data.Y));
		internal.Acceleration.Data.Z =
			(static_cast<float>(internal.Acceleration.RawData.Z) - static_cast<float>(internal.CalibrationInformation.Data.Z)) /
			(static_cast<float>(internal.CalibrationInformation.DataWithGravity.Z) - static_cast<float>(internal.CalibrationInformation.Data.Z));
	}
	else
	{
		internal.Acceleration.Data = Vector<float>(0.0f);
	}
	if (EstimateOrientationFrom(internal.Acceleration))
		changed |= static_cast<int>(StateChangeFlags::OrientationChanged);
	return changed;
}

bool WiiRemote::EstimateOrientationFrom(struct Acceleration& acceleration)
{
	const auto lengthSquare = MathHelper::Square(acceleration.Data.X) + MathHelper::Square(acceleration.Data.Y) + MathHelper::Square(acceleration.Data.Z);
	static const auto epsilon = 0.2f;
	if ((lengthSquare >= (1.0f - epsilon)) && (lengthSquare <= (1.0f + epsilon)))
	{
		if (++wiiRemoteNearGravityUpdates < 2)
			return false;
		const auto inverseLength = 1.0f / MathHelper::Sign(lengthSquare);
		const auto vector = Vector<float>(
			acceleration.Data.X * inverseLength,
			acceleration.Data.Y * inverseLength,
			acceleration.Data.Z * inverseLength);
		acceleration.Orientation.Data = vector;
		auto pitch = -std::asinf(vector.Y) * 57.2957795f;
		auto roll = std::atan2(vector.X, vector.Z) * 57.2957795f;
		if (vector.Z < 0.0f)
		{
			pitch = (vector.Y < 0.0f) ? 180.0f - pitch : -180.0f - pitch;
			roll = (vector.X < 0.0f) ? -180.0f - roll : 180.0f - roll;
		}
		acceleration.Orientation.Rotation = Rotator<float>(pitch, 0.0f, roll);
		acceleration.Orientation.UpdateAge = 0;
		return true;
	}
	wiiRemoteNearGravityUpdates = 0;
	acceleration.Orientation.UpdateAge++;
	return false;
}

void WiiRemote::ApplyJoystickDeadZones(Joystick& joystick)
{
	if ((joystick.DeadZone.X > 0.0f) && (joystick.DeadZone.X <= 1.0f))
	{
		if (std::fabsf(joystick.Data.X) <= joystick.DeadZone.X)
			joystick.Data.X = 0.0f;
		else
		{
			joystick.Data.X -= joystick.DeadZone.X * MathHelper::Sign(joystick.Data.X);
			joystick.Data.X /= 1.0f - joystick.DeadZone.X;
		}
	}
	if ((joystick.DeadZone.Y > 0.0f) && (joystick.DeadZone.Y <= 1.0f))
	{
		if (std::fabsf(joystick.Data.Y) <= joystick.DeadZone.Y)
			joystick.Data.Y = 0.0f;
		else
		{
			joystick.Data.Y -= joystick.DeadZone.Y * MathHelper::Sign(joystick.Data.Y);
			joystick.Data.Y /= 1.0f - joystick.DeadZone.Y;
		}
	}
}

int WiiRemote::ParseIR(unsigned char* buffer)
{
	if (internal.IR.Mode == IRMode::Off)
		return static_cast<int>(StateChangeFlags::NoChange);
	if (bMotionPlusEnabled && (internal.IR.Mode == IRMode::Extended))
		return static_cast<int>(StateChangeFlags::NoChange);
	auto prevIR = internal.IR;
	switch (internal.IR.Mode)
	{
	case IRMode::Basic:
		for (auto step = 0; step < 2; step++)
		{
			auto& dot0 = internal.IR.Dot[step * 2 + 0];
			auto& dot1 = internal.IR.Dot[step * 2 + 1];
			const auto offs = 6 + (step * 5);
			dot0.Visible = !(buffer[offs + 0] == 0xff && buffer[offs + 1] == 0xff);
			dot1.Visible = !(buffer[offs + 3] == 0xff && buffer[offs + 4] == 0xff);
			if (dot0.Visible)
			{
				dot0.RawData.X = buffer[offs + 0] | ((buffer[offs + 2] >> 4) & 0x03) << 8;
				dot0.RawData.Y = buffer[offs + 1] | ((buffer[offs + 2] >> 6) & 0x03) << 8;
				dot0.Data.X = 1.0f - (static_cast<float>(dot0.RawData.X) / static_cast<float>(IR::MaxRawX));
				dot0.Data.Y = (static_cast<float>(dot0.RawData.Y) / static_cast<float>(IR::MaxRawY));
			}
			if (dot1.Visible)
			{
				dot1.RawData.X = buffer[offs + 3] | ((buffer[offs + 2] >> 0) & 0x03) << 8;
				dot1.RawData.Y = buffer[offs + 4] | ((buffer[offs + 2] >> 2) & 0x03) << 8;
				dot1.Data.X = 1.0f - (static_cast<float>(dot1.RawData.X) / static_cast<float>(IR::MaxRawX));
				dot1.Data.Y = (static_cast<float>(dot1.RawData.Y) / static_cast<float>(IR::MaxRawY));
			}
		}
		break;
	case IRMode::Extended:
		for (auto index = 0; index < 4; index++)
		{
			auto& dot = internal.IR.Dot[index];
			const auto offs = 6 + (index * 3);
			dot.Visible = !(buffer[offs] == 0xff && buffer[offs + 1] == 0xff && buffer[offs + 2] == 0xff);
			if (dot.Visible)
			{
				dot.RawData.X = buffer[offs + 0] | ((buffer[offs + 2] >> 4) & 0x03) << 8;
				dot.RawData.Y = buffer[offs + 1] | ((buffer[offs + 2] >> 6) & 0x03) << 8;
				dot.Data.X = 1.0f - (static_cast<float>(dot.RawData.X) / static_cast<float>(IR::MaxRawX));
				dot.Data.Y = (static_cast<float>(dot.RawData.Y) / static_cast<float>(IR::MaxRawY));
				dot.Size = buffer[offs + 2] & 0x0f;
			}
		}
		break;
	case IRMode::Full:
		break;
	}
	return memcmp(&prevIR, &internal.IR, sizeof(internal.IR)) ? static_cast<int>(StateChangeFlags::IRChanged) : 0;
}

int WiiRemote::ParseExtension(unsigned char* buffer, const unsigned int offset)
{
	auto changed = 0;
	switch (internal.ExtensionType)
	{
	case ExtensionType::Nunchuk:
		changed |= ParseNunchuk(buffer, offset);
		break;
	case ExtensionType::Classic:
	case ExtensionType::GH3GhwtGuitar:
	case ExtensionType::GhwtDrums:
		changed |= ParseClassicController(buffer, offset);
		break;
	case ExtensionType::BalanceBoard:
		changed |= ParseBalanceBoard(buffer, offset);
		break;
	case ExtensionType::MotionPlus:
	case ExtensionType::WiiRemotePlus:
		changed |= ParseMotionPlus(buffer, offset);
		break;
	}
	return changed;
}

int WiiRemote::ParseReadAddress(unsigned char* buffer)
{
	const auto address = buffer[4] << 8 | buffer[5];
	const auto size = buffer[3] >> 4;
	auto changed = 0;
	if ((buffer[3] & 0x08) != 0)
		return static_cast<int>(StateChangeFlags::NoChange);
	else if ((buffer[3] & 0x07) != 0)
	{
		if (motionPlusDetectCount)
		{
			--motionPlusDetectCount;
			if (internal.ExtensionType == ExtensionType::MotionPlus || internal.ExtensionType == ExtensionType::WiiRemotePlus)
			{
				bMotionPlusDetected = false;
				bMotionPlusEnabled = false;
			}
		}
		return static_cast<int>(StateChangeFlags::NoChange);
	}
	buffer += 6;
	switch (address)
	{
	case (static_cast<int>(RegisterDevice::Calibration) & 0xffff) :
		internal.CalibrationInformation.Data = Vector<unsigned char>(buffer[0], buffer[1], buffer[2]);
		internal.CalibrationInformation.DataWithGravity = Vector<unsigned char>(buffer[4], buffer[5], buffer[6]);
		break;
	case (static_cast<int>(RegisterDevice::ExtensionType) & 0xffff) :
		changed |= ReadDeviceAddress(buffer);
		break;
	case (static_cast<int>(RegisterDevice::ExtensionCalibration) & 0xffff) :
	case (static_cast<int>(RegisterDevice::BalanceCalibration) & 0xffff) :
		changed |= CalibrationDevice(buffer);
		break;
	case 0x34:
		if (internal.ExtensionType == ExtensionType::BalanceBoard)
		{
			auto& calibration = internal.BalanceBoard.CalibrationInformation;
			calibration.Kg34.TopR = (static_cast<short>(buffer[0]) << 8) | static_cast<short>(buffer[1]);
			calibration.Kg34.BottomR = (static_cast<short>(buffer[2]) << 8) | static_cast<short>(buffer[3]);
			calibration.Kg34.TopL = (static_cast<short>(buffer[4]) << 8) | static_cast<short>(buffer[5]);
			calibration.Kg34.BottomL = (static_cast<short>(buffer[6]) << 8) | static_cast<short>(buffer[7]);
			changed |= static_cast<int>(StateChangeFlags::BalanceConnected);
			SetReportType(InputReport::ButtonsBalanceBoard);
		}
		bInitInProgress = false;
		break;
	default:
		break;
	}
	return changed;
}

int WiiRemote::ParseNunchuk(unsigned char* buffer, const unsigned int offset)
{
	auto changed = 0;
	const auto c = ((buffer[offset + 5] & 0x02) == 0);
	const auto z = ((buffer[offset + 5] & 0x01) == 0);
	if ((c != internal.Nunchuk.C) || (z != internal.Nunchuk.Z))
		changed |= static_cast<int>(StateChangeFlags::NunchukButtonsChanged);
	internal.Nunchuk.C = c;
	internal.Nunchuk.Z = z;
	auto& accel = internal.Nunchuk.Acceleration;
	const auto raw = Vector<unsigned char>(buffer[offset + 2], buffer[offset + 3], buffer[offset + 4]);
	if ((raw.X != accel.RawData.X) || (raw.Y != accel.RawData.Y) || (raw.Z != accel.RawData.Z))
		changed |= static_cast<int>(StateChangeFlags::NunchukAccelerationChanged);
	accel.RawData = raw;
	auto& calibration = internal.Nunchuk.CalibrationInformation;
	accel.Data = Vector<float>
	{
		(static_cast<float>(raw.X) - static_cast<float>(calibration.Data.X)) /
		(static_cast<float>(calibration.DataWithGravity.X) - static_cast<float>(CalibrationInformation.Data.X)),
		(static_cast<float>(raw.Y) - static_cast<float>(calibration.Data.Y)) /
		(static_cast<float>(calibration.DataWithGravity.Y) - static_cast<float>(CalibrationInformation.Data.Y)),
		(static_cast<float>(raw.Z) - static_cast<float>(calibration.Data.Z)) /
		(static_cast<float>(calibration.DataWithGravity.Z) - static_cast<float>(CalibrationInformation.Data.Z))
	};
	if (EstimateOrientationFrom(accel))
		changed |= static_cast<int>(StateChangeFlags::NunchukOrientationChanged);
	auto& joy = internal.Nunchuk.Joystick;
	const auto joyRaw = Vector2D<unsigned char>(buffer[offset + 0], buffer[offset + 1]);
	if ((joyRaw.X != joy.RawData.X) || (joyRaw.Y != joy.RawData.Y))
		changed |= static_cast<int>(StateChangeFlags::NunchukJoystickChanged);
	joy.RawData.X = joyRaw.X;
	joy.RawData.Y = joyRaw.Y;
	if (calibration.Maximum.X != 0x00)
	{
		joy.Data.X = (static_cast<float>(joyRaw.X) - static_cast<float>(calibration.Middle.X)) /
			(static_cast<float>(calibration.Maximum.X) - static_cast<float>(calibration.Minimum.X));
	}
	if (calibration.Maximum.Y != 0x00)
	{
		joy.Data.Y = (static_cast<float>(joyRaw.Y) - static_cast<float>(calibration.Middle.Y)) /
			(static_cast<float>(calibration.Maximum.Y) - static_cast<float>(calibration.Minimum.Y));
	}
	joy.Data.X *= 2.0f;
	joy.Data.Y *= 2.0f;
	joy.DeadZone = Nunchuk.Joystick.DeadZone;
	ApplyJoystickDeadZones(joy);
	return changed;
}

int WiiRemote::ParseClassicController(unsigned char* buffer, const unsigned int offset)
{
	auto changed = 0;
	auto bits = *reinterpret_cast<unsigned short*>(buffer + offset + 4);
	bits = ~bits;
	if (bits != internal.ClassicController.Buttons.Bits)
		changed |= static_cast<int>(StateChangeFlags::ClassicButtonsChanged);
	internal.ClassicController.Buttons.Bits = bits;
	auto& joyL = internal.ClassicController.JoystickL;
	auto& joyR = internal.ClassicController.JoystickR;
	const auto lRaw = Vector2D<float>(static_cast<float>(buffer[offset + 0] & 0x3f), static_cast<float>(buffer[offset + 1] & 0x3f));
	const auto rRaw = Vector2D<float>(
		static_cast<float>((buffer[offset + 2] >> 7) | ((buffer[offset + 1] & 0xc0) >> 5) | ((buffer[offset + 0] & 0xc0) >> 3)),
		static_cast<float>(buffer[offset + 2] & 0x1f));
	if ((joyL.RawData.X != lRaw.X) || (joyL.RawData.Y != lRaw.Y))
		changed |= static_cast<int>(StateChangeFlags::ClassicJoystickLChanged);
	if ((joyR.RawData.X != rRaw.X) || (joyR.RawData.Y != rRaw.Y))
		changed |= static_cast<int>(StateChangeFlags::ClassicJoystickRChanged);
	joyL.RawData.X = lRaw.X;
	joyL.RawData.Y = lRaw.Y;
	joyR.RawData.X = rRaw.X;
	joyR.RawData.Y = rRaw.Y;
	auto& calibration = internal.ClassicController.CalibrationInformation;
	if (calibration.MaximumL.X != 0x00)
	{
		joyL.Data.X = (joyL.RawData.X - static_cast<float>(calibration.MiddleL.X)) /
			(static_cast<float>(calibration.MaximumL.X) - static_cast<float>(calibration.MinimumL.X));
	}
	if (calibration.MaximumL.Y != 0x00)
	{
		joyL.Data.Y = (joyL.RawData.Y - static_cast<float>(calibration.MiddleL.Y)) /
			(static_cast<float>(calibration.MaximumL.Y) - static_cast<float>(calibration.MinimumL.Y));
	}
	if (calibration.MaximumR.X != 0x00)
	{
		joyR.Data.X = (joyR.RawData.X - static_cast<float>(calibration.MiddleR.X)) /
			(static_cast<float>(calibration.MaximumR.X) - static_cast<float>(calibration.MinimumR.X));
	}
	if (calibration.MaximumR.Y != 0x00)
	{
		joyR.Data.Y = (joyR.RawData.Y - static_cast<float>(calibration.MiddleR.Y)) /
			(static_cast<float>(calibration.MaximumR.Y) - static_cast<float>(calibration.MinimumR.Y));
	}
	joyL.Data.X *= 2.0f;
	joyL.Data.Y *= 2.0f;
	joyR.Data.X *= 2.0f;
	joyR.Data.Y *= 2.0f;
	joyL.DeadZone = ClassicController.JoystickL.DeadZone;
	joyR.DeadZone = ClassicController.JoystickR.DeadZone;
	ApplyJoystickDeadZones(joyL);
	ApplyJoystickDeadZones(joyR);
	const unsigned char rawTriggerL = ((buffer[offset + 2] & 0x60) >> 2) | (buffer[offset + 3] >> 5);
	const unsigned char rawTriggerR = buffer[offset + 3] & 0x1f;
	if ((rawTriggerL != internal.ClassicController.RawTriggerL) || (rawTriggerR != internal.ClassicController.RawTriggerR))
		changed |= static_cast<int>(StateChangeFlags::ClassicTriggersChanged);
	internal.ClassicController.RawTriggerL = rawTriggerL;
	internal.ClassicController.RawTriggerR = rawTriggerR;
	if (calibration.MaximumTriggerL != 0x00) 
	{
		internal.ClassicController.TriggerL = static_cast<float>(internal.ClassicController.RawTriggerL) / 
			(static_cast<float>(calibration.MaximumTriggerL) - static_cast<float>(calibration.MinimumTriggerL));
	}
	if (calibration.MaximumTriggerR != 0x00)
	{
		internal.ClassicController.TriggerR = static_cast<float>(internal.ClassicController.RawTriggerR) /
			(static_cast<float>(calibration.MaximumTriggerR) - static_cast<float>(calibration.MinimumTriggerR));
	}
	return changed;
}

int WiiRemote::ParseBalanceBoard(unsigned char* buffer, const unsigned int offset)
{
	auto changed = 0;
	auto prevRaw = internal.BalanceBoard.Raw;
	internal.BalanceBoard.Raw.TopR = (static_cast<short>(buffer[offset + 0]) << 8) | static_cast<short>(buffer[offset + 1]);
	internal.BalanceBoard.Raw.BottomR = (static_cast<short>(buffer[offset + 2]) << 8) | static_cast<short>(buffer[offset + 3]);
	internal.BalanceBoard.Raw.TopL = (static_cast<short>(buffer[offset + 4]) << 8) | static_cast<short>(buffer[offset + 5]);
	internal.BalanceBoard.Raw.BottomL = (static_cast<short>(buffer[offset + 6]) << 8) | static_cast<short>(buffer[offset + 7]);
	if ((internal.BalanceBoard.Raw.TopL != prevRaw.TopL) || (internal.BalanceBoard.Raw.TopR != prevRaw.TopR) ||
		(internal.BalanceBoard.Raw.BottomL != prevRaw.BottomL) || (internal.BalanceBoard.Raw.BottomR != prevRaw.BottomR))
		changed |= static_cast<int>(StateChangeFlags::BalanceWeightChanged);
	internal.BalanceBoard.Kg.TopL = GetBalanceValue(internal.BalanceBoard.Raw.TopL, internal.BalanceBoard.CalibrationInformation.Kg0.TopL,
		internal.BalanceBoard.CalibrationInformation.Kg17.TopL, internal.BalanceBoard.CalibrationInformation.Kg34.TopL);
	internal.BalanceBoard.Kg.TopR = GetBalanceValue(internal.BalanceBoard.Raw.TopR, internal.BalanceBoard.CalibrationInformation.Kg0.TopR,
		internal.BalanceBoard.CalibrationInformation.Kg17.TopR, internal.BalanceBoard.CalibrationInformation.Kg34.TopR);
	internal.BalanceBoard.Kg.BottomL = GetBalanceValue(internal.BalanceBoard.Raw.BottomL, internal.BalanceBoard.CalibrationInformation.Kg0.BottomL,
		internal.BalanceBoard.CalibrationInformation.Kg17.BottomL, internal.BalanceBoard.CalibrationInformation.Kg34.BottomL);
	internal.BalanceBoard.Kg.BottomR = GetBalanceValue(internal.BalanceBoard.Raw.BottomR, internal.BalanceBoard.CalibrationInformation.Kg0.BottomR,
		internal.BalanceBoard.CalibrationInformation.Kg17.BottomR, internal.BalanceBoard.CalibrationInformation.Kg34.BottomR);
	if (bCalibrateAtRest)
	{
		bCalibrateAtRest = false;
		internal.BalanceBoard.AtRestKg = internal.BalanceBoard.Kg;
	}
	internal.BalanceBoard.Kg.TopL -= BalanceBoard.AtRestKg.TopL;
	internal.BalanceBoard.Kg.TopR -= BalanceBoard.AtRestKg.TopR;
	internal.BalanceBoard.Kg.BottomL -= BalanceBoard.AtRestKg.BottomL;
	internal.BalanceBoard.Kg.BottomR -= BalanceBoard.AtRestKg.BottomR;
	internal.BalanceBoard.Kg.Total = internal.BalanceBoard.Kg.TopL + internal.BalanceBoard.Kg.TopR + internal.BalanceBoard.AtRestKg.BottomL + internal.BalanceBoard.AtRestKg.BottomR;
	const auto kg2lb = 2.20462262f;
	internal.BalanceBoard.Lb = internal.BalanceBoard.Kg;
	internal.BalanceBoard.Lb.TopL *= kg2lb;
	internal.BalanceBoard.Lb.TopR *= kg2lb;
	internal.BalanceBoard.Lb.BottomL *= kg2lb;
	internal.BalanceBoard.Lb.BottomR *= kg2lb;
	internal.BalanceBoard.Lb.Total *= kg2lb;
	return changed;
}

int WiiRemote::ParseMotionPlus(unsigned char* buffer, const unsigned int offset)
{
	auto changed = 0;
	bMotionPlusDetected = true;
	bMotionPlusEnabled = true;
	const auto rotation = Rotator<short>(
		(static_cast<unsigned short>(buffer[offset + 5]) & 0xFC) << 6 | static_cast<unsigned short>(buffer[offset + 2]),
		(static_cast<unsigned short>(buffer[offset + 3]) & 0xFC) << 6 | static_cast<unsigned short>(buffer[offset + 0]),
		(static_cast<unsigned short>(buffer[offset + 4]) & 0xFC) << 6 | static_cast<unsigned short>(buffer[offset + 1]));
	if ((rotation.Yaw != 0x3fff) || (rotation.Pitch != 0x3fff) || (rotation.Roll != 0x3fff))
	{
		auto& raw = internal.MotionPlus.Raw;
		if ((raw.Yaw != rotation.Yaw) || (raw.Pitch != rotation.Pitch) || (raw.Roll != rotation.Roll))
			changed |= static_cast<int>(StateChangeFlags::MotionPlusChanged);
		raw = rotation;
		const auto slow = Rotator<bool>(
			(buffer[offset + 3] & 0x1) == 0x1,
			(buffer[offset + 3] & 0x2) == 0x2,
			(buffer[offset + 4] & 0x2) == 0x2);
		const auto scale = Rotator<float>(
			slow.Pitch ? 0.05f : 0.25f,
			slow.Yaw ? 0.05f : 0.25f,
			slow.Roll ? 0.05f : 0.25f
			);
		internal.MotionPlus.Speed = Rotator<float>(
			-(raw.Pitch - 0x1F7F) * scale.Pitch,
			-(raw.Yaw - 0x1F7F) * scale.Yaw,
			-(raw.Roll - 0x1F7F) * scale.Roll
			);
		const auto extension = static_cast<bool>(buffer[offset + 4] & 1);
		if (extension != bMotionPlusExtension)
		{
			if (extension)
				changed |= static_cast<int>(StateChangeFlags::MotionPlusExtensionConnected);
			else
				changed |= static_cast<int>(StateChangeFlags::MotionPlusExtensionDisconnected);
		}
		bMotionPlusExtension = extension;
	}
	return changed;
}

int WiiRemote::ReadDeviceAddress(unsigned char* buffer)
{
	auto changed = 0;
	const auto type = *reinterpret_cast<unsigned long long*>(buffer);
	if ((type == static_cast<unsigned long long>(AddressDevice::MotionPlusDetect)) || 
		(type == static_cast<unsigned long long>(AddressDevice::MotionPlusDetect2)) ||
		(type == static_cast<unsigned long long>(AddressDevice::WiiRemotePlusDetect)) ||
		(type == static_cast<unsigned long long>(AddressDevice::WiiRemotePlusDetect2)))
	{
		if (!bMotionPlusDetected)
			changed |= static_cast<int>(StateChangeFlags::MotionPlusDetected);
		bMotionPlusDetected = true;
		--motionPlusDetectCount;
		return changed;
	}
	switch (type)
	{
	case static_cast<unsigned long long>(AddressDevice::MotionPlus) :
		if (internal.ExtensionType == ExtensionType::MotionPlus)
			return changed;
		internal.ExtensionType = ExtensionType::MotionPlus;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		bMotionPlusDetected = true;
		break;
	case static_cast<unsigned long long>(AddressDevice::WiiRemotePlus) :
		if (internal.ExtensionType == ExtensionType::WiiRemotePlus)
			return changed;
		internal.ExtensionType = ExtensionType::WiiRemotePlus;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		bMotionPlusDetected = true;
		break;
	case static_cast<unsigned long long>(AddressDevice::Nunchuk) :
		if (internal.ExtensionType == ExtensionType::Nunchuk)
			return changed;
		internal.ExtensionType = ExtensionType::Nunchuk;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		break;
	case static_cast<unsigned long long>(AddressDevice::Classic) :
		if (internal.ExtensionType == ExtensionType::Classic)
			return changed;
		internal.ExtensionType = ExtensionType::Classic;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		break;
	case static_cast<unsigned long long>(AddressDevice::GH3GhwtGuitar) :
		if (internal.ExtensionType == ExtensionType::GH3GhwtGuitar)
			return changed;
		internal.ExtensionType = ExtensionType::GH3GhwtGuitar;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		break;
	case static_cast<unsigned long long>(AddressDevice::GhwtDrums) :
		if (internal.ExtensionType == ExtensionType::GhwtDrums)
			return changed;
		internal.ExtensionType = ExtensionType::GhwtDrums;
		ReadAddress(static_cast<int>(RegisterDevice::ExtensionCalibration), 16);
		break;
	case static_cast<unsigned long long>(AddressDevice::BalanceBoard) :
		if (internal.ExtensionType == ExtensionType::BalanceBoard)
			return changed;
		internal.ExtensionType = ExtensionType::BalanceBoard;
		ReadAddress(static_cast<int>(RegisterDevice::BalanceCalibration), 16);
		bMotionPlusEnabled = false;
		break;
	case static_cast<unsigned long long>(AddressDevice::PartiallyInserted) :
		if (internal.ExtensionType == ExtensionType::PartiallyInserted)
			Sleep(50);
		bMotionPlusEnabled = false;
		internal.ExtensionType = ExtensionType::PartiallyInserted;
		changed |= static_cast<int>(StateChangeFlags::ExtensionPartiallyInserted);
		internal.bExtension = false;
		RequestStatusReport();
		break;
	default:
		break;
	}
	return changed;
}

int WiiRemote::CalibrationDevice(unsigned char* buffer)
{
	auto changed = 0;
	switch (internal.ExtensionType)
	{
	case ExtensionType::Nunchuk:
	{
		auto& calibration = internal.Nunchuk.CalibrationInformation;
		calibration.Data = Vector<unsigned char>(buffer[0], buffer[1], buffer[2]);
		calibration.DataWithGravity = Vector<unsigned char>(buffer[4], buffer[5], buffer[6]);
		calibration.Maximum = Vector2D<unsigned char>(buffer[8], buffer[11]);
		calibration.Minimum = Vector2D<unsigned char>(buffer[9], buffer[12]);
		calibration.Middle = Vector2D<unsigned char>(buffer[10], buffer[13]);
		changed |= static_cast<int>(StateChangeFlags::NunchukConnected);
	}
		break;
	case ExtensionType::Classic:
	case ExtensionType::GH3GhwtGuitar:
	case ExtensionType::GhwtDrums:
	{
		auto& calibration = internal.ClassicController.CalibrationInformation;
		calibration.MaximumL = Vector2D<unsigned char>(buffer[0] >> 2, buffer[3] >> 2);
		calibration.MinimumL = Vector2D<unsigned char>(buffer[1] >> 2, buffer[4] >> 2);
		calibration.MiddleL = Vector2D<unsigned char>(buffer[2] >> 2, buffer[5] >> 2);
		calibration.MaximumR = Vector2D<unsigned char>(buffer[6] >> 3, buffer[9] >> 3);
		calibration.MinimumR = Vector2D<unsigned char>(buffer[7] >> 3, buffer[10] >> 3);
		calibration.MiddleR = Vector2D<unsigned char>(buffer[8] >> 3, buffer[11] >> 3);
		calibration.MinimumTriggerL = 0;
		calibration.MaximumTriggerL = 31;
		calibration.MinimumTriggerR = 0;
		calibration.MaximumTriggerR = 31;
		changed |= static_cast<int>(StateChangeFlags::ClassicConnected);
	}
		break;
	case ExtensionType::BalanceBoard:
	{
		auto& calibration = internal.BalanceBoard.CalibrationInformation;
		calibration.Kg0.TopR = (static_cast<short>(buffer[0]) << 8) | static_cast<short>(buffer[1]);
		calibration.Kg0.BottomR = (static_cast<short>(buffer[2]) << 8) | static_cast<short>(buffer[3]);
		calibration.Kg0.TopL = (static_cast<short>(buffer[4]) << 8) | static_cast<short>(buffer[5]);
		calibration.Kg0.BottomL = (static_cast<short>(buffer[6]) << 8) | static_cast<short>(buffer[7]);
		calibration.Kg17.TopR = (static_cast<short>(buffer[8]) << 8) | static_cast<short>(buffer[9]);
		calibration.Kg17.BottomR = (static_cast<short>(buffer[10]) << 8) | static_cast<short>(buffer[11]);
		calibration.Kg17.TopL = (static_cast<short>(buffer[12]) << 8) | static_cast<short>(buffer[13]);
		calibration.Kg17.BottomL = (static_cast<short>(buffer[14]) << 8) | static_cast<short>(buffer[15]);
	}
		break;
	case ExtensionType::MotionPlus:
	case ExtensionType::WiiRemotePlus:
		changed |= static_cast<int>(StateChangeFlags::MotionPlusEnabled);
		bMotionPlusEnabled = true;
		bInitInProgress = false;
		break;
	}
	return changed;
}
