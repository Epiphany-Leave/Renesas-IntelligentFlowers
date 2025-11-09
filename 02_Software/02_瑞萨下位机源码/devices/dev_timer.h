#ifndef __DEV_TIMER_H
#define __DEV_TIMER_H

#define TIMER_DEVICE_NAME_MAX_LENGTH        (16)

typedef struct TimerDev{
    char *name;
    unsigned char channel;
    unsigned char status;
    int (*Init)(struct TimerDev *ptdev);
    int (*Start)(struct TimerDev *ptdev);
    int (*Stop)(struct TimerDev *ptdev);
    int (*Read)(struct TimerDev *ptdev, unsigned char *buf, unsigned int length);
    int (*Timeout)(struct TimerDev *ptdev, unsigned int timeout);
    struct TimerDev *next;
}TimerDevice;

void TimerDevicesRegister(void);
void TimerDeviceInsert(struct TimerDev *ptdev);
struct TimerDev *TimerDeviceFind(const char *name);
void TimerDeviceList(void);

#endif /* __DEV_TIMER_H */
