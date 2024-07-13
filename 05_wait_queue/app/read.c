#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])  
{
    int fd;  //定义int类型的文件描述符
    char buf1[32] = {0};   //定义读取缓冲区buf
    char buf2[32] = {0};  //定义读取缓冲区buf
    fd = open("/dev/test",O_RDWR| O_NONBLOCK);  //打开led驱动
    if (fd < 0)
    {
        perror("open error \n");
        return fd;
    }
printf("read before \n");
    while (1)
    {
         
         read(fd,buf1,sizeof(buf1));  //从/dev/test文件读取数据
         printf("buf is %s \n",buf1);  //打印读取的数据
         sleep(1);
        
    }
    printf("read after\n");

    close(fd);     //关闭文件
    return 0;
}