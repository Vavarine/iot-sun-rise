# IOT Sun Rise

## Todo - In Progress 🚧
- [x] Ir remote control
  - [x] Web App remote control interface
  - [x] Web server remote control api routes
- [ ] Add a "sunrise" feature to the lamp
  - [x] Web App sunrise interface (interface to create, edit and delete alarms)
  - [x] Web server sunrise api routes
    - [x] Save alarms to SPIFFS
    - [x] Read alarms
  - [ ] Schedule alarms
    - [ ] Get saved alarms and 
  - [x] Fade in light
- [ ] Refactoring
  - [ ] Web server IrSend and IrReceive to a class
  - [ ] "Sunrise" feature to a class

## Description

This project is a simple IOT project that uses a ESP8266 to control a bed side lamp. The RGB lamp originally uses a infrared remote control, which is replaced by the ESP8266. I am planning to add a "sunrise" feature to the lamp so that it can be used as a wake up light.

The ESP8266 hosts a webserver that can be used to control the lamp.

## Hardware

- ESP8266
- IR Led
- IR Receiver
- 5V Power Supply
- RGB Lamp (with IR remote)

## Software

The server is written in c++ using the PlatformIO framework. The webserver is based on the `ESP8266WebServer` library. The IR remote is controlled using the `IRremoteESP8266` library.

### Frontend

The server serves a Vite Preact app, as is a light-weight lib, that is written in TypeScript. The app uses [Tailwind CSS](https://tailwindcss.com/) for styling and [DaisyUI](https://daisyui.com/) for components.
