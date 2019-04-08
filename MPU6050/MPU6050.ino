#include <Wire.h>
#define CAL_TIMES (float)2000

const int MPU=0x68;//MPU6050 I2C주소

int16_t AX, AY, AZ;
int64_t sumX = 0, sumY = 0, sumZ = 0;
float calX, calY, calZ;
float AX2, AY2, AZ2;
float Tmp;

int led = 7;

void getData();

void setup()
{
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);//MPU6050 을 동작 대기 모드로 변경
  Wire.endTransmission(true);
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);

  Serial.print("Calibrating.");

  for(int i=1; i<=CAL_TIMES; i++)
  {
    if(i % (int)(CAL_TIMES / 10) == 0) Serial.print('.');
    getData();
    sumX += AX;
    sumY += AY;
    sumZ += AZ;
  }

  Serial.println("Done");

  calX = sumX / CAL_TIMES;
  calY = sumY / CAL_TIMES;
  calZ = sumZ / CAL_TIMES;

  digitalWrite(led, LOW);
  
  Serial.print("calX : ");
  Serial.print(calX);
  Serial.print("\t calY : ");
  Serial.print(calY);
  Serial.print("\t calZ : ");
  Serial.println(calZ);

  delay(2000);
}

void loop()
{
  getData();

  AX2 = (((AX - calX) * 2.0) / 32768) * 9.8;
  AY2 = (((AY - calY)* 2.0) / 32768) * 9.8;
  AZ2 = (((AZ - calZ)* 2.0) / 32768) * 9.8;
/*
  Serial.print("AX  : ");
  Serial.print(AX);
  Serial.print("\t AY  : ");
  Serial.print(AY);
  Serial.print("\t AZ  : ");
  Serial.print(AZ);
*/
  Serial.print("X : ");
  if(AX2 >= 0) Serial.print(' ');
  Serial.print(AX2);
  
  Serial.print("m/s\tY : ");
  if(AY2 >= 0) Serial.print(' ');
  Serial.print(AY2);
  
  Serial.print("m/s\tZ : ");
  if(AZ2 >= 0) Serial.print(' ');
  Serial.print(AZ2);
  Serial.println("m/s");
}

void getData()
{
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);
  
  AX = Wire.read()<<8 | Wire.read();
  AY = Wire.read()<<8 | Wire.read();
  AZ = Wire.read()<<8 | Wire.read();
  
  delay(1);
}
