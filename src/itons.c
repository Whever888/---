#include"font.h"
#include"jpg.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<time.h>
#include<linux/input.h>
#include<sys/mman.h>

// struct LcdDevice *init_lcd(const char *device)
// {
// 	//申请空间
// 	struct LcdDevice* lcd = malloc(sizeof(struct LcdDevice));
// 	if(lcd == NULL)
// 	{
// 		return NULL;
// 	} 

// 	//1打开设备
// 	lcd->fd = open(device, O_RDWR);
// 	if(lcd->fd < 0)
// 	{
// 		perror("open lcd fail");
// 		free(lcd);
// 		return NULL;
// 	}
	
// 	//映射
// 	lcd->mp = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd->fd,0);

// 	return lcd;
// }

