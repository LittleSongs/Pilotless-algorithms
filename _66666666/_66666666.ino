/*
Name:    Car_V1.ino
Created: 3/11/2018 8:39:02 PM
Author:  Administrator
*/

/*
Name:    car1.ino
Created: 2018-02-25 15:15:16
Author:  Leo
*/

//pwm,电机调速  
#define leftWheels_EN 10
#define rightWheels_EN 11

#define leftPwm 110
#define rightPwm 100

//左右轮子
#define leftWheels_IN1 8
#define leftWheels_IN2 12

#define rightWheels_IN3 13
#define rightWheels_IN4 9

//记点
unsigned short crossroads = 0;

//节点
unsigned short node=1;

//红外传感器
struct {
  unsigned short sensor_1 = 50;
  unsigned short sensor_2 = 48;
  unsigned short sensor_3 = 46;
  unsigned short sensor_4 = 44;
  unsigned short sensor_5 = 42;//中
}frontSensor;


struct {
  unsigned short sensor_1 = 23;
  unsigned short sensor_2 = 33;
}midSensor;


void setup() {
  pinMode(leftWheels_EN, OUTPUT);
  pinMode(rightWheels_EN, OUTPUT);
  pinMode(leftWheels_IN1, OUTPUT);
  pinMode(leftWheels_IN2, OUTPUT);
  pinMode(rightWheels_IN3, OUTPUT);
  pinMode(rightWheels_IN4, OUTPUT);
  pinMode(frontSensor.sensor_1, INPUT);
  pinMode(frontSensor.sensor_2, INPUT);
  pinMode(frontSensor.sensor_3, INPUT);
  pinMode(frontSensor.sensor_4, INPUT);
  pinMode(frontSensor.sensor_5, INPUT);//中
  pinMode(midSensor.sensor_1, INPUT);
  pinMode(midSensor.sensor_2, INPUT);
  Serial.begin(9600);
}

//void adjust_turn_left(const short intensity) {
//  //给右边电机速度intensity,右边轮子速度0
//  Control_motor(leftWheels_IN1, leftWheels_IN2, leftWheels_EN, intensity);
//  Control_motor(rightWheels_IN3, rightWheels_IN4, rightWheels_EN, intensity);
//  delay(1000);
//}
//
//void adjust_turn_right(const short intensity) {
//  //给左边电机速度intensity,右边轮子速度0
//  Control_motor(rightWheels_IN3, rightWheels_IN4, rightWheels_EN, intensity);
//  Control_motor(leftWheels_IN1, leftWheels_IN2, leftWheels_EN, -intensity);
//  delay(1000);
//}

void go_ahead() {
  analogWrite(leftWheels_EN, leftPwm);
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 1);
  analogWrite(rightWheels_EN, rightPwm);
  digitalWrite(rightWheels_IN3, 0);
  digitalWrite(rightWheels_IN4, 1);
}

void brake() {
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 0);
  digitalWrite(rightWheels_IN3, 0);
  digitalWrite(rightWheels_IN4, 0);
}

void left() {
  analogWrite(leftWheels_EN, leftPwm - 50);
  analogWrite(rightWheels_EN, rightPwm);
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 1);
  digitalWrite(rightWheels_IN3, 1);
  digitalWrite(rightWheels_IN4, 0);
}

void right() {
  analogWrite(leftWheels_EN, leftPwm);
  analogWrite(rightWheels_EN, rightPwm - 50);
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 1);
  digitalWrite(rightWheels_IN3, 1);
  digitalWrite(rightWheels_IN4, 0);
}

void left90() {
  analogWrite(leftWheels_EN, leftPwm);
  analogWrite(rightWheels_EN, rightPwm);
  digitalWrite(leftWheels_IN1, 1);
  digitalWrite(leftWheels_IN2, 0);
  digitalWrite(rightWheels_IN3, 1);
  digitalWrite(rightWheels_IN4, 0);
}

void left90_slow() {
  analogWrite(leftWheels_EN, leftPwm-50);
  analogWrite(rightWheels_EN, rightPwm-50);
  digitalWrite(leftWheels_IN1, 1);
  digitalWrite(leftWheels_IN2, 0);
  digitalWrite(rightWheels_IN3, 1);
  digitalWrite(rightWheels_IN4, 0);
}

void right90() {
  analogWrite(leftWheels_EN, leftPwm);
  analogWrite(rightWheels_EN, rightPwm);
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 1);
  digitalWrite(rightWheels_IN3, 0);
  digitalWrite(rightWheels_IN4, 1);
}

void right90_slow() {
  analogWrite(leftWheels_EN, leftPwm-50);
  analogWrite(rightWheels_EN, rightPwm-50);
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 1);
  digitalWrite(rightWheels_IN3, 0);
  digitalWrite(rightWheels_IN4, 1);
}

void stop() {
  digitalWrite(leftWheels_IN1, 0);
  digitalWrite(leftWheels_IN2, 0);
  digitalWrite(rightWheels_IN3, 0);
  digitalWrite(rightWheels_IN4, 0);
}

void turnleft(){
  while(digitalRead(frontSensor.sensor_1)==HIGH)
    left90();
  while(digitalRead(frontSensor.sensor_2)==HIGH)
    left90_slow();
  stop();
  ++node;
}

void turnright(){
  while(digitalRead(frontSensor.sensor_1)==HIGH)
    right90();
  while(digitalRead(frontSensor.sensor_2)==HIGH)
    right90_slow();
  stop();
  ++node;
}



void loop()
{
  delay(5000);
  while (1) {
    go_ahead();

    //前面的1,2传感器检测是否到达十字
    if(digitalRead(frontSensor.sensor_1)==LOW || digitalRead(frontSensor.sensor_5)==LOW){
      //delay(400);
      //中间的1,2传感器
      while(digitalRead(midSensor.sensor_1) == HIGH && digitalRead(midSensor.sensor_2) == HIGH);
      //转弯
      stop();
      delay(1000);
      switch(node){
        case 3:
          turnleft();break;
        case 4:
          turnright();break;
        default:
          break;
      }
    }
    if (digitalRead(frontSensor.sensor_2) == LOW) {
      //turn left
      //如果中间sensor检测不到白线，就一直转
      while (digitalRead(frontSensor.sensor_3) == HIGH) {
        //Serial.println("zhong1");
        left();
      }
    }
    if (digitalRead(frontSensor.sensor_4) == LOW) {
      //turn right
      while (digitalRead(frontSensor.sensor_3) == HIGH) {
        right();
      }
    }
  }
}





