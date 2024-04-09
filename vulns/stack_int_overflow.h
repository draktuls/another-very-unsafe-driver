/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by integer overflow handler

*/

#pragma once

#ifndef __STACK_INT_OVERFLOW_H__
#define __STACK_INT_OVERFLOW_H__

#include "../defines.h"

#define STACK_INT_OVERFLOW_SIZE 16

extern long stack_int_overflow_handler(avud_request * request);

#endif