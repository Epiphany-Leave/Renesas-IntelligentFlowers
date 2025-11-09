#ifndef __DEV_DHT11_H
#define __DEV_DHT11_H

typedef struct DHT11Dev{
    unsigned int tempture;
    unsigned int humidity;
    int (*Init)(struct DHT11Dev *ptdev);
    int (*Read)(struct DHT11Dev *ptdev);
}DHT11Device;

struct DHT11Dev *DHT11GetDevice(void);

#endif /* __DEV_DHT11_H */
