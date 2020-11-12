/* 
  Author : Muhammad Luqman 
  Date   : 22-9-2020 

  - Drive Robot
  - Send X and Y over Bluetooth

*/

//  http://www-personal.umich.edu/~johannb/position.htm book link

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// motor pins
#define enc_RA 18
#define enc_RB 5
#define enc_LA 17
#define enc_LB 16
#define motorRa 27
#define motorRb 26
#define motorLa 25
#define motorLb 33
#define Rpwm 32
#define Lpwm 14

//Fixed values
#define WHEEL_DIAMETER 0.067
#define PULSES_PER_REVOLUTION 870.0
#define AXLE_LENGTH  0.129
#define PI 3.1416

// New Variables
float disp_r_wheel       = 0;
float disp_l_wheel       = 0;
int count_R_prev         = 0;
int count_L_prev         = 0;             
float x                  = 0;
float y                  = 0;
float theta              = 0;
float meter_per_ticks    = PI * WHEEL_DIAMETER / PULSES_PER_REVOLUTION;
float orientation_angle , disp_body;

// Old Variables
char command;
int count_R              = 0;                          //For Encoders
int count_L              = 0;  
const int channel_R      = 0;                          //PWM setup
const int channel_L      = 1;   
int Right_motor_speed    = 130;                        // Motor Base speeds
int Left_motor_speed     = 130;

unsigned long  prev_time;

void setup()
{ 
  Serial.begin(115200);
  setup_motors();
  SerialBT.begin("Connect to Robot Car");
} 


void loop()
{
   if (SerialBT.available()) {
   drive();}
   else{
    calculate_traveling();  
    send_data();}
}

void drive(){
  command=SerialBT.read();
  switch (command) {
    case 'f':
      forward();
      break;
    case 'b':
      reverse();
      break;
    case 'r':
      right();
      break;
    case 'l':
      left();
      break;
    case 's':
      stopp();
      break;
                }
  
}

void calculate_traveling(){
      
    count_L_prev = count_L;
    count_R_prev = count_R;
    count_L      = 0;
    count_R      = 0;
    disp_l_wheel = (float)count_L_prev * meter_per_ticks;              // geting distance in meters each wheel has traveled
    disp_r_wheel = (float)count_R_prev * meter_per_ticks;
    
    if (count_L_prev == count_R_prev)
    {                                                                  // The Straight line condition -> book reference Where am i ?
      x += disp_l_wheel * cos(theta);
      y += disp_l_wheel * sin(theta);
    }
    else                                                               // for circular arc equations change
    { orientation_angle = (disp_r_wheel - disp_l_wheel)/AXLE_LENGTH;
      disp_body   = (disp_r_wheel + disp_l_wheel) / 2.0;
      x += (disp_body/orientation_angle) * (sin(orientation_angle + theta) - sin(theta));
      y -= (disp_body/orientation_angle) * (cos(orientation_angle + theta) - cos(theta));
      theta += orientation_angle;

     
       while(theta > PI)
         theta -= (2.0*PI);
       while(theta < -PI) 
         theta += (2.0*PI); 
    }

} 
  

void send_data(){ // sending data after a fixed duration of time
  if( millis() - prev_time>1000){
  SerialBT.print("X =");SerialBT.print(x);SerialBT.print(" | ");
  SerialBT.print("Y =");SerialBT.print(y);SerialBT.print(" | ");
  SerialBT.print("t =");SerialBT.println((theta*180)/3.14);
  prev_time=millis();  }
   
}

// Encoders-Interrupt callback functions
void Update_encR(){
   if (digitalRead(enc_RA) == digitalRead(enc_RB)) count_R--;
    else count_R++;  
}

void Update_encL(){
 if (digitalRead(enc_LA) == digitalRead(enc_LB)) count_L--;
  else count_L++; 
}

// all motor setup is done in one function call
void setup_motors(){
  // pwm setup variables
  const int freq = 5000;
  const int res = 8;

  // direction for motor pinout defination
  pinMode(motorLa, OUTPUT);
  pinMode(motorLb, OUTPUT);
  pinMode(motorRa, OUTPUT);
  pinMode(motorRb, OUTPUT);
  // encoder pinout defination
  pinMode(enc_RA,INPUT);
  pinMode(enc_RB,INPUT);
  pinMode(enc_LA,INPUT);
  pinMode(enc_LB,INPUT);
  // Interrupt connection to gpio pins and defining interrupt case
  attachInterrupt(digitalPinToInterrupt(enc_RA),Update_encR,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LA),Update_encL,CHANGE);
  // Pwm functionality setup
  ledcSetup(channel_R ,freq , res);
  ledcSetup(channel_L ,freq , res);
  ledcAttachPin(Rpwm,channel_R);
  ledcAttachPin(Lpwm,channel_L);

}


void stopp(){
  ledcWrite(channel_R , 0);                             
  ledcWrite(channel_L , 0);
}

void reverse(){
  digitalWrite(motorLa,LOW);
  digitalWrite(motorRa,LOW);
  digitalWrite(motorLb,HIGH);
  digitalWrite(motorRb,HIGH);
  ledcWrite(channel_R , 150);                             
  ledcWrite(channel_L , 150);
}
void forward(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);
  ledcWrite(channel_R , 150);                             
  ledcWrite(channel_L , 150);
}

void left(){
  digitalWrite(motorLa,LOW);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);
  ledcWrite(channel_R , 150);                             
  ledcWrite(channel_L , 150);
}
void right(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,LOW);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);
  ledcWrite(channel_R , 150);                             
  ledcWrite(channel_L , 150);
}
