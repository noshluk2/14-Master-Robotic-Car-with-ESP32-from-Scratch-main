/* 
  Author : Muhammad Luqman 
  Date   : 18-9-2020 

  - Stacking all behaviours
  - States are in order so no need to make a state machine 
  - Direct and simple flow is needed

*/

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define Ir_L 19                                         // Ir pins
#define Ir_R 23
#define motorRa 27
#define motorRb 26
#define motorLa 25
#define motorLb 33
#define Rpwm 32
#define Lpwm 14

#define echo_Pin 15
#define trigger_Pin 13
const int channel_R = 0;
const int channel_L = 1;
const int freq = 5000;
const int res =8;
 
int Right_motor_speed    = 145;                        // Motor Base speeds
int Left_motor_speed     = 130;
int irReadings[2];                                     //Ir sensor array defination

void setup()
{ 
  Serial.begin(115200);
  setup_all_pinouts();
  delay(3000);
  start_robot();

}

void loop(){
  get_Ir();
  action();

}


void get_Ir() {
irReadings[0]=(digitalRead(Ir_L));
irReadings[1]=(digitalRead(Ir_R));

}

void action() {
 // 1 represents to be on black line
  if ((irReadings[0] == 0) && (irReadings[1] == 1))  { // only right  sensor on black
    left();
    }
  else if ((irReadings[0] == 0) && (irReadings[1] == 0) ) { // straight
    forward();
    }
  else if ((irReadings[0] == 1) && (irReadings[1] == 0) ) { // only left sensor on black
    right();
    }
  else if  ((irReadings[0] == 1) && (irReadings[1] == 1)) {
      stop_robot();
  }
}
  

 void setup_all_pinouts(){
    pinMode(Ir_L,INPUT);
    pinMode(Ir_R,INPUT);
    pinMode(trigger_Pin, OUTPUT); 
    pinMode(echo_Pin, INPUT);
    setup_motors();
 } 
void setup_motors(){
  // pwm setup variables
  const int freq = 5000;
  const int res = 8;

  // direction for motor pinout defination
  pinMode(motorLa, OUTPUT);
  pinMode(motorLb, OUTPUT);
  pinMode(motorRa, OUTPUT);
  pinMode(motorRb, OUTPUT);
  // Pwm functionality setup
  ledcSetup(channel_R ,freq , res);
  ledcSetup(channel_L ,freq , res);
  ledcAttachPin(Rpwm,channel_R);
  ledcAttachPin(Lpwm,channel_L);
}

// function to enable the motors of robot
void start_robot(){
  ledcWrite(channel_R , Right_motor_speed);
  ledcWrite(channel_L , Left_motor_speed);
  digitalWrite(motorLa,HIGH);                             // Forward direction configuration
  digitalWrite(motorRa,HIGH);
}



void forward(){
  digitalWrite(motorLa,HIGH);                             
  digitalWrite(motorRa,HIGH); 
  digitalWrite(motorLb,LOW);                             
  digitalWrite(motorRb,LOW);  

}
void left(){
  digitalWrite(motorLa,LOW);                             
  digitalWrite(motorRa,HIGH); 
  digitalWrite(motorLb,LOW);                             
  digitalWrite(motorRb,LOW);  

}
void right(){
  digitalWrite(motorLa,HIGH);                             
  digitalWrite(motorRa,LOW); 
  digitalWrite(motorLb,LOW);                             
  digitalWrite(motorRb,LOW);  

}


// function to disable the motors of robot
void stop_robot(){
  digitalWrite(motorLa,LOW);                             
  digitalWrite(motorRa,LOW);  
}
