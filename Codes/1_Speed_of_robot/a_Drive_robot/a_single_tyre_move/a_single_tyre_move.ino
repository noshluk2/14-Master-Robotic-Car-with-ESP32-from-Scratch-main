/* 
Author : Muhammad Luqman 
Date   : 16-9-2020 
Lets start with Moving Just One TYRE
- Rotate tyre for 2 seconds in forward direction
- Stop it
- Rotate the tyre in reverse for 2 seconds
- Again Stop it 
- Repeat the process


NOTE :
Rotating only one Tyre causes Robot to take a turn !!

*/


// Giving Names to Pins of ESP32 for better understanding ( if not use pins instead of names )
// Two pins for One DC Motor
#define motorRa 27
#define motorRb 26

void setup(){ 
  // Setting up how we will interact with the pins
  // two directional pins are set as output ( as these are signals going out from ESP32)

  pinMode(motorRa, OUTPUT);
  pinMode(motorRb, OUTPUT);
}

void loop(){
  // Forward
  digitalWrite(motorRa,HIGH);
  digitalWrite(motorRb,LOW);
  delay (2000);
  // Stop
  digitalWrite(motorRa,LOW);
  digitalWrite(motorRb,LOW);
  delay(3000);
  // Reverse
  digitalWrite(motorRa,LOW);
  digitalWrite(motorRb,HIGH);
  delay (2000);
  
  // Stoping 
  digitalWrite(motorRa,LOW);
  digitalWrite(motorRb,LOW);
  delay(3000);
  }
