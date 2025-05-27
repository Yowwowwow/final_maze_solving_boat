#ifndef MPU_WRAPPER_H
#define MPU_WRAPPER_H

#include "Arduino.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

class MPUWrapper {
public:
    MPUWrapper(uint8_t interruptPin = 2, uint8_t ledPin = 13);

    bool begin();                 // 初始化裝置，回傳是否成功
    bool read_mpu(float ypr[3]);  // 讀取 ypr 值，成功回傳 true，否則 false

private:
    MPU6050 mpu;
    bool dmpReady = false;
    uint8_t mpuIntStatus;
    uint8_t devStatus;
    uint16_t packetSize;
    uint8_t fifoBuffer[64];
    uint8_t interruptPin;
    uint8_t ledPin;
    bool blinkState = false;

    Quaternion q;
    VectorFloat gravity;
    float internal_ypr[3];

    static volatile bool mpuInterrupt;
    static void dmpDataReady();
};

#endif
