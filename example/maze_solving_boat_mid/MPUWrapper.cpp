#include "MPUWrapper.h"

volatile bool MPUWrapper::mpuInterrupt = false;

MPUWrapper::MPUWrapper(uint8_t interruptPin, uint8_t ledPin)
    : interruptPin(interruptPin), ledPin(ledPin) {}

void MPUWrapper::dmpDataReady() {
    mpuInterrupt = true;
}

bool MPUWrapper::begin() {
    Wire.begin();
    Wire.setClock(400000);

    Serial.begin(9600);
    while (!Serial);

    mpu.initialize();
    pinMode(interruptPin, INPUT);

    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        return false;
    }

    devStatus = mpu.dmpInitialize();

    // 預設的 offset，可依據實際調整
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus == 0) {
        mpu.CalibrateAccel(6);
        mpu.CalibrateGyro(6);
        mpu.PrintActiveOffsets();

        mpu.setDMPEnabled(true);
        attachInterrupt(digitalPinToInterrupt(interruptPin), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();
        dmpReady = true;
        packetSize = mpu.dmpGetFIFOPacketSize();
        return true;
    } else {
        Serial.print("DMP Initialization failed (code ");
        Serial.print(devStatus);
        Serial.println(")");
        return false;
    }
}

bool MPUWrapper::read_mpu(float ypr_out[3]) {
    if (!dmpReady) return false;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(internal_ypr, &q, &gravity);

        for (int i = 0; i < 3; i++) {
            ypr_out[i] = internal_ypr[i] * 180 / M_PI;
        }

        return true;
    }
    return false;
}
