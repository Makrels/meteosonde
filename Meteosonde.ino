#include "DHT.h"
#include <SD.h>
#include <Wire.h>
#include <MS5611.h>


MS5611 ms5611; 

double referencePressure;
 
#define DHTPIN 2
#define DHTTYPE DHT22

DHT sensor(DHTPIN, DHTTYPE);
unsigned long czas;
const int buzzer = 5;
int ledr = 7;
int ledg = 9;
const int chipSelect = 10;

void setup()

{
Serial.begin(9600);  

sensor.begin(); 
ms5611.begin();
pinMode(ledr, OUTPUT); 
pinMode(ledg, OUTPUT);  
pinMode(buzzer, OUTPUT); 

Serial.println("Inicjalizacja karty micro SD");
if (!SD.begin(chipSelect)) {
Serial.println("Blad karty microSD");
delay(2000);
return;
}

Serial.println("Karta zostala zainicjalizowana");
while(!ms5611.begin(MS5611_HIGH_RES))
  {
    Serial.println("Błąd czujnika MS5611!");
    delay(500);
  }

referencePressure = ms5611.readPressure(); // Pobieramy poczatkowe cisnienie do okreslenia wysokosci
checkSettings(); // Sprawdzamy ustawienia
}

void checkSettings()
{
  //Serial.print("Oversampling: ");    
  //Serial.println(ms5611.getOversampling()); 
}

void loop()

{
    // Odczyt wartosci nieskompensowanych
  double realTemperature = ms5611.readTemperature();
  long realPressure = ms5611.readPressure();
  double realAltitude = ms5611.getAltitude(realPressure);
 
  // Odczyt wartosci skompensowanych
  double realTemperature2 = ms5611.readTemperature(true);
  long realPressure2 = ms5611.readPressure(true);
  double realAltitude2 = ms5611.getAltitude(realPressure2);
 
  // Obliczanie wysokosci
  float absoluteAltitude = ms5611.getAltitude(realPressure);
  float relativeAltitude = ms5611.getAltitude(realPressure, referencePressure);
  
void(* resetFunc) (void) = 0;   
czas = millis();
float humidity = sensor.readHumidity(); 
float temperature_C = sensor.readTemperature();
float temperature_F = sensor.readTemperature(true);
float heat_indexF = sensor.computeHeatIndex(temperature_F, humidity);
float heat_indexC = sensor.convertFtoC(heat_indexF);

float calcDewpoint = log(humidity / 100) + ((17.62 * temperature_C) / (243.12 + temperature_C)); //ADD
float DewPoint = 243.12 * calcDewpoint / (17.62 - calcDewpoint);

File sdcard_file = SD.open("data.txt", FILE_WRITE);  //ADD

if (sdcard_file) {

digitalWrite(ledg, HIGH); 
tone(buzzer, 1500); 
  delay(200);        
  noTone(buzzer);  
  delay(200);      
digitalWrite(ledg,LOW); 
  delay(1000);

sdcard_file.print(czas/1000);
sdcard_file.print(" = ");
sdcard_file.print(temperature_C);
sdcard_file.print(" = ");
sdcard_file.print(realTemperature);
sdcard_file.print(" = ");
sdcard_file.print(realTemperature2);
sdcard_file.print(" = ");
sdcard_file.print(humidity);
sdcard_file.print(" = ");
sdcard_file.print(DewPoint);
sdcard_file.print(" = ");
sdcard_file.print(realPressure);
sdcard_file.print(" = ");
sdcard_file.print(realPressure2);
sdcard_file.print(" = ");
sdcard_file.print(realAltitude2);
sdcard_file.print(" = ");
sdcard_file.print(absoluteAltitude);
sdcard_file.print(" = ");
sdcard_file.println(relativeAltitude);
sdcard_file.close();

Serial.print(czas/1000);
Serial.print(" = ");
Serial.print(temperature_C);
Serial.print(" = ");
Serial.print(realTemperature);
Serial.print(" = ");
Serial.print(realTemperature2);
Serial.print(" = ");
Serial.print(humidity);
Serial.print(" = ");
Serial.print(DewPoint);
Serial.print(" = ");
Serial.print(realPressure);
Serial.print(" = ");
Serial.print(realPressure2);
Serial.print(" = ");
Serial.print(realAltitude2);
Serial.print(" = ");
Serial.print(absoluteAltitude);
Serial.print(" = ");
Serial.println(relativeAltitude);    

}  

else {

Serial.println("Blad otwarcia pliku data.txt");
digitalWrite(ledg,LOW); 
digitalWrite(ledr, HIGH);   
delay(1000);             
digitalWrite(ledr, LOW); 
delay(3000);  
resetFunc();

} 

delay(4000);
return;
}
