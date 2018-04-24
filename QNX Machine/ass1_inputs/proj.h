/*
 * proj.h
 *
 *  Created on: Oct 31, 2016
 *      Author: king
 */

#ifndef PROJ_H_
#define PROJ_H_

#define NUM_STATES 8
typedef enum {
	START_STATE = 0,
	READY_STATE = 1,
	LEFT_OPEN_STATE = 2,
	LEFT_CLOSED_STATE = 3,
	RIGHT_OPEN_STATE = 4,
	EXIT_STATE = 6,
	STOP_STATE = 7
} State;

#define NUM_INPUTS 12
typedef enum {
	LEFT_SCAN = 0,
	GUARD_LEFT_UNLOCK = 1,
	LEFT_OPEN = 2,
	WEIGHT_SCALE = 3,
	LEFT_CLOSED = 4,
	GAURD_LEFT_LOCK = 5,
	GAURD_RIGHT_UNLOCK = 6,
	RIGHT_OPEN = 7,
	RIGHT_CLOSED = 8,
	GAURD_RIGHT_LOCK = 9,
	STOP_BUTTON = 10,
	RIGHT_SCAN = 11
} Input;

#define NUM_OUTPUTS 13
typedef enum {
	START_MSG = 0,
	READY_MSG = 1,
	LEFT_SCAN_COMPLETE = 2,
	LEFT_DOOR_UNLOCKED = 3,
	LEFT_DOOR_OPEN = 4,
	WEIGHT_SCANED = 5,
	LEFT_DOOR_CLOSED = 6,
	LEFT_DOOR_LOCKED = 7,
	RIGHT_DOOR_UNLOCKED = 8,
	RIGHT_DOOR_OPEN = 9,
	RIGHT_DOOR_CLOSED = 10,
	RIGHT_DOOR_LOCKED = 11,
	RIGHT_SCAN_COMPLETE = 12
} Output;

const char *outMessage[NUM_OUTPUTS] = { "[status update: initial startup]",
		"[status update: system ready]", "[status update: Left Scan Accepted]",
		"[status update: Left door unlocked]",
		"[status update: Left door open]", "[status update: Weight Scaled]",
		"[status update: Left door closed]",
		"[status update: Left door locked]",
		"[status update: right door unlocked]",
		"[status update: Right door open]",
		"[status update: Right door closed]",
		"[status update: Right door locked]",
		"[status update: Right Scan Accepted]" };

const char *inMessage[NUM_INPUTS] = { "LS", "GLU", "LO", "WS", "LC", "GLL",
		"GRU", "RO", "RC", "GRL", "STOP", "RS" };
#endif /* PROJ_H_ */
