#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define CMD_TEST0 _IOW('L',0,int)
struct args{//定义要传递的结构体
	int a;
int b;
	int c;
};
int main(int argc,char *argv[]){
	int fd;//定义int类型文件描述符
	struct args test;//定义args类型的结构体变量test
	test.a = 1;
	test.b = 2;
	test.c = 3;
	fd = open("/dev/test",O_RDWR,0777);//打开/dev/test设备
	if(fd < 0){
		printf("file open error \n");
	}
	ioctl(fd,CMD_TEST0,&test);//使用ioctl函数传递结构体变量test地址
	close(fd);
}
