int flexSensorReading[2];
int IMUReading[2];
int initFlexSensor[2];
int initIMUReading[4];
int location[4];

//Libraries
#include <Wire.h>
#include <TimerOne.h>

//IMU parameters
#define MPU9250_ADDRESS 0x68
#define MAG_ADDRESS 0x0C
 
#define GYRO_FULL_SCALE_250_DPS 0x00 
#define GYRO_FULL_SCALE_500_DPS 0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18
 
#define ACC_FULL_SCALE_2_G 0x00 
#define ACC_FULL_SCALE_4_G 0x08
#define ACC_FULL_SCALE_8_G 0x10
#define ACC_FULL_SCALE_16_G 0x18

long int ti;
volatile bool intFlag=false;
long int cpt=0;

void setup() {
  Serial.begin(9600);  
  initiateFlexSensor();
  initiateIMU();
}

void loop() {
  updateFlexSensor();
  updateIMU();
  updateLocation();
  sendData();
  delay(100);
}

void callback(){
  intFlag=true;
  digitalWrite(13, digitalRead(13) ^ 1);
}

void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data){
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.endTransmission();
  Wire.requestFrom(Address, Nbytes);
  uint8_t index=0;
  while (Wire.available())
    Data[index++]=Wire.read();
}
 
// Set register address
void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data){
  Wire.beginTransmission(Address);
  Wire.write(Register);
  Wire.write(Data);
  Wire.endTransmission();
}

// Update IMU location
void updateLocation(){
  for(int i = 0;i<2;i++){
    location[i] = IMUReading[i] - initIMUReading[i];
  }
}
 
void initiateIMU(){ //make imu initiator
  Wire.begin();

  // Set accelerometers low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,29,0x06);

  // Set gyroscope low pass filter at 5Hz
  I2CwriteByte(MPU9250_ADDRESS,26,0x06);

  // Configure gyroscope range
  I2CwriteByte(MPU9250_ADDRESS,27,GYRO_FULL_SCALE_1000_DPS);
  // Configure accelerometers range
  I2CwriteByte(MPU9250_ADDRESS,28,ACC_FULL_SCALE_4_G);
  // Set by pass mode for the magnetometers
  I2CwriteByte(MPU9250_ADDRESS,0x37,0x02);

  // Request continuous magnetometer measurements in 16 bits
  I2CwriteByte(MAG_ADDRESS,0x0A,0x16);
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  Timer1.initialize(10000); // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback); // attaches callback() as a timer overflow interrupt
  // Store initial time
  ti=millis();
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  initIMUReading[0] = ax;
  initIMUReading[1] = ay;
}

void updateIMU(){
  uint8_t Buf[14];
  I2Cread(MPU9250_ADDRESS,0x3B,14,Buf);
  int16_t ax=-(Buf[0]<<8 | Buf[1]);
  int16_t ay=-(Buf[2]<<8 | Buf[3]);
  int16_t gx=-(Buf[8]<<8 | Buf[9]);
  int16_t gy=-(Buf[10]<<8 | Buf[11]);
  IMUReading[0] = ax;
  IMUReading[1] = ay;
}

void initiateFlexSensor(){  //find initial value of flex sensors
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  initFlexSensor[0] = analogRead(A0);
  initFlexSensor[1] = analogRead(A1);
}

void updateFlexSensor(){  //update flex sensor readings
  flexSensorReading[0] = abs(analogRead(A0) - initFlexSensor[0]);
  flexSensorReading[1] = abs(analogRead(A1) - initFlexSensor[1]);
}

void arrayString(int x[]){
  for(int i = 0; i < 2; i++){
    Serial.print(x[i]);
    Serial.print(" ");
  }
}

void sendData(){
  Serial.print("<");
  arrayString(flexSensorReading);
  arrayString(location);
  Serial.println(">");
  
}
