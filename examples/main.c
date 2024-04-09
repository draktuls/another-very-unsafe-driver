
#include "comms.h"

int open_avud(){
	avud_fd = open("/dev/avud", O_RDWR);
	if (avud_fd < 0){
		printf("[!] Failed to open AVUD: 0x%lx!\n",avud_fd);
		exit(-1);
	} else {
        puts("[*] Opened kryptor AVUD!");
    }
}

avud_request * alloc_request(){
	avud_request * t = malloc(sizeof(avud_request));
	memset(t,0,sizeof(avud_request));
	if(t == NULL){
		puts("[!] OOM, exiting");
		exit(-1);
	}
	return t;
}

void test_drive(){
	
	int out;

	if(avud_fd == -1){
		exit(-1);
	}
	avud_request * req = alloc_request();
	
	// Make invalid request as a test -> invalid
	out = avud_ioctl(req, 0x1337beef);
	printf("[*] Invalid request: 0x%lx -> 0x%lx -> %s\n",out, errno, strerror(errno));
	assert(errno == 0x19); // Inappropriate ioctl for device

	puts("[+] We are good to go!");
}

int main(){
	
	int out;

	// Open device (stored in global fd)
	open_avud();

	// Test our driver to be valid
	test_drive();
	
}
