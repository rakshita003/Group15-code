#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
//This file just deals with the accelerometer and related info, I have not gotten to looking at the other functions but I will soon. Let me know if there are any errors
LSM6DS3 myIMU;
float jerksX [10];//arrays for most recent 10 jerk values
float jerksY [10];
float jerksZ [10];
float lastX = 0;//floats for current and last acceleration value
float lastY = 0;
float lastZ = 0;
float currentX = 0;
float currentY = 0;
float currentZ = 0;
float jerkThreshold = 0.2;
float jerkCount = 3;
void setup() {
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) { //populate array with zeroes to avoid nullpointer
    jerksX [i] = 0;
    jerksY [i] = 0;
    jerksZ [i] = 0;
  }
  Serial.begin(9600);
  myIMU.begin();
}

void loop() {
  lastX = currentX;//floats for current and last acceleration value
  lastY = currentY;
  lastZ = currentZ;
  currentX = myIMU.readFloatAccelX();
  currentY = myIMU.readFloatAccelY();
  currentZ = myIMU.readFloatAccelZ();
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) { //populate array with zeroes to avoid nullpointer
    if (i != 9) {
      jerksX [i] = jerksX[i + 1];
      jerksY [i] = jerksY[i + 1];
      jerksZ [i] = jerksZ[i + 1];
    }
    else {
      jerksX [i] = currentX - lastX;
      jerksY [i] = currentY - lastY;
      jerksZ [i] = currentZ - lastZ;
    }
  }
  int count = 0;
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) {
    if (jerksX [i] > jerkThreshold || jerksX [i] > jerkThreshold || jerksX [i] > jerkThreshold) {
      count ++;
    }
  }
  if (count > jerkCount) {
    Serial.println("Jerk");
  }else{
    Serial.println(count);
  }
  delay(100);
}
