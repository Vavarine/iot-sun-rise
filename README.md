# IOT Sun Rise

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
