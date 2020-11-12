/* 
  Author : Muhammad Luqman 
  Date   : 16-9-2020 
  
  Forward and reverse Robot Movement

- Robot will move in Forward direction for 3 seconds
- It will stop for 1 second
- Then it will move in Reverse Direction for 2 seconds
- Stop for 1 second
- Repeat from start

*/


// Assigning names to  Pins connected with ESP32

#define motorRa 27
#define motorRb 26

#define motorLa 25
#define motorLb 33

void setup(){ 
  // defining pins working type
  pinMode(motorRa, OUTPUT);
  pinMode(motorRb, OUTPUT);

  pinMode(motorLa, OUTPUT);
  pinMode(motorLb, OUTPUT);
  
}

void loop(){
  forward();
  delay(3000);

  stop_();
  delay(3000);
  
  reverse();
  delay(3000);

  stop_();
  delay(3000);

}
 

  void forward(){
    digitalWrite(motorRa,HIGH);
    digitalWrite(motorRb,LOW);
    digitalWrite(motorLa,HIGH);
    digitalWrite(motorLb,LOW);
  }

  void reverse(){
    digitalWrite(motorRa,LOW);
    digitalWrite(motorRb,HIGH);
    digitalWrite(motorLa,LOW);
    digitalWrite(motorLb,HIGH);
  }

  void stop_(){
  digitalWrite(motorRa,LOW);
  digitalWrite(motorRb,LOW);
  digitalWrite(motorLa,LOW);
  digitalWrite(motorLb,LOW);
  
  }
