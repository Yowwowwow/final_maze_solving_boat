#include <Ultrasonic.h>
#include <Servo.h>
#include "MPUWrapper.h"

// 定義L298n腳位
#define Ain1 A1
#define Ain2 A2
#define Apwm 11
#define Bin1 A3
#define Bin2 2
#define Bpwm 3

//MPU6050
MPUWrapper mpu;

// 建立SERVO物件
Servo myservo;

// 建立超音波物件
Ultrasonic ultrasonic0(4, 5);
Ultrasonic ultrasonic1(6, 7);
Ultrasonic ultrasonic2(8, 9); //front
Ultrasonic ultrasonic3(10, 12);

int dist0, dist1, dist2, dist3;

void setup() {
  Serial.begin(9600);

  myservo.attach(9);

  pinMode(Apwm, OUTPUT);
  pinMode(Bpwm, OUTPUT);
  pinMode(Ain1, OUTPUT);
  pinMode(Bin1, OUTPUT);
  pinMode(Ain2, OUTPUT);
  pinMode(Bin2, OUTPUT);

  digitalWrite(Ain2, LOW);
  digitalWrite(Ain1, HIGH);
  digitalWrite(Bin2, LOW);
  digitalWrite(Bin1, HIGH);

  while (!Serial)
    ;

   if (!mpu.begin()) {
     Serial.println("MPU failed to start.");
     while (1)
       ;
   }
}

void loop() {
  // int ls=0, rs=0;
  // read_ultrasonic();
  // if(distance0<100)ls=100;
  // if(distance1<100)rs=100;
  // motor(ls, rs);
  read_ultrasonic();
  int ls=0, rs=0; //left speed and right speed
  if(1==1 || dist0 >= 4){ //forward distance
    ls = 200;
    rs = 200;
  }
  else{
    if(dist1 > dist2){//if left distance > right distance
      ls = 0;
      rs = 200;
    }
    else{
      ls = 200;
      rs = 0;
    }
  }
  motor(ls, rs);
  delay(50);
}

void read_ultrasonic() {
  dist0 = ultrasonic0.read();
  dist1 = ultrasonic1.read();
  dist2 = ultrasonic2.read();
  dist3 = ultrasonic3.read();
  Serial.print("0: ");
  Serial.print(dist0);
  Serial.print("  1: ");
  Serial.print(dist1);
  Serial.print("  2: ");
  Serial.print(dist2);
  Serial.print("  3: ");
  Serial.println(dist3);
}

void read_MPU() {
  float ypr[3];
  if (mpu.read_mpu(ypr)) {
    Serial.print("Yaw: ");
    Serial.print(ypr[0]);
    Serial.print("  Pitch: ");
    Serial.print(ypr[1]);
    Serial.print("  Roll: ");
    Serial.println(ypr[2]);
  }
  delay(100);
}

void pid() {
  //TODO
}

void motor(int speed_left, int speed_right) {
  analogWrite(Apwm, speed_left);
  analogWrite(Bpwm, speed_right);
  Serial.println("motor run");
  // if (speed_left > 0) {
  //   analogWrite(left_pwm1, 0);
  //   analogWrite(left_pwm2, speed_left);
  // } else {
  //   analogWrite(left_pwm1, speed_left);
  //   analogWrite(left_pwm2, 0);
  // }

  // if (speed_right > 0) {
  //   analogWrite(right_pwm1, 0);
  //   analogWrite(right_pwm2, speed_right);
  // } else {
  //   analogWrite(right_pwm1, speed_right);
  //   analogWrite(right_pwm2, 0);
  // }
}
