#include <Arduino_LSM6DS3.h>
#include <SparkFunLSM6DS3.h>
#include <Wire.h>
#include <SPI.h>
LSM6DS3 SensorOne( I2C_MODE, 0x6A );
//This file just deals with the accelerometer and related info, I have not gotten to looking at the other functions but I will soon. Let me know if there are any errors
void setup() {
  Serial.begin(9600);
//   while(!Serial);

//   if(!IMU.begin()){
//     Serial.println("Failed to initialize IMU!");

//     while(1);
//   }
//   Serial.print("Accelerometer sample rate = ");
//   Serial.print(IMU.accelerationSampleRate());
//   Serial.println(" Hz");
//   Serial.println();
//   Serial.println("Acceleration in G's");
//   Serial.println("X\tY\tZ");
  Serial.println("Processor came out of reset.\n");
  
  //Call .begin() to configure the IMUs
  if( SensorOne.begin() != 0 )
  {
	  Serial.println("Problem starting the sensor at 0x6A.");
  }
  else
  {
	  Serial.println("Sensor at 0x6A started.");
  }

}

void loop() {
//   float x, y, z;
  float ax [10];
  float ay[10];
  float az[10];
  double thedelay = 0.02;
  double thedelayms = thedelay*1000;
  for(int i = 0;i<sizeof(ax);i++){
//     IMU.readAcceleration(x,y,z);
    ax[i] = SensorOne.readFloatAccelX()*9.8;
    ay[i] = SensorOne.readFloatAccelY()*9.8;
    az[i] = SensorOne.readFloatAccelZ()*9.8;//not sure whether we have to add or subtract one from the z value, feel free to change if necessary
    delay(thedelayms);//the period of the lsm6ds3 is 9.61 milliseconds so this delay is more than enough time
    //I arbitrarily chose 10 measurements, it takes in total about 1/5 of a second to do(assuming the delays take up >99% of the time), so not too long. You can mess with the number of measurements as you see fit
  }
  float jerkx [sizeof(ax)];
  float jerky [sizeof(ay)];
  float jerkz [sizeof(az)];
  for(int i = 0;i<sizeof(jerkx);i++){
    if(i+1<sizeof(jerkx)){
    jerkx[i] = (ax[i+1]-ax[i])/thedelay;
    jerky[i] = (ay[i+1]-ay[i])/thedelay;
    jerkz[i] = (az[i+1]-az[i])/thedelay;
    }else{
    jerkx[i] = (ax[i]-ax[i-1])/thedelay;
    jerky[i] = (ay[i]-ay[i-1])/thedelay;
    jerkz[i] = (az[i]-az[i-1])/thedelay;
    }
    
  }
  for(int i = 0;i<sizeof(jerkx);i++){
    Serial.println("Jerk in the x-axis(m/s^3):");
    Serial.print(jerkx[i]);
    Serial.println();
    Serial.println("Jerk in the y-axis(m/s^3):");
    Serial.print(jerky[i]);
    Serial.println();
    Serial.println("Jerk in the z-axis(m/s^3):");
    Serial.print(jerkz[i]);
  }
  //In order for anything else to work, this code above must produce accurate results. Try this, and if it works uncomment some of the code under here
  //I believe that success means that numbers should mostly be around 0, unless you start shaking the sensor. Unless acceleration is linear or faster, jerk should be low. 
//  float dx[sizeof(jerkx)];
//  float dy[sizeof(jerky)];
//  float dz[sizeof(jerkz)];
//  for(int i = 0;i<sizeof(jerkx);i++){  
//    dx[i] = ax[i]*thedelay*thedelay;//acceleration multiplied by time once gives velocity, and velocity multiplied by time gives position. acceleration*time = velocity and velocity*time = position --> position = acceleration*time^2
//    dy[i] = ay[i]*thedelay*thedelay;//the sum of the dx, dy and dz arrays is the total distance travelled over the 200ms time frame, or displacement.
//    dz[i] = az[i]*thedelay*thedelay;
//  }
//  float vx[sizeof(jerkx)];
//  float vy[sizeof(jerky)];
//  float vz[sizeof(jerkz)];//here is the same concept applied for the velocity. The value returned here is change in velocity over 200ms
//  for(int i = 0;i<sizeof(jerkx);i++){
//    vx[i] = ax[i]*thedelay;
//    vy[i] = ay[i]*thedelay;
//    vz[i] = az[i]*thedelay;
//  }
//  float displacementx,displacementy,displacementz;
//  float deltavx,deltavy,deltavz;//here are the final values of the displacement in x, y and z along with the change in velocities
//  for(int i = 0;i<sizeof(jerkx);i++){
//    displacementx = displacementx+dx[i];
//    displacementy = displacementy+dy[i];
//    displacementz = displacementz+dz[i];
//    deltavx = deltavx+vx[i];
//    deltavy = deltavy+vy[i];
//    deltavz = deltavz+vz[i];
//  }
//  for(int i = 0;i<10;i++){
//    String temps = String((i+1)*thedelay);
//    Serial.print("After ");
//    Serial.print(temps);
//    Serial.print(" milliseconds");
//    Serial.println("Displacement in the x-axis:");
//    Serial.print(dx[i]);
//    Serial.println("Change in velocity in the x-axis: ");
//    Serial.print(vx[i]);
//    Serial.println();
//    Serial.println("Displacement in the y-axis:");
//    Serial.print(dy[i]);
//    Serial.println("Change in velocity in the y-axis: ");
//    Serial.print(vy[i]);
//    Serial.println();
//    Serial.println("Displacement in the z-axis:");
//    Serial.print(dz[i]);
//    Serial.println("Change in velocity in the x-axis: ");
//    Serial.print(vz[i]);
//    
//  }
}
