/* 
  Author : Muhammad Luqman 
  Date   : 16-9-2020 

  - Realtime Speed Measuring of Robot through Bluetooth

*/

// Defineing the Pins for the robot -> You can change here easily if your connections are different.
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define motorRa 27
#define motorRb 26
#define motorLa 25
#define motorLb 33
#define Rpwm 32
#define Lpwm 14
#define enc_RA 18
#define enc_RB 5
#define enc_LA 17
#define enc_LB 16

// Global Variables -> to be used by different functions .
// unsigned int because we only need positive values
// short because of only 2 decimal point of accuracy is required.

//Variables
  int count_R = 0;                                  //For Encoders
  int count_L = 0;
  short one_rev_dist = 20.9 ;                       // its in cm
  unsigned int one_rev_ticks = 750 ;                // its an average the encoders are not very accurate so error will be produced.
  const int channel_R = 0;
  const int channel_L = 1;                          // For Pwm setup
  double revolution_done,linear_speed;    
  double angle_R,angle_L;
  unsigned long currentTime, previousTime;
  char command;int pwm;
  double r_dist,l_dist,delta_t;                          

void setup() {
  Serial.begin(115200);                             // starting Serial monitor at buad rate 115200 -> you can change it as-well
  setup_motors();
  SerialBT.begin("Connect  Robot Car");
//  delay(2000);                                      // given time to setup the Robot
}

void loop() {
      if (SerialBT.available()) {
      command=SerialBT.read();}
  switch (command) {

    case 'f':
      forward_set();
      break;
    case 's':
      ledcWrite(channel_R , 0);                            
      ledcWrite(channel_L , 0);
      break;
    case 't': // tell speed
      get_speed();
      break;
    case 'i': // increase speed
      pwm=pwm+20;
      ledcWrite(channel_R , pwm);                            
      ledcWrite(channel_L , pwm);
      break;
    case 'd': // decrease speed
      pwm=pwm-20;
      ledcWrite(channel_R , pwm);                            
      ledcWrite(channel_L , pwm);
      break;
  
    
    }
    
}


void get_speed(){ 
    delta_t=(double)(millis() - previousTime);  
    if ( delta_t > 1000){ // as delta_t is in " Milli Seconds " so we need to multiply with 1000  
        r_dist = (count_R * one_rev_dist)/one_rev_ticks;
        l_dist = (count_L * one_rev_dist)/one_rev_ticks;
        linear_speed= ((r_dist+l_dist)/2 ) * (1000/delta_t);
        previousTime=millis();
        //Serial.print("Linear Speed = ");Serial.println(linear_speed);
        SerialBT.print("linear_speed = ");SerialBT.println(linear_speed);
        reset_variables();  
  
  }
}
void reset_variables(){   
  count_R =0;
  count_L =0;
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
// Encoders-Interrupt callback functions
void Update_encR(){
   if (digitalRead(enc_RA) == digitalRead(enc_RB)) count_R--;
    else count_R++; 
}

void Update_encL(){
 if (digitalRead(enc_LA) == digitalRead(enc_LB)) count_L--;
  else count_L++; 
}

void forward_set(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);

}
