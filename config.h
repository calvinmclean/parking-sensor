#ifndef config_h
#define config_h

// bottom of range for color changing (RED)
#define CLOSE_DISTANCE 30
// top of range for color changing (GREEN)
#define FAR_DISTANCE 65
// max distance to display
#define MAX_DISTANCE 99
// distance from sensor to front of car
#define OFFSET 7

// number of consecutive valid readings before re-enabling display
// this will help to prevent random distance readings while driving
#define DISPLAY_THRESHOLD 5

// Distance sensor pins
#define TRIG_PIN 16 // D0
#define ECHO_PIN 5 // D1

// OLED SPI pins
#define SCLK_PIN 14 // D5
#define MOSI_PIN 13 // D7
#define DC_PIN   4  // D2
#define CS_PIN   15 // D8
#define RST_PIN  0  // D3

#endif