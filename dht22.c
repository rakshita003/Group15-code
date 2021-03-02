#include <DHT.h>;

#define DHTPIN1 2
#define DHTPIN2 3


DHT dht[] = {
  {DHTPIN1, DHT22},
  {DHTPIN2, DHT22},
  };

// DHT dht1(DHTPIN1, DHTTYPE);
// DHT dht2(DHTPIN2, DHTTYPE);

float humidity[2];
float temperature[2];
float avgTemperature = 0.0;
float avgHumidity = 0.0;

void setup()
{ 
  Serial.begin(9600);
// for(byte i = 0; i < sizeof(dht)/sizeof(dht[0]; i++)
// {
//    dht[i].begin();
// }
    
  for (auto& sensor : dht) {
    sensor.begin();
  }
}

void loop()
{
  for (int i = 0; i < 2; i++) {
    temperature[i] = dht[i].readTemperature();
    humidity[i] = dht[i].readHumidity();
  }
  //average
  for(int i = 0; i < 2; i++){
      avgTemperature += temperature[i];
      avgHumidity += Humidity[i];
  }

  for (int i = 0; i < 2; i++) {
    Serial.print(F("Temperature "));
    Serial.print(i);
    Serial.println(temperature[i]);
    Serial.print(F("Humidity "));
    Serial.print(i);
    Serial.println(humidity[i]);
  }

    avgTemperature /=2;
    Serial.print(F("Average Temperature "));
    Serial.println(avgTemperature);
  
    avgHumidity /=2;
    Serial.print(F("Average Humidity "));
    Serial.println(avgHumidity);
  
    delay(10000); //delay 2 sec.
}
