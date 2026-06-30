#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <Wire.h>


const char* ssid = "mynameis";
const char* password = "12345678";


#define BOTtoken "8385153740:AAHQscck4iM4IEbMqzHMCogzIXmDaO7UUFg"
#define CHAT_ID "7608797839"


#define REED_PIN   D5
#define BUZZER_PIN D6
#define GREEN_LED  D7
#define RED_LED    D8

const int MPU_ADDR = 0x68;
int16_t ax, ay, az;


WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);


bool alertSent = false;
bool systemArmed = true;  


void handleTelegram() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

  while (numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {

      String text = bot.messages[i].text;

      if (text == "/on") {
        systemArmed = true;
        bot.sendMessage(CHAT_ID, " System ON", "");
      }

      if (text == "/off") {
        systemArmed = false;
        bot.sendMessage(CHAT_ID, " System OFF", "");

        
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
      }
    }
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(REED_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);

 
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");

  client.setInsecure();

  
  Wire.begin(D2, D1);
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  bot.sendMessage(CHAT_ID, " System Started\nUse /on or /off", "");
  Serial.println("Smart Theft System Ready...");
}

void loop() {

  handleTelegram();   

 
  if (!systemArmed) {
    Serial.println("System OFF");
    delay(300);
    return;
  }

  int reedState = digitalRead(REED_PIN);

 
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  ax = Wire.read() << 8 | Wire.read();
  ay = Wire.read() << 8 | Wire.read();
  az = Wire.read() << 8 | Wire.read();

  long movement = abs(ax) + abs(ay) + abs(az);

  Serial.print("Movement: ");
  Serial.println(movement);

  
  if (reedState == HIGH || movement > 20000) {
    
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BUZZER_PIN, HIGH);

    if (!alertSent) {
      Serial.println("THEFT DETECTED!");
      bot.sendMessage(CHAT_ID, " ALERT: Theft Detected! Bag opened or moved!", "");
      alertSent = true; 
    }

  } else {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    if (alertSent) {
      bot.sendMessage(CHAT_ID, "Bag is safe now.", "");
      alertSent = false;
    }
    
    Serial.println("Safe");
  }

  delay(300);
}
