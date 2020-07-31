#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <poll.h>

int main(int argc, char **argv)
{
    int fd;
	int ret = 0;
    unsigned char buf[4];
    fd = open("/dev/hello", O_RDWR);
    if (fd < 0)
    {
        printf("open is error!\n");
        return -1;
    }

    ret = read(fd, &buf, 4);
    printf("%x\n", buf[0]);
    printf("%x\n", buf[1]);
    printf("%x\n", buf[2]);
    printf("%x\n", buf[3]);
    close(fd);
}
