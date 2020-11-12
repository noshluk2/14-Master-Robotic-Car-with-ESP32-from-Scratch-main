/* 
  Author : Muhammad Luqman 
  Date   : 16-9-2020 

  - Check Encoder ticks with varying speed of robot PER REVOLUTION of Tyre
  - Input Through Serial Monitor

*/


#define motorRa 27
#define motorRb 26
#define motorLa 25
#define motorLb 33
#define Rpwm 32
#define Lpwm 14

// Defining Encoder Pins

#define enc_RA 18
#define enc_RB 5
#define enc_LA 17
#define enc_LB 16



// Global Variables -> to be used by different functions .
// const int = constant integer which means this is not a variable , it will not change through the code.

// GLobal Variables
  int g_ticks;
  int count_R = 0;                                  //For Encoders
  int count_L = 0;                                
  const int channel_R = 0;
  const int channel_L = 1;                          // For Pwm setup                                 

void setup() {
  Serial.begin(115200);                             // starting Serial monitor at buad rate 115200 -> you can change it as-well
  setup_motors();

}

void loop() {
   if (Serial.available() > 0) {
     // in Serial , (1,300,200) = right motor,300 goal ticks , 200 Pwm value
     // in Serial , (2,350,200) = left motor ,350 goal ticks , 200 Pwm value
     // reset after one command as extra ticks are counted due to tyre inertia
    String first  = Serial.readStringUntil(',');
    String second = Serial.readStringUntil(',');
    String third  = Serial.readStringUntil('\0'); 
    // input in serial is string (CHAR)
    // Converting it into Integers
    int motor  =first.toInt();
    g_ticks=second.toInt();
    int pwm_val=third.toInt();

    if( motor == 1)
    { 
    Serial.print(" Running RIGHT Motor");delay(2000);
    digitalWrite(motorLa,HIGH);
    digitalWrite(motorRa,HIGH);
    digitalWrite(motorLb,LOW);
    digitalWrite(motorRb,LOW);
    ledcWrite(channel_R , pwm_val);                        
    ledcWrite(channel_L , 0);

    }
    if( motor == 2){ 
    Serial.print(" Running LEFT Motor");delay(2000);
    digitalWrite(motorLa,HIGH);
    digitalWrite(motorRa,HIGH);
    digitalWrite(motorLb,LOW);
    digitalWrite(motorRb,LOW);
    ledcWrite(channel_R , 0);                    
    ledcWrite(channel_L , pwm_val);
    }
    }
    
    if(count_L > g_ticks){
      Serial.println("REACHED LEFT");stopp();count_L=0;
      }
  
      if(count_R > g_ticks){
      Serial.println("REACHED RIGHT");stopp();count_R=0;
      }
  Serial.print(count_L);Serial.print(" | ");Serial.println(count_R);
}


// all motor setup with encoders
void setup_motors(){
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

  // encoder pinout defination
  pinMode(enc_RA,INPUT);
  pinMode(enc_RB,INPUT);
  pinMode(enc_LA,INPUT);
  pinMode(enc_LB,INPUT);
  // Interrupt connection to gpio pins and defining interrupt case
  attachInterrupt(digitalPinToInterrupt(enc_RA),Update_encR,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LA),Update_encL,CHANGE);

}
// Encoders-Interrupt callback functions

void Update_encR(){ // no extra stuff here like Serial Print -> causes Guru meditation error
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
}

// function to enable the motors of robot
void start(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);
  ledcWrite(channel_R , 150);                             // giving each motor 150 dutycycle resolution
  ledcWrite(channel_L , 150);
}
