#include <Arduino.h>
#include <Wire.h>

#define MPU6050_ADDR         0x68 // MPU-6050 device address
#define MPU6050_SMPLRT_DIV   0x19 // MPU-6050 register address
#define MPU6050_CONFIG       0x1a
#define MPU6050_GYRO_CONFIG  0x1b
#define MPU6050_ACCEL_CONFIG 0x1c
#define MPU6050_WHO_AM_I     0x75
#define MPU6050_PWR_MGMT_1   0x6b

float elapsedTime, currentTime, timePrev;        //Variables for time control
double offsetX = 0, offsetY = 0, offsetZ = 0;
float acc_x, acc_y, acc_z;
float dpsX, dpsY, dpsZ;

float rad_to_deg = 180 / 3.141592654;    //This value is for pasing from radians to degrees values
float Gyro_angle_x, Gyro_angle_y;         //Here we store the angle value obtained with Gyro data
float Acc_angle_x, Acc_angle_y;          //Here we store the angle value obtained with Acc data
float Acc_angle_error_x, Acc_angle_error_y; //Here we store the initial Acc data error

float Total_angle_x, Total_angle_y;
int total_angle_x_int, total_angle_y_int;

const int freq = 50;
const int channel1 = 0;
const int channel2 = 1;
const int channel3 = 2;
const int channel4 = 3;
const int resolution = 11;
const int motorPin1 = 25;
const int motorPin2 = 26;
const int motorPin3 = 27;
const int motorPin4 = 14;
const int multiplier = 101;

int motorFLVal;
int motorFRVal;
int motorBLVal;
int motorBRVal;


void writeMPU6050(byte reg, byte data) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

byte readMPU6050(byte reg) {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(reg);
  Wire.endTransmission(true);
  Wire.requestFrom(MPU6050_ADDR, 1/*length*/);
  byte data =  Wire.read();
  return data;
}

void printData() {

  //  Serial.print("Gyro (deg)");
  //  Serial.print(" X=");
  //  Serial.print(dpsX);
  //  Serial.print(" Y=");
  //  Serial.print(dpsY);
  //  Serial.print(" Z=");
  //  Serial.println(dpsZ);
  //  Serial.print(" Accel (g)");
  //  Serial.print(" X=");
  //  Serial.print(acc_x);
  //  Serial.print(" Y=");
  //  Serial.print(acc_y);
  //  Serial.print(" Z=");
  //  Serial.println(acc_z);
  
  Serial.print("Xº: ");
  Serial.print(total_angle_x_int);
  Serial.print("   |   ");
  Serial.print("Yº: ");
  Serial.print(total_angle_y_int+3);
  Serial.println(" ");
}

void setupMpu() {
  Wire.begin(21, 22);

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  if (readMPU6050(MPU6050_WHO_AM_I) != 0x68) {
    Serial.println("\nWHO_AM_I error.");
    while (true) ;
  }

  writeMPU6050(MPU6050_SMPLRT_DIV, 0x00);   // sample rate: 8kHz/(7+1) = 1kHz
  writeMPU6050(MPU6050_CONFIG, 0x00);       // disable DLPF, gyro output rate = 8kHz
  writeMPU6050(MPU6050_GYRO_CONFIG, 0x08);  // gyro range: ±500dps
  writeMPU6050(MPU6050_ACCEL_CONFIG, 0x00); // accel range: ±2g
  writeMPU6050(MPU6050_PWR_MGMT_1, 0x01);   // disable sleep mode, PLL with X gyro

  Serial.print("Calculate Calibration");
  for (int i = 0; i < 3000; i++) {

    int16_t raw_acc_x, raw_acc_y, raw_acc_z, raw_t, raw_gyro_x, raw_gyro_y, raw_gyro_z ;

    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 14, true);

    raw_acc_x = Wire.read() << 8 | Wire.read();
    raw_acc_y = Wire.read() << 8 | Wire.read();
    raw_acc_z = Wire.read() << 8 | Wire.read();

    raw_t = Wire.read() << 8 | Wire.read();

    raw_gyro_x = Wire.read() << 8 | Wire.read();
    raw_gyro_y = Wire.read() << 8 | Wire.read();
    raw_gyro_z = Wire.read() << 8 | Wire.read();

    dpsX = ((float)raw_gyro_x) / 65.5;
    dpsY = ((float)raw_gyro_y) / 65.5;
    dpsZ = ((float)raw_gyro_z) / 65.5;

    offsetX += dpsX;
    offsetY += dpsY;
    offsetZ += dpsZ;

    acc_x = ((float)raw_acc_x) / 16384.0;
    acc_y = ((float)raw_acc_y) / 16384.0;
    acc_z = ((float)raw_acc_z) / 16384.0;

    /*---X---*/
    Acc_angle_error_x = Acc_angle_error_x + ((atan((acc_y) / sqrt(pow((acc_x), 2) + pow((acc_z), 2))) * rad_to_deg));
    /*---Y---*/
    Acc_angle_error_y = Acc_angle_error_y + ((atan(-1 * (acc_x) / sqrt(pow((raw_acc_y), 2) + pow((acc_z), 2))) * rad_to_deg));

    if (i % 1000 == 0) {
      Serial.print(".");
    }
  }
  Serial.println();

  offsetX /= 3000;
  offsetY /= 3000;
  offsetZ /= 3000;

  Acc_angle_error_x = Acc_angle_error_x / 3000;
  Acc_angle_error_y = Acc_angle_error_y / 3000;

  Serial.print("offsetX : ");
  Serial.println(offsetX);
  Serial.print("offsetY : ");
  Serial.println(offsetY);
  Serial.print("offsetZ : ");
  Serial.println(offsetZ);
}

void calcRotation() {

  int16_t raw_acc_x, raw_acc_y, raw_acc_z, raw_t, raw_gyro_x, raw_gyro_y, raw_gyro_z ;

  timePrev = currentTime;                        // the previous time is stored before the actual time read
  currentTime = millis();                        // actual time read
  elapsedTime = (currentTime - timePrev) / 1000; //divide by 1000 in order to obtain seconds

  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);

  raw_acc_x = Wire.read() << 8 | Wire.read();
  raw_acc_y = Wire.read() << 8 | Wire.read();
  raw_acc_z = Wire.read() << 8 | Wire.read();
  raw_t = Wire.read() << 8 | Wire.read();
  raw_gyro_x = Wire.read() << 8 | Wire.read();
  raw_gyro_y = Wire.read() << 8 | Wire.read();
  raw_gyro_z = Wire.read() << 8 | Wire.read();

  acc_x = ((float)raw_acc_x) / 16384.0;
  acc_y = ((float)raw_acc_y) / 16384.0;
  acc_z = ((float)raw_acc_z) / 16384.0;

  dpsX = ((float)raw_gyro_x) / 65.5; // LSB sensitivity: 65.5 LSB/dps @ ±500dps
  dpsY = ((float)raw_gyro_y) / 65.5;
  dpsZ = ((float)raw_gyro_z) / 65.5;

  dpsX = dpsX - offsetX;
  dpsY = dpsY - offsetY;
  dpsZ = dpsZ - offsetZ;

  Gyro_angle_x = dpsX * elapsedTime;
  /*---X---*/
  Gyro_angle_y = dpsY * elapsedTime;

  Acc_angle_x = (atan((acc_y) / sqrt(pow((acc_x), 2) + pow((acc_z), 2))) * rad_to_deg) - Acc_angle_error_x;
  /*---Y---*/
  Acc_angle_y = (atan(-1 * (acc_x) / sqrt(pow((acc_y), 2) + pow((acc_z), 2))) * rad_to_deg) - Acc_angle_error_y;

  //////////////////////////////////////Total angle and filter/////////////////////////////////////
  /*---X axis angle---*/
  Total_angle_x = 0.98 * (Total_angle_x + Gyro_angle_x) + 0.02 * Acc_angle_x;
  /*---Y axis angle---*/
  Total_angle_y = 0.98 * (Total_angle_y + Gyro_angle_y) + 0.02 * Acc_angle_y;

  total_angle_x_int = Total_angle_x;  
  total_angle_y_int = Total_angle_y;
}

void setupMotor(){

  ledcSetup(channel1, freq, resolution);
  ledcSetup(channel2, freq, resolution);
  ledcSetup(channel3, freq, resolution);
  ledcSetup(channel4, freq, resolution);

  ledcAttachPin(motorPin1, channel1);
  ledcAttachPin(motorPin2, channel2);
  ledcAttachPin(motorPin3, channel3);
  ledcAttachPin(motorPin4, channel4);

  motorFLVal = 101;
  motorFRVal = 101;
  motorBLVal = 101;
  motorBRVal = 101;

  ledcWrite(channel1, motorFLVal);
  ledcWrite(channel2, motorFRVal);
  ledcWrite(channel3, motorBLVal);
  ledcWrite(channel4, motorBRVal);
}

void setup() {
  Serial.begin(115200);
  delay(100);
  // setupMotor();
  setupMpu();
}

void loop() {
  calcRotation();
  printData();
}