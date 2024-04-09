/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by heap note for important variables and defines.

*/

#pragma once

#ifndef __HEAP_NOTE_H__
#define __HEAP_NOTE_H__

#include "../defines.h"

#define CTL_ADD 0
#define CTL_EDIT 1
#define CTL_FREE 2
#define CTL_VIEW 3
#define CTL_EDIT_TAG 4
#define CTL_VIEW_TAG 5
#define CTL_SWITCH_SAFE 6

#define SAFE 0x13376969
#define UNSAFE 0xdeadbeef

extern void * heap_notes[];

extern long heap_note_handler(size_t cmd, avud_request * request);

#endif