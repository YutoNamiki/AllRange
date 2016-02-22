#include "WiiRemotePluginPrivatePCH.h"
#include "WiiRemoteManager.h"
#include "WiiRemoteDelegate.h"
#include "FWiiRemotePlugin.h"

UWiiRemoteManager* manager;

void OnStateChanged(WiiRemote& wiiRemote, StateChangeFlags changed, const WiiRemoteState& newState);

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
		WiiRemotes[index].CallbackTriggerFlags = static_cast<StateChangeFlags>((static_cast<int>(StateChangeFlags::ChangedAll) | static_cast<int>(StateChangeFlags::Connected)));
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
	id = 0;
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
			OnAccelChanged(wiiRemote, FVector(wiiRemote->Acceleration.Data.X, wiiRemote->Acceleration.Data.Y, wiiRemote->Acceleration.Data.Z));
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
			OnOrientationChanged(wiiRemote, wiiRemote->Acceleration.Orientation.Rotation.Pitch, wiiRemote->Acceleration.Orientation.Rotation.Roll);
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
			OnNunchukAccelChanged(wiiRemote, FVector(wiiRemote->Nunchuk.Acceleration.Data.X, wiiRemote->Nunchuk.Acceleration.Data.Y, wiiRemote->Nunchuk.Acceleration.Data.Z));
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
			OnNunchukJoystickChanged(wiiRemote, wiiRemote->Nunchuk.Joystick.Data.X, wiiRemote->Nunchuk.Joystick.Data.Y);
			ChangeFlags[index].IsNunchukJoystickChanged = false;
		}
		if (ChangeFlags[index].IsNunchukOrientationChanged)
		{
			OnNunchukOrientationChanged(wiiRemote, wiiRemote->Nunchuk.Acceleration.Orientation.Rotation.Pitch, wiiRemote->Nunchuk.Acceleration.Orientation.Rotation.Roll);
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
			OnClassicJoystickLChanged(wiiRemote, wiiRemote->ClassicController.JoystickL.Data.X, wiiRemote->ClassicController.JoystickL.Data.Y);
			ChangeFlags[index].IsClassicJoystickLChanged = false;
		}
		if (ChangeFlags[index].IsClassicJoystickRChanged)
		{
			OnClassicJoystickRChanged(wiiRemote, wiiRemote->ClassicController.JoystickR.Data.X, wiiRemote->ClassicController.JoystickR.Data.Y);
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

WiiRemote* UWiiRemoteManager::LastValidWiiRemote()
{
	for (auto i = 0; i < Data.Num(); i++)
	{
		auto& wiiRemote = WiiRemotes[i];
		if (wiiRemote.IsConnected())
			return &wiiRemote;
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

void UWiiRemoteManager::OnConnected(WiiRemote* wiiRemote)
{
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d  has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnConnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnConnectionLost(WiiRemote* wiiRemote)
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
		WiiRemoteDelegate->OnConnectionLostFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
	for (auto i = 0; i < LastPairedWiiRemoteID.Num(); i++)
	{
		if (wiiRemote->UniqueID == LastPairedWiiRemoteID[i])
			LastPairedWiiRemoteID[i] = LastValidWiiRemote()->UniqueID;
	}
}

void UWiiRemoteManager::OnBatteryChanged(WiiRemote* wiiRemote, uint8 batteryPercent)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Battery remains %d %"), batteryPercent);
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBatteryChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), static_cast<int32>(batteryPercent));
}

void UWiiRemoteManager::OnBatteryDrained(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Battery has drained."));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBatteryDrainedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnLEDsChanged(WiiRemote* wiiRemote, uint8 ledBits)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnLEDsChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), static_cast<WiiRemoteLED>(ledBits));
}

void UWiiRemoteManager::OnButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteButtons buttons)
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
		WiiRemoteDelegate->OnButtonsChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].Buttons);
	}
}

void UWiiRemoteManager::OnAccelChanged(WiiRemote* wiiRemote, FVector accel)
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
		WiiRemoteDelegate->OnAccelChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].Acceleration);
	}
}

void UWiiRemoteManager::OnOrientationChanged(WiiRemote* wiiRemote, float pitch, float roll)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	if (!Data[index].IsInitialized)
		return;
	if (WiiRemoteDelegate)
	{
		Data[index].OrientationPitch = pitch;
		Data[index].OrientationRoll = roll;
		if (IsWiiRemoteValidForInputMapping(wiiRemote->UniqueID))
		{
			EmitAnalogInputEventForKey(EWiiRemoteKeys::OrientationPitch, Data[index].OrientationPitch, index);
			EmitAnalogInputEventForKey(EWiiRemoteKeys::OrientationRoll, Data[index].OrientationRoll, index);
		}
		WiiRemoteDelegate->OnOrientationChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].OrientationPitch, Data[index].OrientationRoll);
	}
}

void UWiiRemoteManager::OnIRChanged(WiiRemote* wiiRemote, TArray<FWiiRemoteDot> dots)
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
		WiiRemoteDelegate->OnIRChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].Dots);
	}
}

void UWiiRemoteManager::OnNunchukConnected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d Nunchuk has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectNunchuk = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnNunchukConnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnNunchukButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteNunchukButtons buttons)
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
		WiiRemoteDelegate->OnNunchukButtonsChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].NunchukButtons);
	}
}

void UWiiRemoteManager::OnNunchukAccelChanged(WiiRemote* wiiRemote, FVector accel)
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
		WiiRemoteDelegate->OnNunchukAccelChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].NunchukAcceleration);
	}
}

void UWiiRemoteManager::OnNunchukOrientationChanged(WiiRemote* wiiRemote, float pitch, float roll)
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
		WiiRemoteDelegate->OnNunchukOrientationChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].NunchukOrientationPitch, Data[index].NunchukOrientationRoll);
	}
}

void UWiiRemoteManager::OnNunchukJoystickChanged(WiiRemote* wiiRemote, float x, float y)
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
		WiiRemoteDelegate->OnNunchukJoystickChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].NunchukJoystickX, Data[index].NunchukJoystickY);
	}
}

void UWiiRemoteManager::OnClassicConnected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d ClassicController has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectClassicController = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnClassicConnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnClassicButtonsChanged(WiiRemote* wiiRemote, FWiiRemoteClassicControllerButtons buttons)
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
		WiiRemoteDelegate->OnClassicButtonsChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].ClassicControllerButtons);
	}
}

void UWiiRemoteManager::OnClassicJoystickLChanged(WiiRemote* wiiRemote, float x, float y)
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
		WiiRemoteDelegate->OnClassicJoystickLChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].ClassicControllerLeftJoystickX, Data[index].ClassicControllerLeftJoystickY);
	}
}

void UWiiRemoteManager::OnClassicJoystickRChanged(WiiRemote* wiiRemote, float x, float y)
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
		WiiRemoteDelegate->OnClassicJoystickRChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].ClassicControllerRightJoystickX, Data[index].ClassicControllerRightJoystickY);
	}
}

void UWiiRemoteManager::OnClassicTriggersChanged(WiiRemote* wiiRemote, float left, float right)
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
		WiiRemoteDelegate->OnClassicTriggersChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].ClassicControllerLeftTrigger, Data[index].ClassicControllerRightTrigger);
	}
}

void UWiiRemoteManager::OnBalanceConnected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d BalanceBoard has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectBalanceBoard = true;
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnBalanceConnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnBalanceWeightChanged(WiiRemote* wiiRemote, FWiiRemoteBalanceBoard balanceBoard)
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
		WiiRemoteDelegate->OnBalanceWeightChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].BalanceBoard);
	}
}

void UWiiRemoteManager::OnMotionPlusDetected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has connected."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusDetectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnMotionPlusEnabled(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus is enabled."), IdentifyWiiRemote(wiiRemote->UniqueID));
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusEnabledFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnMotionPlusSpeedChanged(WiiRemote* wiiRemote, FRotator speed)
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
		WiiRemoteDelegate->OnMotionPlusSpeedChangedFunction(IdentifyWiiRemote(wiiRemote->UniqueID), Data[index].MotionPlusSpeed);
	}
}

void UWiiRemoteManager::OnMotionPlusExtensionConnected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has connected extension."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusExtensionConnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnMotionPlusExtensionDisconnected(WiiRemote* wiiRemote)
{
	auto index = WiiRemoteIndexForWiiRemote(wiiRemote->UniqueID);
	if (index == -1)
		return;
	UE_LOG(WiiRemotePluginLog, Log, TEXT("WiiRemote %d MotionPlus has disconnected extension."), IdentifyWiiRemote(wiiRemote->UniqueID));
	Data[index].IsConnectMotionPlus = true;
	wiiRemote->EnableMotionPlus();
	if (WiiRemoteDelegate)
		WiiRemoteDelegate->OnMotionPlusExtensionDisconnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void UWiiRemoteManager::OnExtensionDisconnected(WiiRemote* wiiRemote)
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
		WiiRemoteDelegate->OnExtensionDisconnectedFunction(IdentifyWiiRemote(wiiRemote->UniqueID));
}

void OnStateChanged(WiiRemote& wiiRemote, StateChangeFlags changed, const WiiRemoteState& newState)
{
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::Connected))
	{
		if (newState.ExtensionType != ExtensionType::BalanceBoard)
			wiiRemote.SetReportType(InputReport::ButtonsAccelerationIRExtension);
		else
			wiiRemote.SetReportType(InputReport::ButtonsBalanceBoard);
	}
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ChangedAll))
		wiiRemote.RefreshState();
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusDetected))
		wiiRemote.EnableMotionPlus();
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionConnected))
	{
		if (wiiRemote.MotionPlusEnabled())
			wiiRemote.EnableMotionPlus();
	}
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionDisconnected))
	{
		if (wiiRemote.MotionPlusConnected())
			wiiRemote.EnableMotionPlus();
	}

	auto index = manager->WiiRemoteIndexForWiiRemote(wiiRemote.UniqueID);
	if (index == -1)
		return;
	
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::Connected))
		manager->ChangeFlags[index].IsConnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ConnectionLost))
		manager->ChangeFlags[index].IsConnectionLost = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::BatteryChanged))
		manager->ChangeFlags[index].IsBatteryChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::BatteryDrained))
		manager->ChangeFlags[index].IsBatteryDrained = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::LEDsChanged))
		manager->ChangeFlags[index].IsLEDChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::NunchukConnected))
		manager->ChangeFlags[index].IsNunchukConnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ClassicConnected))
		manager->ChangeFlags[index].IsClassicConnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::BalanceConnected))
		manager->ChangeFlags[index].IsBalanceConnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusDetected))
		manager->ChangeFlags[index].IsMotionPlusDetected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusEnabled))
		manager->ChangeFlags[index].IsMotionPlusEnabled = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionConnected))
		manager->ChangeFlags[index].IsMotionPlusExtensionConnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusExtensionDisconnected))
		manager->ChangeFlags[index].IsMotionPlusExtensionDisconnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ExtensionDisconnected))
		manager->ChangeFlags[index].IsExtensionDisconnected = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ButtonsChanged))
		manager->ChangeFlags[index].IsButtonsChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::AccelerationChanged))
		manager->ChangeFlags[index].IsAccelChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::OrientationChanged))
		manager->ChangeFlags[index].IsOrientationChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::IRChanged))
		manager->ChangeFlags[index].IsIRChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::NunchukButtonsChanged))
		manager->ChangeFlags[index].IsNunchukButtonsChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::NunchukAccelerationChanged))
		manager->ChangeFlags[index].IsNunchukAccelChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::NunchukOrientationChanged))
		manager->ChangeFlags[index].IsNunchukOrientationChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::NunchukJoystickChanged))
		manager->ChangeFlags[index].IsNunchukJoystickChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ClassicButtonsChanged))
		manager->ChangeFlags[index].IsClassicButtonsChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ClassicJoystickLChanged))
		manager->ChangeFlags[index].IsClassicJoystickLChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ClassicJoystickRChanged))
		manager->ChangeFlags[index].IsClassicJoystickRChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::ClassicTriggersChanged))
		manager->ChangeFlags[index].IsClassicTriggerChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::BalanceWeightChanged))
		manager->ChangeFlags[index].IsBalanceWeightChanged = true;
	if (static_cast<int>(changed) & static_cast<int>(StateChangeFlags::MotionPlusSpeedChanged))
		manager->ChangeFlags[index].IsMotionPlusSpeedChanged = true;
}

void UWiiRemoteManager::SetWiiRemoteButtons(WiiRemote& wiiRemote, FWiiRemoteButtons& buttons)
{
	buttons.IsPushA = wiiRemote.Buttons.A();
	buttons.IsPushB = wiiRemote.Buttons.B();
	buttons.IsPushUp = wiiRemote.Buttons.Up();
	buttons.IsPushDown = wiiRemote.Buttons.Down();
	buttons.IsPushRight = wiiRemote.Buttons.Right();
	buttons.IsPushLeft = wiiRemote.Buttons.Left();
	buttons.IsPushOne = wiiRemote.Buttons.One();
	buttons.IsPushTwo = wiiRemote.Buttons.Two();
	buttons.IsPushPlus = wiiRemote.Buttons.Plus();
	buttons.IsPushMinus = wiiRemote.Buttons.Minus();
	buttons.IsPushHome = wiiRemote.Buttons.Home();
}

void UWiiRemoteManager::SetWiiRemoteDots(WiiRemote& wiiRemote, TArray<FWiiRemoteDot>& dots)
{
	dots.Init(FWiiRemoteDot(), 4);
	for (auto index = 0; index < dots.Num(); index++)
	{
		dots[index].bVisible = wiiRemote.IR.Dot[index].Visible;
		dots[index].X = wiiRemote.IR.Dot[index].Data.X;
		dots[index].Y = wiiRemote.IR.Dot[index].Data.Y;
	}
}

void UWiiRemoteManager::SetWiiRemoteNunchukButtons(WiiRemote& wiiRemote, FWiiRemoteNunchukButtons& buttons)
{
	buttons.IsPushC = wiiRemote.Nunchuk.C;
	buttons.IsPushZ = wiiRemote.Nunchuk.Z;
}

void UWiiRemoteManager::SetWiiRemoteClassicControllerButtons(WiiRemote& wiiRemote, FWiiRemoteClassicControllerButtons& buttons)
{
	buttons.IsPushA = wiiRemote.ClassicController.Buttons.A();
	buttons.IsPushB = wiiRemote.ClassicController.Buttons.B();
	buttons.IsPushX = wiiRemote.ClassicController.Buttons.X();
	buttons.IsPushY = wiiRemote.ClassicController.Buttons.Y();
	buttons.IsPushUp = wiiRemote.ClassicController.Buttons.Up();
	buttons.IsPushDown = wiiRemote.ClassicController.Buttons.Down();
	buttons.IsPushRight = wiiRemote.ClassicController.Buttons.Right();
	buttons.IsPushLeft = wiiRemote.ClassicController.Buttons.Left();
	buttons.IsPushPlus = wiiRemote.ClassicController.Buttons.Plus();
	buttons.IsPushMinus = wiiRemote.ClassicController.Buttons.Minus();
	buttons.IsPushHome = wiiRemote.ClassicController.Buttons.Home();
	buttons.IsPushZL = wiiRemote.ClassicController.Buttons.ZL();
	buttons.IsPushZR = wiiRemote.ClassicController.Buttons.ZR();
	buttons.IsPushTriggerL = wiiRemote.ClassicController.Buttons.TriggerL();
	buttons.IsPushTriggerR = wiiRemote.ClassicController.Buttons.TriggerR();
}

void UWiiRemoteManager::SetWiiRemoteBalanceBoard(WiiRemote& wiiRemote, FWiiRemoteBalanceBoard& balanceBoard)
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
