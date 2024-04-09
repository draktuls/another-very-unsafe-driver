/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by oobr stack handler

*/

#pragma once

#ifndef __STACK_OOBR_H__
#define __STACK_OOBR_H__

#include "../defines.h"

#define STACK_VAR_SIZE 256

extern long stack_oobr_handler(avud_request * request);

#endif