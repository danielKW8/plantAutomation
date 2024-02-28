#include <LiquidCrystal_I2C.h>

#include <dht.h>
#include <string.h>
dht DHT;
#define DHT11PIN 2
#define AOUT_PIN A0
const int pumpSwitch = 3; 
const int pumpPin = 4;
const int prPIN = A5;
const int mThreshold = 350;
const int pumpSet = 5;
const int pumpLED = 9;
const int waterState = 6;
int buttonValue = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino On");
  pinMode(waterState, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(prPIN, INPUT);
  pinMode(pumpSet, INPUT);
  pinMode(pumpLED,OUTPUT);
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
  Serial.println(digitalRead(pumpSet));
  Serial.println(buttonValue);
  if(digitalRead(pumpSet) == HIGH){
    buttonValue += 1;
  }
  if(buttonValue % 2 != 0){
    digitalWrite(pumpLED, HIGH);
    Serial.println("PUMP LED ON");
  }
  else{
    digitalWrite(pumpLED, LOW);
  }
  checkMoisture(soilMoisture);
  lcd.clear();
  lcdSM(soilMoisture);
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

/*
void lcdLight(int lightInput){
  String lightState = "Blinding";
  //lcd.setCursor(0,1);
  //lcd.print("Light: ");
  if(lightInput >= 400){
    lightState = "Luminous";
  }
  if(lightInput >= 600){
    lightState = "Bright";
  }
  if(lightInput >= 700){
    lightState = "Moderate";
  }
  if(lightInput >= 800){
    lightState = "Dim";
  }
  if(lightInput >= 900){
    lightState = "Dark";
  }
   
  //lcd.print(lightState);
  Serial.print("Light: ");
  Serial.println(lightInput);
  Serial.println(lightState);
}
*/

void checkMoisture(int smInput){
  if(smInput > mThreshold && buttonValue % 2 != 0){
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
