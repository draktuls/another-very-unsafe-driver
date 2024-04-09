/*

Original author: draktuls 
License: GPLv3
Code description:
	This source defines stack buffer underflow

*/

#include "stack_buf.h"

long stack_buf_handler(avud_request * request){
	
	long ret = 0;
	int index;
	
	unsigned char data[STACK_UNDERFLOW_SIZE] = {0};
	
	if(request->input == NULL){
		pr_err("[AVUD] (stack_buf) stack_buf_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}
	
	if(request->input_len > sizeof(data)){
		pr_err("[AVUD] (stack_buf) stack_buf_handler: input length is too great!");
		ret = -EINVAL;
		goto exit;
	}
	
	// VULN HERE
	// We are checking if input offset is too great
	// this however doesn't include the lower end of the spectrum
	if(request->input_offset > sizeof(data) - 1){
		pr_err("[AVUD] (stack_buf) stack_buf_handler: input offset is too great!");
		ret = -EINVAL;
		goto exit;
	}
	
	index = request->input_offset;
	unsigned char * offset_data = &data[index];
	
	if(copy_from_user((void *)offset_data,(void *)request->input, request->input_len)){
		pr_err("[AVUD] (stack_buf) stack_buf_handler: cannot copy input buffer!");
		ret = -EINVAL;
		goto exit;
	}
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(stack_buf_handler);
