/*

Original author: draktuls 
License: GPLv3
Code description:
	This header is used by stack buffer underflow handler

*/

#pragma once

#ifndef __STACK_BUF_H__
#define __STACK_BUF_H__

#include "../defines.h"

#define STACK_UNDERFLOW_SIZE 256

extern long stack_buf_handler(avud_request * request);

#endif