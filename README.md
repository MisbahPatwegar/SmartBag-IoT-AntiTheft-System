## IoT Smart Bag: Geo-Fenced Anti-Theft System
An IoT-based smart bag security system built with ESP8266 NodeMCU that detects unauthorized movement, bag opening, and geofence breaches — sending real-time alerts via Telegram Bot.

## Overview
Theft of personal belongings (laptops, wallets, documents) is a common concern in public spaces like buses, railways, and colleges. This project embeds a low-cost IoT circuit inside a regular bag to monitor its status continuously and instantly alert the owner via smartphone 

 | Hardware | Components |
 |---|---|
 |ESP8266 NodeMCU | Main microcontroller + Wi-Fi|
 |MPU6050 | Movement/tilt detection (accelerometer)| 
 |Magnetic Reed Switch | Bag open/close detection |
 |Active Buzzer | Local sound alert |
 |Green LED | Safe status indicator |
 |Red LED | Alert status indicator |
 |8650 Li-ion Battery + TP4056 | Portable power + charging | 

 ## How It Works
 The system runs in Safe Mode (green LED on) and switches to Alert Mode when any trigger condition is met:
- Movement Detection – MPU6050 monitors X/Y-axis acceleration.
- Bag Open Detection – A reed switch + magnet on the zipper detects unauthorized opening (HIGH = open).
- Wi-Fi Geofencing – ESP8266 scans for the owner's hotspot ; if not found, the bag is considered outside the safe zone (~15-20m range).
On any trigger: buzzer sounds, red LED turns on, and an instant Telegram alert is sent via Bot API. System auto-resets to Safe Mode after 5 seconds.

 **System Architecture**
![System Architecture](images/system_architecture.png)

 **Circuit Setup**
![Breadboard Setup](images/breadboard_setup.jpg)

 **Telegram Alerts in Action**
![Telegram Alerts](images/telegram_alerts.png)

## Software Stack
- Arduino IDE 2.3.8 with ESP8266 board package
- Libraries: `MPU6050` (Electronic Cats), `ArduinoJson`, `CTBot` (Telegram), `ESP8266WiFi`, `Wire`
- Telegram Bot (`SmartBagAlert`) created via BotFather for real-time notifications

## Results

 |Test Case|  Result |
 |---|---|
 |Bag lifted suddenly | Alert in < 2 sec|
 |Zipper opened | Alert in < 0.5 sec|
 |Geofence breach | Alert in 60 sec | 
 |Telegram delivery | Avg 1-2 sec|

## Future Improvements

- GPS module (NEO-6M) for real-time location tracking
- Camera module for visual confirmation on alert

