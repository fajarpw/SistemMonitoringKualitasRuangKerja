#include "DHT.h"
 
 #define sensorLDR A1
 int nilaiSensor;

#define DHTPIN A0
#define DHTTYPE DHT11

#define RED_LED 9
#define BLUE_LED 11
 
DHT dht(DHTPIN, DHTTYPE);

float dataCelcius, dataHumidity;
int dataLightIntensity;
bool isTempGood, isHumidityGood, isLightIntensityGood, isAirQualityGood, isRoomQualityGood;

void setup() {
  Serial.begin(9600);
  Serial.println("======================================");
  Serial.println("Sistem Monitoring Kualitas Ruang Kerja");
  Serial.println("======================================");
  dht.begin();
}

void loop() {
  delay(2000);
  ReadFromDHT11();
  ReadFromLDR();
  ClasifyRoomQuality();
}

void ReadFromDHT11(){
  float humidity = dht.readHumidity();
  float tempCelcius = dht.readTemperature();
  float tempFahrenheit = dht.readTemperature(true);

  if (isnan(humidity) || isnan(tempCelcius) || isnan(tempFahrenheit))
  {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }

  float hif = dht.computeHeatIndex(tempFahrenheit, humidity);
  float hic = dht.computeHeatIndex(tempCelcius, humidity, false);

  dataCelcius = tempCelcius;
  dataHumidity = humidity;

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(tempCelcius);
  Serial.println(F("°C "));
}

void ReadFromLDR(){
 nilaiSensor = analogRead(sensorLDR);
 Serial.print("Nilai Sensor : ");
 Serial.println(nilaiSensor);
 delay(1000);
}

void ClasifyTempQuality(){
  if(dataCelcius >=23 && dataCelcius <=26){
    isTempGood = true;
  }else if(dataCelcius < 23 || dataCelcius > 26){
    isTempGood = false;
  }else{
   Serial.println("Fail to clasify temp data");
  }
}
 void ClasifyHumidityQuality(){
  if(dataHumidity >=40 && dataHumidity <=60){
    isHumidityGood = true;
  }else if(dataHumidity < 40 || dataHumidity > 60){
    isHumidityGood = false;
  }else{
   Serial.println("Fail to clasify humidity data");
  }
 }

void ClasifyAirQuality(){
  ClasifyTempQuality();
  ClasifyHumidityQuality();
  if(isTempGood && isHumidityGood){
    isAirQualityGood = true;
  }else{
    isAirQualityGood = false;
  }
}
 
void ClasifyLightIntensityQuality(){
  if(dataLightIntensity >=300 && dataLightIntensity <=500){
    isLightIntensityGood = true;
  }else if(dataLightIntensity < 300 || dataLightIntensity > 500){
    isLightIntensityGood = false;
  }else{
   Serial.println("Fail to clasify light intensity data");
  }
}

void ClasifyRoomQuality(){
  ClasifyAirQuality();
  ClasifyLightIntensityQuality();
  
  if(isLightIntensityGood && isAirQualityGood){
    isRoomQualityGood = true;
    Serial.println("Status Kualitas: Baik");
  }else if(isLightIntensityGood == false || isAirQualityGood == false){
    isRoomQualityGood = false;
    Serial.println("Status Kualitas: Tidak Baik");
  }else {
      Serial.println("Fail to clasify room data");
  }
}
