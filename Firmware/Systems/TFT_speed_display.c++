#include <TFT_eSPI.h>
#include <SPI.h>

#define USER_SETUP_INFO "User_Setup"
#define ILI9341_DRIVER
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5
#define TFT_DC   2
#define TFT_RST  4

TFT_eSPI tft = TFT_eSPI();

// Constants
const int SPEED_UPDATE_INTERVAL = 1000; // Update speed every 1 second
const int SPEED_X = 120; // X-coordinate for speed display
const int SPEED_Y = 160; // Y-coordinate for speed display

// Variables
int currentSpeed = 0; // This will be replaced with actual speed from ESC later
unsigned long lastUpdate = 0;

void setup() {
  tft.init();
  tft.setRotation(1); // Adjust rotation if needed
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString("Go-Kart Speed", 70, 20);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastUpdate >= SPEED_UPDATE_INTERVAL) {
    updateSpeed();
    displaySpeed();
    lastUpdate = currentTime;
  }
}

void updateSpeed() {
  // For now, constant value
  // Later this function will fetch the speed from the ESC
  currentSpeed = 25; // Example constant speed in km/h
}

void displaySpeed() {
  tft.setTextSize(6);
  tft.setCursor(SPEED_X, SPEED_Y);
  tft.printf("%3d", currentSpeed);
  tft.setTextSize(2);
  tft.drawString("km/h", SPEED_X + 110, SPEED_Y + 30);
}
