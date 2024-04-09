/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used for heap bof handler

*/

#pragma once

#include "../defines.h"

#define BOF_MAX_BUFFER_SIZE 2048

extern long heap_bof_handler(avud_request * request);
