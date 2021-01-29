#ifndef config_h
#define config_h

// bottom of range for color changing (RED)
#define CLOSE_DISTANCE 30
// top of range for color changing (GREEN)
#define FAR_DISTANCE 65
// max distance to display
#define MAX_DISTANCE 99

// number of consecutive valid readings before re-enabling display
// this will help to prevent random distance readings while driving
#define DISPLAY_THRESHOLD 5

/**
 * Sensor Configuration -- this allows for configuring multiple sensors
 *
 * When multiple sensors are enabled, the smallest distance measurement will be displayed on
 * the screen.
 *
 * When configuring sensor pins in the array below, each sensor should look like this:
 * { TRIG_PIN, ECHO_PIN, OFFSET }
 * Where the OFFSET is the distance from the front of the sensor to the front of the car
 *
 * 16 = D0
 *  5 = D1
 */
#define NUM_SENSORS 1
#define SENSOR_PINS { {16, 5, 7} }

 // OLED SPI pins
#define SCLK_PIN 14 // D5
#define MOSI_PIN 13 // D7
#define DC_PIN   4  // D2
#define CS_PIN   15 // D8
#define RST_PIN  0  // D3

#endif