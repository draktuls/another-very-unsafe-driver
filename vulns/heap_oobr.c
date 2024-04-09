/*

Original author: draktuls 
License: GPLv3
Code description:
	This source implements basic out of bounds read from the heap

*/

#include "heap_oobr.h"

long heap_oobr_handler(avud_request * request){
	
	long ret = 0;

	if(request->output == NULL){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: output is NULL!");
		ret = -EINVAL;
		goto exit;
	}

	if(request->input == NULL){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}

	if(request->input_len == 0 || request->input_len > HEAP_OOBR_STRUCT_SIZE){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: input length is 0 or bigger than max");
		ret = -EINVAL;
		goto exit;
	}

	size_t chunk_size = sizeof(heap_struct_oobr);
	
	heap_struct_oobr * chunk = kzalloc(chunk_size, GFP_KERNEL);
	if (!chunk)
    {
        pr_err("[AVUD] (heap_oobr) heap_oobr_handler: couldn't allocate memory!\n");
        ret = -ENOMEM;
		goto exit;
    }

	// We are setting our data_size to our chunk size
	chunk->data_size = chunk_size;

	// VULN HERE
	// this will overflow into our structure by 8 or 4 bytes
	// since the next parameter is controlling the size 
	// This can corrupt the later code execution which uses this size and may lead to oobr or oobw
	// In this case we can overread the buffer and our structure -> OOBR
	if(copy_from_user((void *)chunk->data,(void *)request->input, chunk->data_size)){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: cannot copy input into chunk");
		ret = -EINVAL;
		goto free_exit;
	}

	if(chunk->data_size > OOBR_MAX_READ){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: data size is too large");
		ret = -EINVAL;
		goto free_exit;
	}
	
	// Copy back to user output
	if(copy_to_user((void *)request->output,(void *)chunk->data, chunk->data_size)){
		pr_err("[AVUD] (heap_oobr) heap_oobr_handler: cannot copy to output buffer!");
		ret = -EINVAL;
		goto free_exit;
	}
	
	free_exit:

	kfree(chunk);

	exit:
	
	return ret;
}

EXPORT_SYMBOL(heap_oobr_handler);
