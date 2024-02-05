#include <dht.h>
#include <LiquidCrystal_I2C.h>
#include <string.h>
dht DHT;
int tempType = 0;
#define DHT11PIN 2
#define AOUT_PIN A0
const int waterState = 3; 
const int pumpPin = 4;
const int prPIN = A1;
const int mThreshold = 350;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  pinMode(waterState, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(prPIN, INPUT);
  Serial.begin(9600);
  lcd.init();
  delay(1000);
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.backlight();
}

void loop() {
  float structData = DHT.read11(DHT11PIN);
  float humidity = DHT.humidity;
  float temperature = DHT.temperature;
  float PRreading = analogRead(prPIN);
  int soilMoisture = analogRead(AOUT_PIN);
  lcd.clear();
  lcdHumidity(humidity);
  lcdTemp(temperature);
  delay(1000);
  checkMoisture(soilMoisture);
  lcd.clear();
  lcdSM(soilMoisture);
  lcdLight(PRreading);
  delay(1000);
  
}

void lcdHumidity(int humidityInput){
  lcd.setCursor(0, 1); 
  lcd.print("Humidity: ");
  lcd.print(humidityInput);
  lcd.print("%");
  Serial.print("Humidity: ");
  Serial.print(humidityInput);
  Serial.println("%");
}

void lcdTemp(int tempInput){
  lcd.setCursor(0, 0); 
  lcd.print("Temperature: ");
  lcd.print(tempInput);
  lcd.print((char)223);
  lcd.print("C");
  Serial.print("Temp: ");
  Serial.println(tempInput);
}

void lcdSM(int smInput){
  lcd.setCursor(0, 0); 
  lcd.print("Moisture: ");
  lcd.print(smInput);
  Serial.print("Moisture: ");
  Serial.println(smInput);
}

void lcdLight(int lightInput){
  String lightState = "Blinding";
  lcd.setCursor(0,1);
  lcd.print("Light: ");
  if(lightInput >= 300){
    lightState = "Luminous";
  }
  if(lightInput >= 500){
    lightState = "Bright";
  }
  if(lightInput >= 600){
    lightState = "Moderate";
  }
  if(lightInput >= 700){
    lightState = "Dim";
  }
  if(lightInput >= 950){
    lightState = "Dark";
  }
   
  lcd.print(lightState);
  Serial.print("Light: ");
  Serial.println(lightInput);
  Serial.println(lightState);
}

void checkMoisture(int smInput){
  if(smInput > mThreshold){
    digitalWrite(waterState, HIGH);
    pumpState("ON");
  }
  else{
    digitalWrite(waterState,LOW);
    pumpState("OFF");
  }
}

void pumpState(String state){
  if(state == "ON"){
    digitalWrite(pumpPin,HIGH);
    Serial.println("PUMP ON");

  }
  else{
    digitalWrite(pumpPin,LOW);
    Serial.println("PUMP OFF");
  }
}