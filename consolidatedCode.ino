#include "SparkFunLSM6DS3.h"
#include "Wire.h"
#include "SPI.h"
//This file just deals with the accelerometer and related info, I have not gotten to looking at the other functions but I will soon. Let me know if there are any errors
LSM6DS3 myIMU;
float jerksX [10];    //arrays for most recent 10 jerk values
float jerksY [10];
float jerksZ [10];
float lastX = 0;      //floats for current and last acceleration value
float lastY = 0;
float lastZ = 0;
float currentX = 0;
float currentY = 0;
float currentZ = 0;
float jerkThreshold = 0.2;  //jerk value to count as jerk, adjust for lower sensitivity
float jerkCount = 5;  //number of jerks in period to count as jerk, needed to rule out normal acceleration
//period = ammount of time in between jerk readings. This is the rate determining step of the void loop
//so every 5th loop we will output binary for jerk and float for temp and humidity
//loopCount = number of loops and increments each loop, if on the 5th loop it will reset and output
int period = 100;
int loopCount = 0;

//weights of readings when averaging
//dht has a period of 2 seconds, lsm has a period of 0.1 seconds, JAMZ wants output every 0.5 seconds
//so, there will be 5 lsm readings and 2 dht readings per output, 0.3 + 0.3 + 5*0.08 = 1
//the same dht readings will be used for 4 consecutive outputs
float dhtWeight = 0.3;
float lsmWeight = 0.08;

#include <DHT.h>

#define DHTPIN1 2
#define DHTPIN2 3


DHT dht[] = {
  {DHTPIN1, DHT11},  //declare DHT's
  {DHTPIN2, DHT11},  //Switch to DHT22 for final
};

// DHT dht1(DHTPIN1, DHTTYPE);
// DHT dht2(DHTPIN2, DHTTYPE);

float humidity[2];        //arrays for temp and humidity values
float temperature[2];
float avgTemperature = 0.0;   //average values
float avgHumidity = 0.0;


void setup() {
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) { //populate array with zeroes to avoid nullpointer
    jerksX [i] = 0;
    jerksY [i] = 0;
    jerksZ [i] = 0;
  }
  Serial.begin(9600);
  int test = sizeof(jerksX) / sizeof(float);
  Serial.println(test);
  myIMU.begin();//initialize accelerometer
  for (auto& sensor : dht) {//initialize dht's
    sensor.begin();
  }
}

void loop() {
  //
  //
  //
  //Jerk Part of Code
  //Author: Noah Aynalem
  //Description: Makes an array of jerk values and if a certain # exceed the threshold a violent shake is detected
  //
  //
  //
  lastX = currentX;//floats for current and last acceleration value reset
  lastY = currentY;
  lastZ = currentZ;
  currentX = myIMU.readFloatAccelX();
  currentY = myIMU.readFloatAccelY();
  currentZ = myIMU.readFloatAccelZ();
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) {//shifts all jerk readings in array down one position
    if (i != 9) {
      jerksX [i] = jerksX[i + 1];
      jerksY [i] = jerksY[i + 1];
      jerksZ [i] = jerksZ[i + 1];
    }
    else {
      jerksX [i] = currentX - lastX;//add news reading to end of array
      jerksY [i] = currentY - lastY;
      jerksZ [i] = currentZ - lastZ;
    }
  }
  int count = 0;//count the # of jerks in the last 10 readings
  for (int i = 0; i < sizeof(jerksX) / sizeof(float); i++) {
    if (jerksX [9] > jerkThreshold || jerksX [9] > jerkThreshold || jerksX [9] > jerkThreshold) {
      count ++;
    }
  }
  //
  //
  //
  //Temperature and Humidity part of Code
  //Author: Rakshita Mathur
  //Description: Reads temperature and humidity from both dht's and lsm, then calculates a weighted average of all 3
  //
  //
  //
  avgTemperature += myIMU.readTempC()*lsmWeight;
  
  if (loopCount == 4) {//loop 4 (output loop)
    for (int i = 0; i < 2; i++) {
      temperature[i] = dht[i].readTemperature();
      humidity[i] = dht[i].readHumidity();
    }
    for (int i = 0; i < 2; i++) {
      avgTemperature += temperature[i] * dhtWeight;
      avgHumidity = avgHumidity + humidity[i]*0.5;
    }
    if (count > jerkCount) {//output
      char mystr[] = "1";
      Serial.write(mystr, 1);
      Serial.println("Jerk");
    }
    else {
      char mystr[] = "0";
      Serial.write(mystr, 1);
      Serial.println("No Jerk");
    }
    Serial.println("Temp");
    Serial.println(avgTemperature);
    Serial.println("Humidity");
    Serial.println(avgHumidity);
    avgTemperature = 0.0;
    avgHumidity = 0.0;
    loopCount = 0;
  } else {//loops 0-3
    loopCount++;
  }
  delay(period);//delay between readings

}
