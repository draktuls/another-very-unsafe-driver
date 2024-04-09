/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by basic bof handler

*/

#pragma once

#ifndef __STACK_BOF_H__
#define __STACK_BOF_H__

#include "../defines.h"

#define STACK_OVERFLOW_SIZE 256

extern long stack_bof_handler(avud_request * request);

#endif