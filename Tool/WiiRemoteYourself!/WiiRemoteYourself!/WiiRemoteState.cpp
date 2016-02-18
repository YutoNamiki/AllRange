#include "WiiRemoteYourselfPCH.h"
#include "WiiRemoteState.h"
#include <string>

void WiiRemoteState::Clear(const bool includingDeadzones)
{
	Vector2D<float> nunchukDeadzone, classicJoyLDeadzone, classicJoyRDeadzone;
	if (!includingDeadzones)
	{
		nunchukDeadzone = Nunchuk.Joystick.DeadZone;
		classicJoyLDeadzone = ClassicController.JoystickL.DeadZone;
		classicJoyRDeadzone = ClassicController.JoystickR.DeadZone;
	}
	memset(this, 0, sizeof(WiiRemoteState));
	if (!includingDeadzones)
	{
		Nunchuk.Joystick.DeadZone = nunchukDeadzone;
		ClassicController.JoystickL.DeadZone = classicJoyLDeadzone;
		ClassicController.JoystickR.DeadZone = classicJoyRDeadzone;
	}
}
