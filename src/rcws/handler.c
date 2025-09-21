
#include "handler.h"
#include <stdio.h>
#include <stdbool.h>

#define CONSTRAINT_REL_AZIMUTH_ANGLE_UPPER				(80.0f)
#define CONSTRAINT_REL_AZIMUTH_ANGLE_LOWER				(-80.0f)

#define CONSTRAINT_ABS_ELEVATION_ANGLE_UPPER			(85.0f)
#define CONSTRAINT_ABS_ELEVATION_ANGLE_LOWER			(30.0f)

static bool azimuth_is_valid(float setpoint)
{
	if ((setpoint >= CONSTRAINT_REL_AZIMUTH_ANGLE_LOWER) &&
		(setpoint <= CONSTRAINT_REL_AZIMUTH_ANGLE_UPPER))
		return true;

	return false;
}

static bool elevation_is_valid(float setpoint)
{
	if ((setpoint >= CONSTRAINT_ABS_ELEVATION_ANGLE_LOWER) &&
		(setpoint <= CONSTRAINT_ABS_ELEVATION_ANGLE_UPPER))
		return true;

	return false;
}



int handler_barrel_set_azimuth(float setpoint)
{
	int ret = -1;

	printf("Set azimuth to: %f\n", setpoint);
	if (!azimuth_is_valid(setpoint)) {
		printf("set point is not within allowed value!\n");
		goto err;
	}

err:
	return ret;
}


int handler_barrel_set_elevation(float setpoint)
{
	int ret = -1;

	printf("Set elevation to: %f\n", setpoint);
	if (!elevation_is_valid(setpoint)) {
		printf("set point is not within allowed value!\n");
		goto err;
	}

err:
	return ret;
}


int handler_barrel_manual_ctrl_azimuth(int direction)
{
    printf("Manual control azimuth direction: %d\n", direction);
	return 0;
}

int handler_barrel_manual_ctrl_elevation(int direction)
{
    printf("Manual control elevation direction: %d\n", direction);
	return 1;
}