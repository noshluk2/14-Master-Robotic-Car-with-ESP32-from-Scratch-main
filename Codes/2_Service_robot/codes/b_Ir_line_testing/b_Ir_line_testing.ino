/* 
  Author : Muhammad Luqman 
  Date   : 18-9-2020 

  - Interfacing IR sensors and detecting Black Line

*/
// Lights on the sensor are faulty but This code is too check the output clearly

#define Ir_L 23                                         // Ir pins
#define Ir_R 19

int irReadings[2];                                    //Ir sensor array defination

void setup()
{ 
  pinMode(Ir_L,INPUT);
  pinMode(Ir_R,INPUT);
  Serial.begin(115200);
}

void loop(){    Get_Ir(); }

void Get_Ir() {
  irReadings[0]=(digitalRead(Ir_L));
  irReadings[1]=(digitalRead(Ir_R));
  Serial.print(" \n ");Serial.print(irReadings[0]);Serial.print(" | ");
  Serial.print(irReadings[1]);Serial.print(" \n ");

}
