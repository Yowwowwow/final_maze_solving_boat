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
#define led 13

//MPU6050
MPUWrapper mpu;

// 建立SERVO物件
// Servo myservo;

// 建立超音波物件
Ultrasonic ultrasonic0(4, 5);
Ultrasonic ultrasonic1(6, 7);
Ultrasonic ultrasonic2(8, 9); //front
Ultrasonic ultrasonic3(10, 12);

int distL, distB, distF, distR;
int yaw;
int ccc = 0;
int dir = 0;
int canrot = 0;
int Lempty=0, Rempty=0;

float angle(){
  float tmp = yaw - dir;
  if(tmp<-180)tmp+=360;
  else if(tmp>180)tmp-=360;
  return tmp;
}

void setup() {
  Serial.begin(9600);

  // myservo.attach(9);

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

  while (!Serial);
  while(1){
    read_ultrasonic();
    if(distF >= 15 && distF <= 100)break;
    delay(50);
  }
  if (!mpu.begin()) {
      Serial.println("MPU failed to start.");
      while (1);
  }
  for(int i=0;i<2;i++){
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

void loop() {
  // int ls=0, rs=0;
  // read_ultrasonic();
  // if(distance0<100)ls=100;
  // if(distance1<100)rs=100;
  // motor(ls, rs);
  read_ultrasonic();
  read_MPU();
  int ls=0, rs=0; //left speed and right speed
  // if(canrot==1){
  //   if(distL > 20 && distB > 20){
  //     if(Lempty>=8){
  //       dir += 90;
  //       if(dir>180)dir-=360;
  //       canrot = 0;
  //       Lempty = Rempty = 0;
  //     }
  //     else Lempty++;
  //   }
  //   else Lempty = 0;
  //   if(distR > 20 && distB > 20){
  //     if(Rempty>=8){
  //       dir -= 90;
  //       if(dir<-180)dir+=360;
  //       canrot = 0;
  //       Lempty = Rempty = 0;
  //     }
  //     else Rempty++;
  //   }
  //   else Rempty = 0;
  // }
  float ang = angle();
  if(ang<5 && ang>-5){ //forward distance
    ls = 150;
    rs = 150;
    canrot = 1;
  }
  else{
    if(ang > 5){//if left distance > right distance
      //int s = (((int)ang)-5)/175*60+60;
      ls = -100;
      rs = 100;
    }
    else{
      //int s = ((((int)ang)*(-1))-5)/175*60+60;
      //s*=10;
      //if(s>100)s=100;
      ls = 100;
      rs = -100;
    }
  }
  motor(ls, rs);
  delay(50);
}

void read_ultrasonic() {
  distL = ultrasonic0.read(); //L
  distB = ultrasonic1.read(); //B
  distF = ultrasonic2.read(); //F
  distR = ultrasonic3.read(); //R
  //Serial.print("0: ");
  Serial.print("L: ");
  Serial.print(distL);
  //Serial.print("  1: ");
  // Serial.print(dist1);
  //Serial.print("  3: ");
  Serial.print("  R: ");
  Serial.print(distR);
  Serial.print("  F: ");
  Serial.println(distF);
}

void read_MPU() {
  float ypr[3];
  if (mpu.read_mpu(ypr)) {
    yaw = ypr[0];
    Serial.print(ccc);
    Serial.print(": ");
    Serial.print("Yaw: ");
    Serial.print(ypr[0]);
    Serial.print("  Pitch: ");
    Serial.print(ypr[1]);
    Serial.print("  Roll: ");
    Serial.println(ypr[2]);
    ccc++;
  }
  //delay(100);
}

void pid() {
  //TODO
}

void motor(int ls, int rs) {
  if(ls>=0){
    digitalWrite(Ain2, LOW);
    digitalWrite(Ain1, HIGH);
    analogWrite(Apwm, ls);
  }
  else{
    digitalWrite(Ain1, LOW);
    digitalWrite(Ain2, HIGH);
    analogWrite(Apwm, -1*ls);
  }
  if(rs>=0){
    digitalWrite(Bin2, LOW);
    digitalWrite(Bin1, HIGH);
    analogWrite(Bpwm, rs);
  }
  else{
    digitalWrite(Bin1, LOW);
    digitalWrite(Bin2, HIGH);
    analogWrite(Bpwm, -1*rs);
  }
  //Serial.println("motor run");
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
