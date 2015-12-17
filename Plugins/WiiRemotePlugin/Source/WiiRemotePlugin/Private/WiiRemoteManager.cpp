#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteManager.h"
#include "WiiRemoteDelegate.h"
#include "FWiiRemotePlugin.h"

UWiiRemoteManager* manager;

void OnStateChanged(wiimote& wiiRemote, state_change_flags changed, const wiimote_state& new_state);

UWiiRemoteManager::UWiiRemoteManager(class FObjectInitializer const& objectInitializer)
	: Super(objectInitializer)
	, WiiRemoteDelegate(nullptr)
{
	manager = this;
	Data.Init(FWiiRemoteDeviceData(), 4);
	ChangeFlags.Init(FWiiRemoteChangeFlags(), 4);
	LastPairedWiiRemoteID.Init(-1, 4);
}

UWiiRemoteManager::~UWiiRemoteManager()
{
	ShutDown();
	for (auto index = 0; index < Data.Num(); index++)
	{
		if (WiiRemotes[index].IsConnected())
		{
			WiiRemotes[index].SetLEDs(0);
			WiiRemotes[index].Disconnect();
		}
	}
}

bool UWiiRemoteManager::Startup()
{
	for (auto index = 0; index < Data.Num(); index++)
	{
		WiiRemotes[index].CallbackTriggerFlags = static_cast<state_change_flags>((state_change_flags::CHANGED_ALL | state_change_flags::CONNECTED));
		WiiRemotes[index].ChangedCallback = OnStateChanged;
	}
	for (auto index = 0; index < Data.Num(); index++)
	{
		if (!WiiRemotes[index].IsConnected())
		{
			if (WiiRemotes[index].Connect())
			{
				UE_LOG(LogTemp, Warning, TEXT("WiiRemote %d is connected!"), index);
				FWiiRemoteDeviceData data;
				WiiRemotes[index].SetLEDs(1 << index);
				data.LED = static_cast<WiiRemoteLED>(1 << index);
				data.IsInitialized = true;
				Data[index] = data;
				LastPairedWiiRemoteID[index] = WiiRemotes[index].UniqueID;
			}
		}
	}
	return true;
}

void UWiiRemoteManager::ShutDown()
{
	
}

void UWiiRemoteManager::Reset()
{
	ShutDown();
	Startup();
}

void UWiiRemoteManager::Tick(float deltaTime)
{
	for (auto index = 0; index < ChangeFlags.Num(); index++)
	{
		auto wiiRemote = &WiiRemotes[index];
		if (ChangeFlags[index].IsConnected)
		{
			OnConnected(wiiRemote);
			ChangeFlags[index].IsConnected = false;
		}
		if (ChangeFlags[index].IsConnectionLost)
		{
			OnConnectionLost(wiiRemote);
			ChangeFlags[index].IsConnectionLost = false;
		}
		if (ChangeFlags[index].IsBatteryChanged)
		{
			OnBatteryChanged(wiiRemote, wiiRemote->BatteryPercent);
			ChangeFlags[index].IsBatteryChanged = false;
		}
		if (ChangeFlags[index].IsBatteryDrained)
		{
			OnBatteryDrained(wiiRemote);
			ChangeFlags[index].IsBatteryDrained = false;
		}
		if (ChangeFlags[index].IsLEDChanged)
		{
			OnLEDsChanged(wiiRemote, wiiRemote->LED.Bits);
			ChangeFlags[index].IsLEDChanged = false;
		}
		if (ChangeFlags[index].IsNunchukConnected)
		{
			OnNunchukConnected(wiiRemote);
			ChangeFlags[index].IsNunchukConnected = false;
		}
		if (ChangeFlags[index].IsClassicConnected)
		{
			OnClassicConnected(wiiRemote);
			ChangeFlags[index].IsClassicConnected = false;
		}
		if (ChangeFlags[index].IsBalanceConnected)
		{
			OnBalanceConnected(wiiRemote);
			ChangeFlags[index].IsBalanceConnected = false;
		}
		if (ChangeFlags[index].IsMotionPlusDetected)
		{
			OnMotionPlusDetected(wiiRemote);
			ChangeFlags[index].IsMotionPlusDetected = false;
		}
		if (ChangeFlags[index].IsMotionPlusEnabled)
		{
			OnMotionPlusEnabled(wiiRemote);
			ChangeFlags[index].IsMotionPlusEnabled = false;
		}
		if (ChangeFlags[index].IsMotionPlusExtensionConnected)
		{
			OnMotionPlusExtensionConnected(wiiRemote);
			ChangeFlags[index].IsMotionPlusExtensionConnected = false;
		}
		if (ChangeFlags[index].IsMotionPlusExtensionDisconnected)
		{
			OnMotionPlusExtensionDisconnected(wiiRemote);
			ChangeFlags[index].IsMotionPlusExtensionDisconnected = false;
		}
		if (ChangeFlags[index].IsExtensionDisconnected)
		{
			OnExtensionDisconnected(wiiRemote);
			ChangeFlags[index].IsExtensionDisconnected = false;
		}
		if (ChangeFlags[index].IsAccelChanged)
		{
			OnAccelChanged(wiiRemote, FVector(wiiRemote->Acceleration.X, wiiRemote->Acceleration.Y, wiiRemote->Acceleration.Z));
			ChangeFlags[index].IsAccelChanged = false;
		}
		if (ChangeFlags[index].IsButtonsChanged)
		{
			FWiiRemoteButtons buttons;
			UWiiRemoteManager::SetWiiRemoteButtons(*wiiRemote, buttons);
			OnButtonsChanged(wiiRemote, buttons);
			ChangeFlags[index].IsButtonsChanged = false;
		}
		if (ChangeFlags[index].IsOrientationChanged)
		{
			OnOrientationChanged(wiiRemote, wiiRemote->Acceleration.Orientation.Pitch, wiiRemote->Acceleration.Orientation.Roll);
			ChangeFlags[index].IsOrientationChanged = false;
		}
		if (ChangeFlags[index].IsIRChanged)
		{
			TArray<FWiiRemoteDot> dots;
			UWiiRemoteManager::SetWiiRemoteDots(*wiiRemote, dots);
			OnIRChanged(wiiRemote, dots);
			ChangeFlags[index].IsIRChanged = false;
		}
		if (ChangeFlags[index].IsNunchukAccelChanged)
		{
			OnNunchukAccelChanged(wiiRemote, FVector(wiiRemote->Nunchuk.Acceleration.X, wiiRemote->Nunchuk.Acceleration.Y, wiiRemote->Nunchuk.Acceleration.Z));
			ChangeFlags[index].IsNunchukAccelChanged = false;
		}
		if (ChangeFlags[index].IsNunchukButtonsChanged)
		{
			FWiiRemoteNunchukButtons buttons;
			UWiiRemoteManager::SetWiiRemoteNunchukButtons(*wiiRemote, buttons);
			OnNunchukButtonsChanged(wiiRemote, buttons);
			ChangeFlags[index].IsNunchukButtonsChanged = false;
		}
		if (ChangeFlags[index].IsNunchukJoystickChanged)
		{
			OnNunchukJoystickChanged(wiiRemote, wiiRemote->Nunchuk.Joystick.X, wiiRemote->Nunchuk.Joystick.Y);
			ChangeFlags[index].IsNunchukJoystickChanged = false;
		}
		if (ChangeFlags[index].IsNunchukOrientationChanged)
		{
			OnNunchukOrientationChanged(wiiRemote, wiiRemote->Nunchuk.Acceleration.Orientation.Pitch, wiiRemote->Nunchuk.Acceleration.Orientation.Roll);
			ChangeFlags[index].IsNunchukOrientationChanged = false;
		}
		if (ChangeFlags[index].IsClassicButtonsChanged)
		{
			FWiiRemoteClassicControllerButtons buttons;
			UWiiRemoteManager::SetWiiRemoteClassicControllerButtons(*wiiRemote, buttons);
			OnClassicButtonsChanged(wiiRemote, buttons);
			ChangeFlags[index].IsClassicButtonsChanged = false;
		}
		if (ChangeFlags[index].IsClassicJoystickLChanged)
		{
			OnClassicJoystickLChanged(wiiRemote, wiiRemote->ClassicController.JoystickL.X, wiiRemote->ClassicController.JoystickL.Y);
			ChangeFlags[index].IsClassicJoystickLChanged = false;
		}
		if (ChangeFlags[index].IsClassicJoystickRChanged)
		{
			OnClassicJoystickRChanged(wiiRemote, wiiRemote->ClassicController.JoystickR.X, wiiRemote->ClassicController.JoystickR.Y);
			ChangeFlags[index].IsClassicJoystickRChanged = false;
		}
		if (ChangeFlags[index].IsClassicTriggerChanged)
		{
			OnClassicTriggersChanged(wiiRemote, wiiRemote->ClassicController.TriggerL, wiiRemote->ClassicController.TriggerR);
			ChangeFlags[index].IsClassicTriggerChanged = false;
		}
		if (ChangeFlags[index].IsBalanceWeightChanged)
		{
			FWiiRemoteBalanceBoard balanceBoard;
			SetWiiRemoteBalanceBoard(*wiiRemote, balanceBoard);
			OnBalanceWeightChanged(wiiRemote, balanceBoard);
			ChangeFlags[index].IsBalanceWeightChanged = false;
		}
		if (ChangeFlags[index].IsMotionPlusSpeedChanged)
		{
			OnMotionPlusSpeedChanged(wiiRemote, FRotator(wiiRemote->MotionPlus.Speed.Pitch, wiiRemote->MotionPlus.Speed.Yaw, wiiRemote->MotionPlus.Speed.Roll));
			ChangeFlags[index].IsMotionPlusSpeedChanged = false;
		}
	}
}

int32 UWiiRemoteManager::IdentifyWiiRemote(uint64 wiiRemoteId)
{
	//for (auto i = 0; i < WiiRemotes.Num(); i++)
	for (auto i = 0; i < 4; i++)
	{
		if (WiiRemotes[i].IsConnected())
		{
			if (WiiRemotes[i].UniqueID == wiiRemoteId)
				return i + 1;
		}
	}
	return 0;
}

wiimote* UWiiRemoteManager::LastValidWiiRemote()
{
	for (auto i = 0; i < Data.Num(); i++)
	{
		auto wiiRemote = WiiRemotes[i];
		if (wiiRemote.IsConnected())
			return &WiiRemotes[i];
	}
	return nullptr;
}

bool UWiiRemoteManager::IsWiiRemoteValidForInputMapping(uint64 wiiRemoteId)
{
	for (auto i = 0; i < LastPairedWiiRemoteID.Num(); i++)
		if (wiiRemoteId == LastPairedWiiRemoteID[i])
			return true;
	return false;
}

int32 UWiiRemoteManager::WiiRemoteIndexForWiiRemote(uint64 wiiRemoteId)
{
	return IdentifyWiiRemote(wiiRemoteId) - 1;
}

void UWiiRemoteManager::OnConnected(wiimote* wiiRemote)
{
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d  has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnConnected(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnConnectionLost(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	Data[index].IsInitialized = false;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d  has disconnected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	wiiRemote->SetLEDs(0x00);
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnCennectionLost(IdentifyWiiRemote(wiiRemote->UniqueID));
	for (auto i = 0; i < LastPairedWiiRemoteID.Num(); i++)
	{
		if (wiiRemote->UniqueID == LastPairedWiiRemoteID[i])
			LastPairedWiiRemoteID[i] = LastValidWiiRemote()->UniqueID;
	}
}

void UWiiRemoteManager::OnBatteryChanged(wiimote* wiiRemote, uint8 batteryPercent)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Battery remains %d %"), batteryPercent);
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBatteryChanged(wiiRemote->UniqueID, static_cast<int32>(batteryPercent));
}

void UWiiRemoteManager::OnBatteryDrained(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Battery has drained."));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBatteryDrained(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnLEDsChanged(wiimote* wiiRemote, uint8 ledBits)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnLEDsChanged(wiiRemote->UniqueID, static_cast<WiiRemoteLED>(ledBits));
}

void UWiiRemoteManager::OnButtonsChanged(wiimote* wiiRemote, FWiiRemoteButtons buttons)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			const auto dataButtons = Data[index].Buttons;
			UpdateButtons(dataButtons.IsPushA, buttons.IsPushA, EWiiRemoteKeys::A, index);
			UpdateButtons(dataButtons.IsPushB, buttons.IsPushB, EWiiRemoteKeys::B, index);
			UpdateButtons(dataButtons.IsPushDown, buttons.IsPushDown, EWiiRemoteKeys::Down, index);
			UpdateButtons(dataButtons.IsPushHome, buttons.IsPushHome, EWiiRemoteKeys::Home, index);
			UpdateButtons(dataButtons.IsPushLeft, buttons.IsPushLeft, EWiiRemoteKeys::Left, index);
			UpdateButtons(dataButtons.IsPushMinus, buttons.IsPushMinus, EWiiRemoteKeys::Minus, index);
			UpdateButtons(dataButtons.IsPushOne, buttons.IsPushOne, EWiiRemoteKeys::One, index);
			UpdateButtons(dataButtons.IsPushPlus, buttons.IsPushPlus, EWiiRemoteKeys::Plus, index);
			UpdateButtons(dataButtons.IsPushRight, buttons.IsPushRight, EWiiRemoteKeys::Right, index);
			UpdateButtons(dataButtons.IsPushTwo, buttons.IsPushTwo, EWiiRemoteKeys::Two, index);
			UpdateButtons(dataButtons.IsPushUp, buttons.IsPushUp, EWiiRemoteKeys::Up, index);
		}
		Data[index].Buttons = buttons;
		WiiRemoteDelegate->OnButtonsChanged(wiiRemote->UniqueID, Data[index].Buttons);
	}
}

void UWiiRemoteManager::OnAccelChanged(wiimote* wiiRemote, FVector accel)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].Acceleration = accel;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteKeys::AccelerationX, Data[index].Acceleration.X, index);
			EmitAnalogInputEventForKey(EWiiRemoteKeys::AccelerationY, Data[index].Acceleration.Y, index);
			EmitAnalogInputEventForKey(EWiiRemoteKeys::AccelerationZ, Data[index].Acceleration.Z, index);
		}
		WiiRemoteDelegate->OnAccelChanged(wiiRemote->UniqueID, Data[index].Acceleration);
	}
}

void UWiiRemoteManager::OnOrientationChanged(wiimote* wiiRemote, float pitch, float roll)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		UE_LOG(LogTemp, Warning, TEXT("WiiRemote Orientation Pitch:%f, Roll:%f"), pitch, roll);
		Data[index].OrientationPitch = pitch;
		Data[index].OrientationRoll = roll;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteKeys::OrientationPitch, Data[index].OrientationPitch, index);
			EmitAnalogInputEventForKey(EWiiRemoteKeys::OrientationRoll, Data[index].OrientationRoll, index);
		}
		WiiRemoteDelegate->OnOrientationChanged(wiiRemote->UniqueID, Data[index].OrientationPitch, Data[index].OrientationRoll);
	}
}

void UWiiRemoteManager::OnIRChanged(wiimote* wiiRemote, TArray<FWiiRemoteDot> dots)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].Dots = dots;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			if (Data[index].Dots[0].bVisible)
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot1_X, Data[index].Dots[0].X, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot1_Y, Data[index].Dots[0].Y, index);
			}
			else
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot1_X, 0.0f, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot1_Y, 0.0f, index);
			}
			if (Data[index].Dots[1].bVisible)
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot2_X, Data[index].Dots[1].X, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot2_Y, Data[index].Dots[1].Y, index);
			}
			else
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot2_X, 0.0f, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot2_Y, 0.0f, index);
			}
			if (Data[index].Dots[2].bVisible)
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot3_X, Data[index].Dots[2].X, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot3_Y, Data[index].Dots[2].Y, index);
			}
			else
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot3_X, 0.0f, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot3_Y, 0.0f, index);
			}
			if (Data[index].Dots[3].bVisible)
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot4_X, Data[index].Dots[3].X, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot4_Y, Data[index].Dots[3].Y, index);
			}
			else
			{
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot4_X, 0.0f, index);
				EmitAnalogInputEventForKey(EWiiRemoteKeys::Dot4_Y, 0.0f, index);
			}
		}
		WiiRemoteDelegate->OnIRChanged(wiiRemote->UniqueID, Data[index].Dots);
	}
}

void UWiiRemoteManager::OnNunchukConnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Nunchuk has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectNunchuk = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnNunchukConnected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnNunchukButtonsChanged(wiimote* wiiRemote, FWiiRemoteNunchukButtons buttons)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			const auto dataButtons = Data[index].NunchukButtons;
			UpdateButtons(dataButtons.IsPushC, buttons.IsPushC, EWiiRemoteNunchukKeys::C, index);
			UpdateButtons(dataButtons.IsPushZ, buttons.IsPushZ, EWiiRemoteNunchukKeys::Z, index);
		}
		Data[index].NunchukButtons = buttons;
		WiiRemoteDelegate->OnNunchukButtonsChanged(wiiRemote->UniqueID, Data[index].NunchukButtons);
	}
}

void UWiiRemoteManager::OnNunchukAccelChanged(wiimote* wiiRemote, FVector accel)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].NunchukAcceleration = accel;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::AccelerationX, Data[index].NunchukAcceleration.X, index);
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::AccelerationY, Data[index].NunchukAcceleration.Y, index);
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::AccelerationZ, Data[index].NunchukAcceleration.Z, index);
		}
		WiiRemoteDelegate->OnNunchukAccelChanged(wiiRemote->UniqueID, Data[index].NunchukAcceleration);
	}
}

void UWiiRemoteManager::OnNunchukOrientationChanged(wiimote* wiiRemote, float pitch, float roll)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].NunchukOrientationPitch = pitch;
		Data[index].NunchukOrientationRoll = roll;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::OrientationPitch, Data[index].NunchukOrientationPitch, index);
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::OrientationRoll, Data[index].NunchukOrientationRoll, index);
		}
		WiiRemoteDelegate->OnNunchukOrientationChanged(wiiRemote->UniqueID, Data[index].NunchukOrientationPitch, Data[index].NunchukOrientationRoll);
	}
}

void UWiiRemoteManager::OnNunchukJoystickChanged(wiimote* wiiRemote, float x, float y)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].NunchukJoystickX = x;
		Data[index].NunchukJoystickY = y;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::JoystickX, Data[index].NunchukJoystickX, index);
			EmitAnalogInputEventForKey(EWiiRemoteNunchukKeys::JoystickY, Data[index].NunchukJoystickY, index);
		}
		WiiRemoteDelegate->OnNunchukJoystickChanged(wiiRemote->UniqueID, Data[index].NunchukJoystickX, Data[index].NunchukJoystickY);
	}
}

void UWiiRemoteManager::OnClassicConnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d ClassicController has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectClassicController = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnClassicConnected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnClassicButtonsChanged(wiimote* wiiRemote, FWiiRemoteClassicControllerButtons buttons)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			const auto dataButtons = Data[index].ClassicControllerButtons;
			UpdateButtons(dataButtons.IsPushA, buttons.IsPushA, EWiiRemoteClassicControllerKeys::A, index);
			UpdateButtons(dataButtons.IsPushB, buttons.IsPushB, EWiiRemoteClassicControllerKeys::B, index);
			UpdateButtons(dataButtons.IsPushDown, buttons.IsPushDown, EWiiRemoteClassicControllerKeys::Down, index);
			UpdateButtons(dataButtons.IsPushHome, buttons.IsPushHome, EWiiRemoteClassicControllerKeys::Home, index);
			UpdateButtons(dataButtons.IsPushLeft, buttons.IsPushLeft, EWiiRemoteClassicControllerKeys::Left, index);
			UpdateButtons(dataButtons.IsPushMinus, buttons.IsPushMinus, EWiiRemoteClassicControllerKeys::Minus, index);
			UpdateButtons(dataButtons.IsPushPlus, buttons.IsPushPlus, EWiiRemoteClassicControllerKeys::Plus, index);
			UpdateButtons(dataButtons.IsPushRight, buttons.IsPushRight, EWiiRemoteClassicControllerKeys::Right, index);
			UpdateButtons(dataButtons.IsPushTriggerL, buttons.IsPushTriggerL, EWiiRemoteClassicControllerKeys::TriggerL, index);
			UpdateButtons(dataButtons.IsPushTriggerR, buttons.IsPushTriggerR, EWiiRemoteClassicControllerKeys::TrigerrR, index);
			UpdateButtons(dataButtons.IsPushUp, buttons.IsPushUp, EWiiRemoteClassicControllerKeys::Up, index);
			UpdateButtons(dataButtons.IsPushX, buttons.IsPushX, EWiiRemoteClassicControllerKeys::X, index);
			UpdateButtons(dataButtons.IsPushY, buttons.IsPushY, EWiiRemoteClassicControllerKeys::Y, index);
			UpdateButtons(dataButtons.IsPushZL, buttons.IsPushZL, EWiiRemoteClassicControllerKeys::ZL, index);
			UpdateButtons(dataButtons.IsPushZR, buttons.IsPushZR, EWiiRemoteClassicControllerKeys::ZR, index);
		}
		Data[index].ClassicControllerButtons = buttons;
		WiiRemoteDelegate->OnClassicButtonsChanged(wiiRemote->UniqueID, Data[index].ClassicControllerButtons);
	}
}

void UWiiRemoteManager::OnClassicJoystickLChanged(wiimote* wiiRemote, float x, float y)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].ClassicControllerLeftJoystickX = x;
		Data[index].ClassicControllerLeftJoystickY = y;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::LeftJoystickX, Data[index].ClassicControllerLeftJoystickX, index);
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::LeftJoystickY, Data[index].ClassicControllerLeftJoystickY, index);
		}
		WiiRemoteDelegate->OnClassicJoystickLChanged(wiiRemote->UniqueID, Data[index].ClassicControllerLeftJoystickX, Data[index].ClassicControllerLeftJoystickY);
	}
}

void UWiiRemoteManager::OnClassicJoystickRChanged(wiimote* wiiRemote, float x, float y)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].ClassicControllerRightJoystickX = x;
		Data[index].ClassicControllerRightJoystickY = y;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::RightJoystickX, Data[index].ClassicControllerRightJoystickX, index);
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::RightJoystickY, Data[index].ClassicControllerRightJoystickY, index);
		}
		WiiRemoteDelegate->OnClassicJoystickRChanged(wiiRemote->UniqueID, Data[index].ClassicControllerRightJoystickX, Data[index].ClassicControllerRightJoystickY);
	}
}

void UWiiRemoteManager::OnClassicTriggersChanged(wiimote* wiiRemote, float left, float right)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].ClassicControllerLeftTrigger = left;
		Data[index].ClassicControllerRightTrigger = right;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::LeftTrigger, Data[index].ClassicControllerLeftTrigger, index);
			EmitAnalogInputEventForKey(EWiiRemoteClassicControllerKeys::RightTrigger, Data[index].ClassicControllerRightTrigger, index);
		}
		WiiRemoteDelegate->OnClassicTriggersChanged(wiiRemote->UniqueID, Data[index].ClassicControllerLeftTrigger, Data[index].ClassicControllerRightTrigger);
	}
}

void UWiiRemoteManager::OnBalanceConnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d BalanceBoard has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectBalanceBoard = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBalanceConnected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnBalanceWeightChanged(wiimote* wiiRemote, FWiiRemoteBalanceBoard balanceBoard)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].BalanceBoard = balanceBoard;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomL, Data[index].BalanceBoard.AtRestKilograms.BottomL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::AtRestKilograms_BottomR, Data[index].BalanceBoard.AtRestKilograms.BottomR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopL, Data[index].BalanceBoard.AtRestKilograms.TopL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::AtRestKilograms_TopR, Data[index].BalanceBoard.AtRestKilograms.TopR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::AtRestKilograms_Total, Data[index].BalanceBoard.AtRestKilograms.Total, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Kilograms_BottomL, Data[index].BalanceBoard.Kilograms.BottomL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Kilograms_BottomR, Data[index].BalanceBoard.Kilograms.BottomR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Kilograms_TopL, Data[index].BalanceBoard.Kilograms.TopL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Kilograms_TopR, Data[index].BalanceBoard.Kilograms.TopR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Kilograms_Total, Data[index].BalanceBoard.Kilograms.Total, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Pounds_BottomL, Data[index].BalanceBoard.Pounds.BottomL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Pounds_BottomR, Data[index].BalanceBoard.Pounds.BottomR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Pounds_TopL, Data[index].BalanceBoard.Pounds.TopL, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Pounds_TopR, Data[index].BalanceBoard.Pounds.TopR, index);
			EmitAnalogInputEventForKey(EWiiRemoteBalanceBoardKeys::Pounds_Total, Data[index].BalanceBoard.Pounds.Total, index);
		}
		WiiRemoteDelegate->OnBalanceWeightChanged(wiiRemote->UniqueID, Data[index].BalanceBoard);
	}
}

void UWiiRemoteManager::OnMotionPlusDetected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusDetected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnMotionPlusEnabled(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus is enabled."), IdentifyWiiRemote(wiiRemote->UniqueID));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusEnabled(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnMotionPlusSpeedChanged(wiimote* wiiRemote, FRotator speed)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].MotionPlusSpeed = speed;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteMotionPlusKeys::SpeedOrientationPitch, Data[index].MotionPlusSpeed.Pitch, index);
			EmitAnalogInputEventForKey(EWiiRemoteMotionPlusKeys::SpeedOrientationYaw, Data[index].MotionPlusSpeed.Yaw, index);
			EmitAnalogInputEventForKey(EWiiRemoteMotionPlusKeys::SpeedOrientationRoll, Data[index].MotionPlusSpeed.Roll, index);
		}
		WiiRemoteDelegate->OnMotionPlusSpeedChanged(wiiRemote->UniqueID, Data[index].MotionPlusSpeed);
	}
}

void UWiiRemoteManager::OnMotionPlusExtensionConnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has connected extension."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusExtensionConnected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnMotionPlusExtensionDisconnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has disconnected extension."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusExtensionDisconnected(wiiRemote->UniqueID);
}

void UWiiRemoteManager::OnExtensionDisconnected(wiimote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d has disconnected extension."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectBalanceBoard = false;
	Data[index].IsConnectClassicController = false;
	Data[index].IsConnectMotionPlus = false;
	Data[index].IsConnectNunchuk = false;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnExtensionDisconnected(wiiRemote->UniqueID);
}

void OnStateChanged(wiimote& wiiRemote, state_change_flags changed, const wiimote_state& new_state)
{
	if (changed & state_change_flags::CONNECTED)
	{
		if (new_state.ExtensionType != wiimote::BALANCE_BOARD)
			wiiRemote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
		else
			wiiRemote.SetReportType(wiimote::IN_BUTTONS_BALANCE_BOARD);
	}
	if (changed & state_change_flags::CHANGED_ALL)
		wiiRemote.RefreshState();
	if (changed & state_change_flags::MOTIONPLUS_DETECTED)
		wiiRemote.EnableMotionPlus();
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_CONNECTED)
	{
		if (wiiRemote.MotionPlusEnabled())
			wiiRemote.EnableMotionPlus();
	}
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_DISCONNECTED)
	{
		if (wiiRemote.MotionPlusConnected())
			wiiRemote.EnableMotionPlus();
	}

	auto index = manager->WiiRemoteIndexForWiiRemote(wiiRemote.UniqueID);
	if (index == -1)
		return;
	
	if (changed & state_change_flags::CONNECTED)
		manager->ChangeFlags[index].IsConnected = true;
	if (changed & state_change_flags::CONNECTION_LOST)
		manager->ChangeFlags[index].IsConnectionLost = true;
	if (changed & state_change_flags::BATTERY_CHANGED)
		manager->ChangeFlags[index].IsBatteryChanged = true;
	if (changed & state_change_flags::BATTERY_DRAINED)
		manager->ChangeFlags[index].IsBatteryDrained = true;
	if (changed & state_change_flags::LEDS_CHANGED)
		manager->ChangeFlags[index].IsLEDChanged = true;
	if (changed & state_change_flags::NUNCHUK_CONNECTED)
		manager->ChangeFlags[index].IsNunchukConnected = true;
	if (changed & state_change_flags::CLASSIC_CONNECTED)
		manager->ChangeFlags[index].IsClassicConnected = true;
	if (changed & state_change_flags::BALANCE_CONNECTED)
		manager->ChangeFlags[index].IsBalanceConnected = true;
	if (changed & state_change_flags::MOTIONPLUS_DETECTED)
		manager->ChangeFlags[index].IsMotionPlusDetected = true;
	if (changed & state_change_flags::MOTIONPLUS_ENABLED)
		manager->ChangeFlags[index].IsMotionPlusEnabled = true;
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_CONNECTED)
		manager->ChangeFlags[index].IsMotionPlusExtensionConnected = true;
	if (changed & state_change_flags::MOTIONPLUS_EXTENSION_DISCONNECTED)
		manager->ChangeFlags[index].IsMotionPlusExtensionDisconnected = true;
	if (changed & state_change_flags::EXTENSION_DISCONNECTED)
		manager->ChangeFlags[index].IsExtensionDisconnected = true;
	if (changed & state_change_flags::BUTTONS_CHANGED)
		manager->ChangeFlags[index].IsButtonsChanged = true;
	if (changed & state_change_flags::ACCEL_CHANGED)
		manager->ChangeFlags[index].IsAccelChanged = true;
	if (changed & state_change_flags::ORIENTATION_CHANGED)
		manager->ChangeFlags[index].IsOrientationChanged = true;
	if (changed & state_change_flags::IR_CHANGED)
		manager->ChangeFlags[index].IsIRChanged = true;
	if (changed & state_change_flags::NUNCHUK_BUTTONS_CHANGED)
		manager->ChangeFlags[index].IsNunchukButtonsChanged = true;
	if (changed & state_change_flags::NUNCHUK_ACCEL_CHANGED)
		manager->ChangeFlags[index].IsNunchukAccelChanged = true;
	if (changed & state_change_flags::NUNCHUK_ORIENTATION_CHANGED)
		manager->ChangeFlags[index].IsNunchukOrientationChanged = true;
	if (changed & state_change_flags::NUNCHUK_JOYSTICK_CHANGED)
		manager->ChangeFlags[index].IsNunchukJoystickChanged = true;
	if (changed & state_change_flags::CLASSIC_BUTTONS_CHANGED)
		manager->ChangeFlags[index].IsClassicButtonsChanged = true;
	if (changed & state_change_flags::CLASSIC_JOYSTICK_L_CHANGED)
		manager->ChangeFlags[index].IsClassicJoystickLChanged = true;
	if (changed & state_change_flags::CLASSIC_JOYSTICK_R_CHANGED)
		manager->ChangeFlags[index].IsClassicJoystickRChanged = true;
	if (changed & state_change_flags::CLASSIC_TRIGGERS_CHANGED)
		manager->ChangeFlags[index].IsClassicTriggerChanged = true;
	if (changed & state_change_flags::BALANCE_WEIGHT_CHANGED)
		manager->ChangeFlags[index].IsBalanceWeightChanged = true;
	if (changed & state_change_flags::MOTIONPLUS_SPEED_CHANGED)
		manager->ChangeFlags[index].IsMotionPlusSpeedChanged = true;
}

void UWiiRemoteManager::SetWiiRemoteButtons(wiimote& wiiRemote, FWiiRemoteButtons& buttons)
{
	buttons.IsPushA = wiiRemote.Button.A();
	buttons.IsPushB = wiiRemote.Button.B();
	buttons.IsPushUp = wiiRemote.Button.Up();
	buttons.IsPushDown = wiiRemote.Button.Down();
	buttons.IsPushRight = wiiRemote.Button.Right();
	buttons.IsPushLeft = wiiRemote.Button.Left();
	buttons.IsPushOne = wiiRemote.Button.One();
	buttons.IsPushTwo = wiiRemote.Button.Two();
	buttons.IsPushPlus = wiiRemote.Button.Plus();
	buttons.IsPushMinus = wiiRemote.Button.Minus();
	buttons.IsPushHome = wiiRemote.Button.Home();
}

void UWiiRemoteManager::SetWiiRemoteDots(wiimote& wiiRemote, TArray<FWiiRemoteDot>& dots)
{
	dots.Init(FWiiRemoteDot(), 4);
	for (auto index = 0; index < dots.Num(); index++)
	{
		dots[index].bVisible = wiiRemote.IR.Dot[index].bVisible;
		dots[index].X = wiiRemote.IR.Dot[index].X;
		dots[index].Y = wiiRemote.IR.Dot[index].Y;
	}
}

void UWiiRemoteManager::SetWiiRemoteNunchukButtons(wiimote& wiiRemote, FWiiRemoteNunchukButtons& buttons)
{
	buttons.IsPushC = wiiRemote.Nunchuk.C;
	buttons.IsPushZ = wiiRemote.Nunchuk.Z;
}

void UWiiRemoteManager::SetWiiRemoteClassicControllerButtons(wiimote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons)
{
	buttons.IsPushA = wiiRemote.ClassicController.Button.A();
	buttons.IsPushB = wiiRemote.ClassicController.Button.B();
	buttons.IsPushX = wiiRemote.ClassicController.Button.X();
	buttons.IsPushY = wiiRemote.ClassicController.Button.Y();
	buttons.IsPushUp = wiiRemote.ClassicController.Button.Up();
	buttons.IsPushDown = wiiRemote.ClassicController.Button.Down();
	buttons.IsPushRight = wiiRemote.ClassicController.Button.Right();
	buttons.IsPushLeft = wiiRemote.ClassicController.Button.Left();
	buttons.IsPushPlus = wiiRemote.ClassicController.Button.Plus();
	buttons.IsPushMinus = wiiRemote.ClassicController.Button.Minus();
	buttons.IsPushHome = wiiRemote.ClassicController.Button.Home();
	buttons.IsPushZL = wiiRemote.ClassicController.Button.ZL();
	buttons.IsPushZR = wiiRemote.ClassicController.Button.ZR();
	buttons.IsPushTriggerL = wiiRemote.ClassicController.Button.TriggerL();
	buttons.IsPushTriggerR = wiiRemote.ClassicController.Button.TriggerR();
}

void UWiiRemoteManager::SetWiiRemoteBalanceBoard(wiimote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard)
{
	balanceBoard.AtRestKilograms.BottomL = wiiRemote.BalanceBoard.AtRestKg.BottomL;
	balanceBoard.AtRestKilograms.BottomR = wiiRemote.BalanceBoard.AtRestKg.BottomR;
	balanceBoard.AtRestKilograms.TopL = wiiRemote.BalanceBoard.AtRestKg.TopL;
	balanceBoard.AtRestKilograms.TopR = wiiRemote.BalanceBoard.AtRestKg.TopR;
	balanceBoard.AtRestKilograms.Total = wiiRemote.BalanceBoard.AtRestKg.Total;
	balanceBoard.Kilograms.BottomL = wiiRemote.BalanceBoard.Kg.BottomL;
	balanceBoard.Kilograms.BottomR = wiiRemote.BalanceBoard.Kg.BottomR;
	balanceBoard.Kilograms.TopL = wiiRemote.BalanceBoard.Kg.TopL;
	balanceBoard.Kilograms.TopR = wiiRemote.BalanceBoard.Kg.TopR;
	balanceBoard.Kilograms.Total = wiiRemote.BalanceBoard.Kg.Total;
	balanceBoard.Pounds.BottomL = wiiRemote.BalanceBoard.Lb.BottomL;
	balanceBoard.Pounds.BottomR = wiiRemote.BalanceBoard.Lb.BottomR;
	balanceBoard.Pounds.TopL = wiiRemote.BalanceBoard.Lb.TopL;
	balanceBoard.Pounds.TopR = wiiRemote.BalanceBoard.Lb.TopR;
	balanceBoard.Pounds.Total = wiiRemote.BalanceBoard.Lb.Total;
}

bool UWiiRemoteManager::EmitKeyUpEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent keyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyUpEvent(keyEvent);
}

bool UWiiRemoteManager::EmitKeyDownEventForKey(FKey key, int32 user, bool repeat)
{
	FKeyEvent keyEvent(key, FSlateApplication::Get().GetModifierKeys(), user, repeat, 0, 0);
	return FSlateApplication::Get().ProcessKeyDownEvent(keyEvent);
}

bool UWiiRemoteManager::EmitAnalogInputEventForKey(FKey key, float value, int32 user)
{
	FAnalogInputEvent analogInputEvent(key, FSlateApplication::Get().GetModifierKeys(), user, false, 0, 0, value);
	return FSlateApplication::Get().ProcessAnalogInputEvent(analogInputEvent);
}

FRotator UWiiRemoteManager::ConvertOrientationToUE(FRotator rawOrientation)
{
	return FRotator(rawOrientation.Pitch * -1.0f, rawOrientation.Yaw * -1.0f, rawOrientation.Roll);
}

FVector UWiiRemoteManager::ConvertVectorToUE(FVector rawAcceleration)
{
	return FVector(rawAcceleration.X, -rawAcceleration.Y, rawAcceleration.Z);
}

void UWiiRemoteManager::UpdateButtons(bool oldState, bool newState, FKey key, int32 user)
{
	if (oldState != newState)
		newState ? EmitKeyDownEventForKey(key, user, false) : EmitKeyUpEventForKey(key, user, false);
}
