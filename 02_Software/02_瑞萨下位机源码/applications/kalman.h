#ifndef __KALMAN_H
#define __KALMAN_H

#include "drv_gpt.h"


/* 数据加权平均 */
double weightedAverage(double *data, double *weights, int size);
/* 贝叶斯融合函数 */
double bayesianFusion(double *probabilities, int size);
/* 卡尔曼滤波函数 */
void kalmanFilter(double *measurements, double *estimates, int numMeasurements, double processNoise, double measurementNoise);
/* 控制水泵逻辑 */
void Control_WaterPump(void);


#endif
