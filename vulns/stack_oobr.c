/*

Original author: draktuls 
License: GPLv3
Code description:
	This source defines basic oobr on the stack

*/

#include "stack_oobr.h"

long stack_oobr_handler(avud_request * request){
	
	long ret = 0;
	unsigned char data[STACK_VAR_SIZE] = {0};
	
	memset(data,'A',STACK_VAR_SIZE);
	
	if(request->output == NULL){
		pr_err("[AVUD] (stack_oobr) stack_oobr_handler: output is NULL!");
		ret = -EINVAL;
		goto exit;
	}
	
	if(request->output_len == 0){
		pr_err("[AVUD] (stack_oobr) stack_oobr_handler: output length is 0");
		ret = -EINVAL;
		goto exit;
	}

	size_t size = request->output_len;

	unsigned char * temp = kzalloc(size, GFP_KERNEL);
	if (!temp)
    {
        pr_err("[AVUD] (stack_oobr) stack_oobr_handler: couldn't allocate temp memory!");
        ret = -ENOMEM;
		goto exit;
    }

	// VULN HERE
	// we are passing raw output length as length parameter
	// this will cause to leak more data than the buffer holds
	// resulting in information leakage
	memcpy(temp, data, size);
	
	// Compile time workaround..
	// We need to memcpy the buffer first and then copy to user
	if(copy_to_user((void *)request->output,(void *)temp, size)){
		pr_err("[AVUD] (stack_oobr) stack_oobr_handler: cannot copy to output buffer!");
		ret = -EINVAL;
		goto exit;
	}
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(stack_oobr_handler);
