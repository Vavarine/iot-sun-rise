# IOT Sun Rise

## Description

This project uses a ESP8266 to control a bed side lamp. The RGB lamp originally uses a infrared remote control, which is replaced by the ESP8266.

**If you'd like to create a similar project, I've also [created a starter](https://github.com/Vavarine/esp8266-preact) template with the core features and functionalities.**

### Features

- Authenticates the user using a password (stored in the EEPROM)
- Control the lamp using a web interface
- Set the color of the lamp
- Set the brightness of the lamp
- Set the color temperature of the lamp
- Set the times the lamp should turn on, increasing the brightness over time (sun rise simulation, stored in the EEPROM)
- Manages a display that shows the current time, date an weather information.

### Web Interface

The web interface is a single page application that is served by the ESP8266. The interface is responsive and can be used on mobile devices. The interface is written in TypeScript and uses Preact as a lib. The styling is done using Tailwind CSS and DaisyUI. The interface is build using Vite and is served by the ESP8266. The choices for the technologies are made to keep the interface light-weight and fast since the ESP8266 has limited resources.

## Software

The server is written in c++ using the PlatformIO framework. The webserver is based on the `ESP8266WebServer` library. The IR remote is controlled using the `IRremoteESP8266` library. Adafruit's libraries are used to control the display. The weather information is fetched using the `OpenWeatherMap` API.

## Hardware

- ESP8266
- IR Led
- IR Receiver
- 5V Power Supply
- RGB Lamp (with IR remote)

