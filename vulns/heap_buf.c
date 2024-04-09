/*

Original author: draktuls 
License: GPLv3
Code description:
	This source implements buffer underflow on the heap

*/

#include "heap_buf.h"

long copy_input(heap_struct_buf * chunk, char * buf, size_t size){

    long ret = 0;

    char * user_data = kzalloc(size, GFP_KERNEL);
	if (!user_data)
    {
        pr_err("[AVUD] (heap_buf) copy_input: couldn't allocate memory!\n");
        ret = -ENOMEM;
		goto exit;
    }

    if(copy_from_user((void *)user_data,(void *)buf, size)){
		pr_err("[AVUD] (heap_buf) copy_input: cannot copy input buffer!");
		ret = -EINVAL;
		goto free_exit;
	}

    // VULN HERE
    // During the data processing we are checking if the character is a tab \t
    // in case it is we will just increase the counter and ignore this character
    
    // The problem here is that if we go over 127 bytes we will overflow into -128
    // this is then used as the index which results into buffer underflow
    // index used is in both userland and kernel
    int8_t i = 0;
    while(true){
        if(user_data[i] != '\t'){
            if(user_data[i] == '\n'){
                break;
            }

        chunk->data[i] = user_data[i];
        
        }

        i++;

    }

    free_exit:

    kfree(user_data);

    exit:

    return ret;

}

long heap_buf_handler(avud_request * request){
	
	long ret = 0;
	
	if(request->input == NULL){
		pr_err("[AVUD] (heap_buf) heap_buf_handler: input is NULL!");
		ret = -EINVAL;
		goto exit;
	}
	
    size_t chunk_size = sizeof(heap_struct_buf);

	if(request->input_len > chunk_size){
		pr_err("[AVUD] (heap_buf) heap_buf_handler: input length is too great!");
		ret = -EINVAL;
		goto exit;
	}

    heap_struct_buf * chunk = kzalloc(chunk_size, GFP_KERNEL);
	if (!chunk)
    {
        pr_err("[AVUD] (heap_buf) heap_buf_handler: couldn't allocate memory!\n");
        ret = -ENOMEM;
		goto exit;
    }

    ret = copy_input(chunk, request->input, request->input_len);

    kfree(chunk);
	
	exit:
	
	return ret;
}

EXPORT_SYMBOL(heap_buf_handler);
