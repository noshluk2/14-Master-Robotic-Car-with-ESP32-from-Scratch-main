/* 
  Author : Muhammad Luqman 
  Date   : 22-9-2020 

  - Implementating Differential drive Kinematics
  - Encoder Test for localization
  - Producing X Y co ordinate values

*/



// motor pins
#define enc_RA 18
#define enc_RB 5
#define enc_LA 17
#define enc_LB 16

//-------------->>>>>>>>>>>>>>>> Very interesting thing change the value of pulse_per_rev you will get wrong answers of x y theta 
// low values show much more distance traveling 
//Fixed values
#define WHEEL_DIAMETER 0.067
#define PULSES_PER_REVOLUTION 850.0
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
int count_R              = 0;                          //For Encoders
int count_L              = 0;
int Right_motor_speed    = 130;                        // Motor Base speeds
int Left_motor_speed     = 130;
unsigned long  prev_time;

void setup()
{ 
  Serial.begin(115200);
  setup_encoder();
} 


void loop(){
  calculate_traveling();
  print_x_y();
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
      theta += orientation_angle; // it is in radians 

     
       while(theta > PI)
         theta -= (2.0*PI);
       while(theta < -PI) 
         theta += (2.0*PI); 
    }
  
} 
  

void print_x_y(){

 Serial.print("X = ");Serial.print(x);Serial.print(" "); 
 Serial.print("Y = ");Serial.print(y);Serial.print(" ");
 Serial.print("Heading = ");Serial.println((theta*180)/3.14); // as we can easily understand degrees so converting from radians to degrees
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

void setup_encoder(){
 
  pinMode(enc_RA,INPUT);
  pinMode(enc_RB,INPUT);
  pinMode(enc_LA,INPUT);
  pinMode(enc_LB,INPUT);
  // Interrupt connection to gpio pins and defining interrupt case
  attachInterrupt(digitalPinToInterrupt(enc_RA),Update_encR,CHANGE);
  attachInterrupt(digitalPinToInterrupt(enc_LA),Update_encL,CHANGE);
  
}
