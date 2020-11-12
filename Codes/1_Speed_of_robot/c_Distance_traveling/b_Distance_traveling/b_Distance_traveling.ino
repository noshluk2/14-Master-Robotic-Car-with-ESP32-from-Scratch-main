/* 
  Author : Muhammad Luqman 
  Date   : 16-9-2020 

  - Input Through Serial Monitor

*/

// Move Straight for 100 Centi-Meters

// Defineing the Pins for the robot -> You can change here easily if your connections are different.

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

// unsigned int because we only need positive values
// short because of only 2 decimal point of accuracy is required.

//Variables
  int count_R = 0;                                  //For Encoders
  int count_L = 0;
  short one_rev_dist = 20.9 ;                       // its in cm
  unsigned int one_rev_ticks = 500 ;                // its an average the encoders are not very accurate so error will be produced.
  short ticks_req = 0;                              //  Defining only for " Right MOTOR "
  const int channel_R = 0;
  const int channel_L = 1;                          
  bool car_moving;                                  

void setup() {
  Serial.begin(115200);                             // starting Serial monitor at buad rate 115200 -> you can change it as-well
  setup_motors();
  delay(4000);                                      // given time to setup the Robot
  Precise_movement();
}

void loop() {} // we have an empty loop

void Precise_movement(){
  forward_direction();
  distance_to_travel(150);
  //Serial.println("Went 100cm forward");

}


void distance_to_travel(int centimeters){
  reset_variables();                                // reseting variables so it may not count previous ticks into new task.'
  short rev_req = centimeters / one_rev_dist;       // this will produced revolution required for required distance to travel
  ticks_req = rev_req * one_rev_ticks ;             // this will produced ticks motor should do to complete required distance
  Serial.println(ticks_req); 
  start();                                          //Enable motors
  while(car_moving){
    Serial.println(count_R);
    if(count_R>=ticks_req){
      stopp();  }
                   }
  }

void reset_variables(){
  ticks_req = 0 ;                                         
  count_R =0;
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

// function to disable the motors of robot
void stopp(){
  ledcWrite(channel_R , 0);                             // giving each motor 0 dutycycle value
  ledcWrite(channel_L , 0);    
  car_moving=false;
}

// function to enable the motors of robot
void start(){
  ledcWrite(channel_R , 200);                             // giving each motor 150 dutycycle resolution
  ledcWrite(channel_L , 200);
  car_moving=true;
}


void reverse_direction(){
  digitalWrite(motorLa,LOW);
  digitalWrite(motorRa,LOW);
  digitalWrite(motorLb,HIGH);
  digitalWrite(motorRb,HIGH);
}
void forward_direction(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);
}
