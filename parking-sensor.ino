#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include "graphics.h"
#include "config.h"

Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN, RST_PIN);

int distances[DISPLAY_THRESHOLD];

void setup() {
    Serial.begin(115200);

    // Prepare distance sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    // Prepare display
    tft.begin();
    tft.setTextSize(FONT_SIZE);

#ifdef DISPLAY_VOLVO_LOGO
    // Display Volvo logo for 1 second on boot-up
    tft.drawBitmap(0, 0, volvoLogo, 128, 128, BLACK, WHITE);
    delay(1000);
#endif

    // Check for sensor data
    if (readDistance() <= 0) {
        Serial.println("No pulse from sensor");
        displayNA();
        delay(3000);
        tft.fillScreen(BLACK);
    }

    tft.fillScreen(BLACK);
}

void loop() {
    long duration = readDuration();
    int distance = duration * 0.034 / 2 - OFFSET;

    if (duration == 0) {
        Serial.println("No pulse from sensor");
    } else {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        saveMeasurement(distance);
        if (distance <= MAX_DISTANCE && checkPrevious()) {
            displayDistance(distance);
        } else {
            tft.fillScreen(BLACK);
        }
    }
    delay(200);
}

/*
  readDuration will send a pulse and read the incoming pulse to get the duration
*/
long readDuration() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    return pulseIn(ECHO_PIN, HIGH);
}

/*
  displayDistance will write the measured distance value to the OLED display with a
  color gradient
*/
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
    tft.setTextSize(FONT_SIZE);

    int numDigits = (distance >= 10) + 1;
    tft.setCursor((SCREEN_WIDTH - FONT_W * numDigits) / 2 - 1, (SCREEN_HEIGHT - FONT_H) / 2 + 5);
    tft.println(distance);
}

/*
  displayNA is used to put "N/A" on the display to indicate there is no distance input
*/
void displayNA() {
    tft.setTextSize(FONT_SIZE - 2);
    tft.setTextColor(WHITE, BLACK);
    tft.setCursor(1, (SCREEN_HEIGHT - 8 * (FONT_SIZE - 2)) / 2 + 5);
    tft.println("N/A");
}

/*
  saveMeasurement will append the most recent measurement to the array and shift existing
  measurements to keep only DISPLAY_THRESHOLD values in the array
*/
void saveMeasurement(int distance) {
    for (int i = 0; i < DISPLAY_THRESHOLD - 1; i++) {
        distances[i] = distances[i + 1];
    }
    distances[DISPLAY_THRESHOLD - 1] = distance;
}

/*
  checkPrevious will determine if there have been enough consecutive measurements to
  re-enable the display
*/
bool checkPrevious() {
    int i;
    for (i = 0; i < DISPLAY_THRESHOLD; i++) {
        if (distances[i] > MAX_DISTANCE) {
            break;
        }
    }
    return (i == DISPLAY_THRESHOLD);
}
