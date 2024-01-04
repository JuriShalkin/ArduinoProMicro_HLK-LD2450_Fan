#define INTERVAL 100

#include <Servo.h>

Servo myservo1;

long previousMillis = 0;
byte findheader = 0; 
int16_t Target1_angle = 0;

float tanAngle[] = {0.000, 0.035, 0.070, 0.105, 0.141, 0.176, 0.213, 0.249, 0.287, 0.325, 0.364, 0.404, 0.445, 0.488, 0.532, 0.577, 0.625, 0.675, 0.727, 0.781, 0.839, 0.900, 0.966, 1.036, 1.111, 1.192, 1.280, 1.376, 1.483, 1.600, 1.732};

struct Position {
  int16_t Target1_X;
  int16_t Target1_Y;
  int16_t Target1_angle;
  //int Target1_Dist;
  //int Target1_Speed;
  //int Target2_X;
  //int Target2_Y;
  //int Target2_Dist;
  //int Target2_Speed;
  //int Target3_X;
  //int Target3_Y;
  //int Target3_Dist;
  //int Target3_Speed;
};

Position GetCoord() {
  Position str;
  if(Serial1.available()){
    if (Serial1.read()==0xAA && Serial1.read()==0xFF) {
      findheader=1;
      }
    else {
      return;
      }
    }
  if (findheader == 1 && Serial1.available()){
   byte h_dataX, l_dataX, h_dataY, l_dataY;
   byte buf[28];
   Serial1.readBytes(buf, 28);
   h_dataX = buf[2];
   l_dataX = buf[3];
   h_dataY = buf[4];
   l_dataY = buf[5];
   if (buf[27]==0xCC) {
    if((l_dataX >> 7) == 0x1){
      str.Target1_X = ((l_dataX & 0x7F) << 8) + h_dataX;  
    }
    else {
      str.Target1_X = 0-(((l_dataX & 0x7F) << 8) + h_dataX);  
    }
       
    if((l_dataY >> 7) == 0x1){
      str.Target1_Y = (((l_dataY & 0x7F) << 8) + h_dataY); 
    }
    else {
      str.Target1_Y = 0 - (((l_dataY & 0x7F) << 8) + h_dataY); 
    }
    
    float Target1_tan = float(str.Target1_X) / float(str.Target1_Y);

    int i = 0;
    while (tanAngle[i] < abs(Target1_tan)) {
      i = i+1;
    }
    if (Target1_tan >= 0){
      str.Target1_angle = i * 2;  
    }
    else {
      str.Target1_angle = 0 - (i * 2);
    }
   }
  else {
    return;
  }
   return str;
   findheader = 0; // в конце сбрасывая флаг найденного заголовка
  } 
}

void setup() {
  //Position str;
  myservo1.attach(9);
  myservo1.write(90);
  Serial.begin(250000); // Init console
  unsigned long previousMillis = millis();
  Serial1.begin(256000); 
}

void loop() {  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < INTERVAL)
  return;
  Position str;
  str = GetCoord();
  //Serial.println(str.Target1_X);
  //Serial.print(" ");

  Serial.print(str.Target1_X);
  Serial.print(", ");
  Serial.print(str.Target1_Y);
  Serial.print(", ");
  Serial.println(90+str.Target1_angle);
  myservo1.write(90+str.Target1_angle);
  while (Serial1.available()) Serial1.read();
  previousMillis = currentMillis;
   
}
