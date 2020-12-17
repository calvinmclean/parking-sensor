#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include "graphics.h"

// bottom of range for color changing (RED)
#define CLOSE_DISTANCE 30
// top of range for color changing (GREEN)
#define FAR_DISTANCE 65
// max distance to display
#define MAX_DISTANCE 99
// distance from sensor to front of car
#define OFFSET 7

// number of consecutive valid readings before re-enabling display
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

// Display settings
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

#define FONT_SIZE 9
#define FONT_W 5 * FONT_SIZE
#define FONT_H 8 * FONT_SIZE

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

int distances[DISPLAY_THRESHOLD];
bool on = true;

void setup() {
    Serial.begin(115200);

    // Prepare distance sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Prepare display
    tft.begin();
    tft.setTextSize(FONT_SIZE);

    // Display Volvo logo for 1 second on boot-up
    tft.drawBitmap(0, 0, volvoLogo, 128, 128, BLACK, WHITE);
    delay(1000);
    tft.fillScreen(BLACK);
}

void loop() {
    long duration = readDuration();
    int distance = duration * 0.034 / 2 - OFFSET;

    // Used for testing fonts:
    // long duration = 1;
    // distance++;
    // if (distance > MAX_DISTANCE + 5) {
    //     distance = 0;
    // }

    if (duration == 0) {
        Serial.println("No pulse from sensor");
        tft.setTextSize(FONT_SIZE - 2);
        tft.setTextColor(WHITE, BLACK);
        tft.setCursor(1, (SCREEN_HEIGHT - 8 * (FONT_SIZE - 2)) / 2 + 5);
        tft.println("N/A");
    } else {
        tft.setTextSize(FONT_SIZE);
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        saveMeasurement(distance);
        if (distance <= MAX_DISTANCE) {
            checkPrevious();
            if (on) {
                displayDistance(distance);
            }
        } else {
            on = false;
            tft.fillScreen(BLACK);
        }
    }
    delay(200);
}

long readDuration() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    return pulseIn(ECHO_PIN, HIGH);
}

void displayDistance(int distance) {
    int value = map(distance, CLOSE_DISTANCE, FAR_DISTANCE, 0, 255);
    uint8_t red = 255 - value;
    uint8_t green = value;

    // https://stackoverflow.com/a/13721305
    uint16_t color = ((red >> 3) << 11) | ((green >> 2) << 5) | 0;

    if (distance >= FAR_DISTANCE) {
        color = GREEN;
    } else if (distance <= CLOSE_DISTANCE) {
        color = RED;
    }
    tft.setTextColor(color, BLACK);

    int numDigits = (distance >= 10) + 1;
    tft.setCursor((SCREEN_WIDTH - FONT_W * numDigits) / 2 - 1, (SCREEN_HEIGHT - FONT_H) / 2 + 5);
    tft.println(distance);
}

void saveMeasurement(int distance) {
    for (int i = 0; i < DISPLAY_THRESHOLD - 1; i++) {
        distances[i] = distances[i + 1];
    }
    distances[DISPLAY_THRESHOLD - 1] = distance;
}

void checkPrevious() {
    int i;
    for (i = 0; i < DISPLAY_THRESHOLD; i++) {
        if (distances[i] > MAX_DISTANCE) {
            break;
        }
    }
    on = (i == DISPLAY_THRESHOLD);
}
