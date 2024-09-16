#include <Adafruit_Sensor.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <Servo.h>

#define DHTPIN 8 // Remove 'D' if using Uno
#define DHTTYPE DHT11
#define RELAY1 3 // FAN // Remove 'D' if using Uno
#define RELAY2 5 // LIGHT // Remove 'D' if using Uno
#define TRIGGER_PIN   10
#define ECHO_PIN      9
#define MAX_DISTANCE 200

int powerPin = 6;
const int sensor_hujan1 = 12;
const int sensor_hujan2 = 13;
const int pompa = 11;

LiquidCrystal_I2C lcd(0x3f,16,2); // Can also be 0x3f or 0x27
DHT dht(DHTPIN, DHTTYPE);
Servo servoAne;
NewPing ultrasonic(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup(){
  Serial.begin(9600 ); // Change to 9600 if using Arduino Uno, 115200 if using Wemos
  lcd.begin();
  dht.begin();
  servoAne.attach(6);
  servoAne.write(0);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  pinMode(sensor_hujan1, INPUT);
  pinMode(sensor_hujan2, INPUT);
  pinMode(pompa, OUTPUT);
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  //digitalWrite(RELAY1, HIGH);
  //digitalWrite(RELAY2, HIGH);
}

void loop(){
  int H = dht.readHumidity();
  int T = dht.readTemperature(); // Temperature in Celsius
  int BA1 = digitalRead(sensor_hujan1);
  int BA2 = digitalRead(sensor_hujan2);
  int MK = ultrasonic.ping_cm();

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(H);
  lcd.print("%");

  Serial.print(H);
  Serial.println("%");

  lcd.setCursor(0,1);
  lcd.print("Suhu: ");
  lcd.print(T);
  lcd.print("C");

  Serial.print("Suhu: ");
  Serial.print(T);
  Serial.println("C");

  Serial.print("Hasil Sensor: ");
  Serial.print(MK);
  Serial.println(" cm");

  // Control relay for light
  if(T < 13){
    digitalWrite(RELAY2, LOW); // Turn on light
  }
  else if(T >= 16){
    digitalWrite(RELAY2, HIGH); // Turn off light
  }

  // Control relay for fan
  if(T <= 23){
    digitalWrite(RELAY1, HIGH); // Turn on fan
  }
  else if(T > 26){
    digitalWrite(RELAY1, LOW); // Turn off fan
  }

  // Control pump based on rain sensors
  if (BA1 == HIGH && BA2 == HIGH){
    digitalWrite(pompa, HIGH); // Turn on pump
  }
  else if (BA1 == LOW && BA2 == HIGH){
    digitalWrite(pompa, HIGH); // Turn on pump
  }
  else if (BA1 == LOW && BA2 == LOW){
    digitalWrite(pompa, LOW); // Turn off pump
  }

  // Control servo based on distance
  if(MK <= 5){
    servoAne.write(180); // Open
  }
  else{
    servoAne.write(0); // Close
  }
  delay(500); // Adjust delay as needed
}
