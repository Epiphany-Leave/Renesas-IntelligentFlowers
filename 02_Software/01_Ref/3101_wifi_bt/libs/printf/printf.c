#include <devices.h>
#include <hal_data.h>
#include <stdio.h>

/* 重定向 printf 输出 */
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _write(int fd, char *pBuffer, int size)
{
    (void)fd;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
    pLogDevice->Write(pLogDevice, (unsigned char*)&ch, 1);
    return size;
}


int _read (int fd, char *pBuffer, int size)
{
    (void)fd;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
	/* 启动接收字符 */
    while(pLogDevice->Read(pLogDevice, (unsigned char*)&pBuffer, size) != size)
    {}
    return size;
}
#else
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION < 6010050)
struct __FILE{
   int handle;
};
#endif
FILE __stdout;
int fputc(int ch, FILE *f)
{
    (void)f;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
    pLogDevice->Write(pLogDevice, (unsigned char*)&ch, 1);
    return ch;
}

/* 重写这个函数,重定向scanf */
int fgetc(FILE *f)
{
	uint8_t ch;
	
	(void)f;
    struct UartDev *pLogDevice = UartDeviceFind("Log");
	/* 启动接收字符 */
    while(pLogDevice->Read(pLogDevice, (unsigned char*)&ch, 1) != 1)
    {}
	/* 回显 */
	{
		fputc(ch, &__stdout);
		
		/* 回车之后加换行 */
		if (ch == '\r')
		{
			fputc('\n', &__stdout);
		}
	}
    
	return (int)ch;
}
#endif