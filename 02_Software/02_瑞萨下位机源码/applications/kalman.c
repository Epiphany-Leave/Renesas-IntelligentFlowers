#include "kalman.h"
#include "app.h"
#include "drv_adc.h"
#include <devices.h>
#include <drivers.h>
#include <errno.h>
#include <libs.h>
#include "hal_data.h"
#include <stdio.h>

/* 定义常量 */
#define TARGET_SOIL_MOISTURE 60.0   // 目标土壤湿度
#define NUM_SENSORS 3               // 传感器数据个数

/* 全局变量 */
double measurements[NUM_SENSORS];  // 存放各传感器数据
double estimates[1];               // 卡尔曼滤波后的估计结果
double processNoise = 0.1;         // 卡尔曼滤波过程噪声
double measurementNoise = 0.2;     // 卡尔曼滤波测量噪声
double weights[NUM_SENSORS] = {0.4, 0.3, 0.3};  // 权重值

/* 数据预处理与融合函数 */
double fuseSensorData(double *data, double *weights, int size);
void kalmanFilter(double *measurements, double *estimates, int numMeasurements, double processNoise, double measurementNoise);
void Control_WaterPump(void);  // 控制策略函数

/* 定时器中断回调函数，每1ms执行一次 */
void gpt_timer2_callback(timer_callback_args_t *p_args){
    if(p_args->event == TIMER_EVENT_CYCLE_END)
    {
        // 模拟采集传感器数据
        double soilMoistureData[NUM_SENSORS] = {55.0, 60.0, 58.0};
        double airTempData[NUM_SENSORS] = {25.0, 24.5, 25.5};
        double airHumidityData[NUM_SENSORS] = {65.0, 63.0, 66.0};

        // 数据融合
        measurements[0] = fuseSensorData(soilMoistureData, weights, NUM_SENSORS);
        measurements[1] = fuseSensorData(airTempData, weights, NUM_SENSORS);
        measurements[2] = fuseSensorData(airHumidityData, weights, NUM_SENSORS);

        // 卡尔曼滤波处理
        kalmanFilter(measurements, estimates, NUM_SENSORS, processNoise, measurementNoise);

        // 控制策略
        Control_WaterPump();
    }
}

/* 数据融合：加权平均 */
double fuseSensorData(double *data, double *weights, int size) {
    double weightedSum = 0.0;
    double totalWeight = 0.0;
    for (int i = 0; i < size; i++) {
        weightedSum += data[i] * weights[i];
        totalWeight += weights[i];
    }
    return weightedSum / totalWeight;
}

/* 卡尔曼滤波函数 */
void kalmanFilter(double *measurements, double *estimates, int numMeasurements, double processNoise, double measurementNoise) {
    double estimate = 0.0;
    double priorEstimate = 0.0;
    double priorError = 1.0;
    double errorCovariance = priorError * priorError;
    double kalmanGain;

    for (int i = 0; i < numMeasurements; i++) {
        // 卡尔曼增益计算
        kalmanGain = errorCovariance / (errorCovariance + measurementNoise);
        // 更新状态估计
        estimate = priorEstimate + kalmanGain * (measurements[i] - priorEstimate);
        // 更新协方差
        errorCovariance = (1 - kalmanGain) * errorCovariance;
        // 更新先验估计
        priorEstimate = estimate;
    }
    estimates[0] = estimate;
}

/* 控制水泵逻辑 */
void Control_WaterPump(void) {
    static double previousEstimate = 0.0;  // 上次的估计结果
    double threshold = 2.0;  // 阈值，避免频繁开关
    if (estimates[0] < TARGET_SOIL_MOISTURE - threshold) {
        // 打开水泵
        g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11, BSP_IO_LEVEL_HIGH);
    } else if (estimates[0] > TARGET_SOIL_MOISTURE + threshold) {
        // 关闭水泵
        g_ioport.p_api->pinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_11, BSP_IO_LEVEL_LOW);
    }
    previousEstimate = estimates[0];
}
