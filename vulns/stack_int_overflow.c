/*

Original author: draktuls 
License: GPLv3
Code description:
	This source implements integer overflow handler

*/

#include "stack_int_overflow.h"

long stack_int_overflow_handler(avud_request * request){
	
	long ret = 0;
	const uint8_t data_size = sizeof(uint64_t);
	
	uint64_t data[STACK_INT_OVERFLOW_SIZE] = {0};
	
	if(request->input == NULL){
		pr_err("[AVUD] (stack_integer_overflow) stack_int_overflow_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}

	if(request->input_len == 0){
		pr_err("[AVUD] (stack_integer_overflow) stack_int_overflow_handler: input length is zero!");
		ret = -EINVAL;
		goto exit;
	}
	
	// VULN HERE
	// This following line acquires the maximum length of the request
	// by multiplying the uint64_t size with the input_len
	// Thanks to the uint8 size it will overflow at 32 input length
	// and pass the size check 
	uint8_t max_length = request->input_len * data_size;

	if(max_length > sizeof(data)){
		pr_err("[AVUD] (stack_integer_overflow) stack_int_overflow_handler: input length is too great!");
		ret = -EINVAL;
		goto exit;
	}
	
	uint64_t i = 0;
	uint64_t cur;
	uint64_t temp;
	while(true){
		
		cur = i * data_size;
		
		if(cur > max_length){
			break;
		}
		
		if(copy_from_user((void *)&temp,(void *)request->input + cur, data_size)){
			pr_err("[AVUD] (stack_integer_overflow) stack_int_overflow_handler: cannot copy input buffer!");
			ret = -EINVAL;
			goto exit;
		}
		
		if(temp == 0xdeadcafebabe1337){
			break;
		}
		
		data[i] = temp;
		
		i++;
		
	}
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(stack_int_overflow_handler);
