#ifndef config_h
#define config_h

/**
 * Distance Display Settings -- configure what is shown on the screen
 *
 * - CLOSE_DISTANCE:
 *   Smallest distance to display on the screen and bottom of range for color changing (100% RED)
 * - FAR_DISTANCE:
 *   Top of range for color changing (100% GREEN)
 * - MAX_DISTANCE:
 *   Largest distance to display on screen
 * - DISPLAY_THRESHOLD:
 *   Number of consecutive valid readings before re-enabling display this will help to prevent random
 *   distance readings while driving
 */
#define CLOSE_DISTANCE 30
#define FAR_DISTANCE 65
#define MAX_DISTANCE 99
#define DISPLAY_THRESHOLD 5

/**
 * Sensor Configuration -- this allows for configuring multiple sensors
 *
 * When multiple sensors are enabled, the smallest distance measurement will be displayed on
 * the screen.
 *
 * When configuring sensor pins in the array below, each sensor should look like this:
 * { TRIG_PIN, ECHO_PIN, OFFSET }
 * OFFSET is the distance from the front of the sensor to the front of the car.
 */
#define NUM_SENSORS 2
#define SENSORS { {GPIO_NUM_26, GPIO_NUM_27, 7}, {GPIO_NUM_14, GPIO_NUM_12, 13} }

/**
 * OLED SPI Pins
 */
#define SCLK_PIN GPIO_NUM_18
#define MOSI_PIN GPIO_NUM_23
#define DC_PIN   GPIO_NUM_16
#define CS_PIN   GPIO_NUM_17
#define RST_PIN  GPIO_NUM_5

#endif
