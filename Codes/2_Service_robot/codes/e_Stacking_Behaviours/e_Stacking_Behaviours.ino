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
 
int Right_motor_speed    = 165;                        // Motor Base speeds
int Left_motor_speed     = 140;
int irReadings[2];                                     //Ir sensor array defination
int table_stop=0;

long duration;
int distance;
char state='a';
bool serve_meal=false;
bool table_is_selected;
int serve_at_table_number=0;String inString="";
void setup()
{ 
  Serial.begin(115200);
  setup_all_pinouts();
  Serial.println(" All set -> Get ready ");
  SerialBT.begin("Connect  Robot Car");
  delay(3000);

}

void loop(){
    switch (state)
    {
    case 'a': //Standing For Meal Picking
        pick_meal();
        SerialBT.print("Meal Picked -> Going to serve ");
        break;
    case 'b':// Going to Serve the Meal at respective Table
        reach_table();
        break;
    case 'c': // Returning to counter after meal had been served
        SerialBT.print("Returning to Counter");
        return_to_counter();
        break;
    
    }
}
void return_to_counter(){
  start_robot();
  digitalWrite(motorRa,HIGH); 
  digitalWrite(motorLa,LOW);
  digitalWrite(motorLb,HIGH); // left tyre reverse movement
  delay(1100);
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorLb,LOW);    
   while(!(table_stop == serve_at_table_number) ){
    get_Ir();
    action();
    
  }
  stop_robot(); state='d';
  SerialBT.print("Job Done !");stop_robot();
}


void reach_table(){
    get_Ir();
    action();
    if(table_stop == serve_at_table_number){
        stop_robot(); // we have reached the desired table
        measure_distance();// waiting for the meal to be picked up
        while(distance<4){ // looping until meal is placed
            measure_distance();
        }
        SerialBT.print("Meal Served");
        table_stop=0;//resetting value so it can count while returning
        delay(4000); // to easily pic the meal
        state='c'; // moving to next state
           
    }
}

void pick_meal(){
      while(!serve_meal){
        measure_distance();
        get_table_number();
        if(distance<4 && (table_is_selected>0)){
            serve_meal=true;
            delay(2000); // to easily place the glass
            state='b';  start_robot();
            SerialBT.print("Going to Serve at Table Number =  ");SerialBT.println(serve_at_table_number);
        }
    }
}
void get_table_number(){
    if (SerialBT.available()>0){ 
       // it reads a char not a number
     int table_number_char=SerialBT.read();
     if(isDigit(table_number_char)){
     inString="";
     inString+=(char)table_number_char;
     }
     serve_at_table_number = inString.toInt();
     table_is_selected=true;
     
     }
     
}
void measure_distance(){
    digitalWrite(trigger_Pin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger_Pin, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigger_Pin, LOW);
    
    duration = pulseIn(echo_Pin, HIGH);
    distance= duration*0.034/2;
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
      table_stop++;delay(200);// as it was detection one occurence many times causing to stop at first table ->counting it 3 times

  }
  Serial.print(irReadings[0]);Serial.print(" | ");Serial.println(irReadings[1]);
  Serial.println(table_stop);
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
