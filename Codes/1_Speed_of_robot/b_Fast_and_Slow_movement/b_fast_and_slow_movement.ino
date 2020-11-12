/* 
  Author : Muhammad Luqman 
  Date   : 16-9-2020 

  Slowly Increase Robot Speed Then Decrease

*/

// Assigning names to esp32 Pins 

#define motorRa 27
#define motorRb 26
#define motorLa 25
#define motorLb 33

// PWM pin for both motor to attach configured Channel
#define Rpwm 32
#define Lpwm 14


// Global Variables for PWM
const int channel_R = 0;
const int channel_L = 1;
const int freq = 5000;
const int res = 8;

void setup(){ 
  setup_all_pinouts();
  setup_direction(); 
  Serial.begin(115200);
}

void loop(){
    for (int pwm_value=0 ;pwm_value<=255;pwm_value=pwm_value+15){

      ledcWrite(channel_R , pwm_value);               
      ledcWrite(channel_L , pwm_value);
      Serial.print("Increasing PWM Value = ");
      Serial.println(pwm_value);
      delay(500); 
    }

  for (int pwm_value=255;pwm_value>=0;pwm_value=pwm_value-15){
    ledcWrite(channel_R , pwm_value);               
    ledcWrite(channel_L , pwm_value);
    Serial.print("Decreasing PWM Value = ");
    Serial.println(pwm_value); 
    delay(500);  }

  }
  
void setup_direction(){
  digitalWrite(motorLa,HIGH);
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorLb,LOW);
  digitalWrite(motorRb,LOW);

}
void setup_all_pinouts(){
  
  pinMode(motorLa, OUTPUT);
  pinMode(motorLb, OUTPUT);
  pinMode(motorRa, OUTPUT);
  pinMode(motorRb, OUTPUT);
  
  ledcSetup(channel_R ,freq , res);
  ledcSetup(channel_L ,freq , res);

  ledcAttachPin(Rpwm,channel_R);
  ledcAttachPin(Lpwm,channel_L);
}
