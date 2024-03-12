#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Stepper.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_SPIDevice.h>
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(4, D0, NEO_GRB + NEO_KHZ800);

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

// Client Code
#include "BLEDevice.h"

// The remote service we wish to connect to.
// BLE variables
// The remote service we wish to connect to.
static BLEUUID serviceUUID("80ce2fe1-195b-437d-b811-9e0bc9dca6ed");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("41b23795-d294-432a-a5bd-5ddcdf444404");
static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

#define TFT_DC D7
#define TFT_CS D6
#define TFT_MOSI D10
#define TFT_CLK D8
#define TFT_RST D5
#define TFT_MISO D9

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

#define IN1 D1  // Motor driver input pins
#define IN2 D2
#define IN3 D3
#define IN4 D4

const int stepsPerRevolution = 2048;
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

#define NUM_COLORS 13
uint32_t colors[NUM_COLORS] = {
  pixels.Color(255, 0, 0),       // Red
  pixels.Color(255, 165, 0),     // Orange
  pixels.Color(255, 255, 0),     // Yellow
  pixels.Color(154, 205, 50),    // Green Yellow
  pixels.Color(0, 128, 0),       // Green
  pixels.Color(0, 255, 0),       // Blue Green
  pixels.Color(0, 0, 255),       // Blue
  pixels.Color(138, 43, 226),    // Blue Violet
  pixels.Color(75, 0, 130),      // Violet
  pixels.Color(147, 112, 219),   // Mauve
  pixels.Color(255, 105, 180),   // Mauve Pink
  pixels.Color(255, 192, 203)    // Pink
};

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    if (length == 0) {
      Serial.println("Received empty data");
      return;
    }

    char* token = strtok((char*)pData, ",\n");
    if (token == NULL) {
      Serial.println("Incomplete or invalid data received");
      return;
    }
    uint8_t red = atoi(token);
    token = strtok(NULL, ",\n");
    if (token == NULL) {
      Serial.println("Incomplete or invalid data received");
      return;
    }
    uint8_t green = atoi(token);
    token = strtok(NULL, ",\n");
    if (token == NULL) {
      Serial.println("Incomplete or invalid data received");
      return;
    }
    uint8_t blue = atoi(token);

    Serial.print("Received RGB data - R: ");
    Serial.print(red);
    Serial.print(", G: ");
    Serial.print(green);
    Serial.print(", B: ");
    Serial.println(blue);

    // Find the closest color in the predefined colors array
    int closestColorIndex = 0;
    float minDistance = 255 * 255 * 3; // Maximum possible distance
    for (int i = 0; i < NUM_COLORS; ++i) {
      uint32_t predefinedColor = colors[i];
      uint8_t predefinedRed = (predefinedColor >> 16) & 0xFF;
      uint8_t predefinedGreen = (predefinedColor >> 8) & 0xFF;
      uint8_t predefinedBlue = predefinedColor & 0xFF;

      // Calculate Euclidean distance between received color and predefined color
      float distance = sqrt(pow(red - predefinedRed, 2) + pow(green - predefinedGreen, 2) + pow(blue - predefinedBlue, 2));
      if (distance < minDistance) {
        minDistance = distance;
        closestColorIndex = i;
      }
    }

    // Display color on TFT display in loop
    tft.fillScreen(ILI9341_BLACK);
    tft.fillCircle(120, 160, 50, colors[closestColorIndex]);
    tft.setCursor(60, 150);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.print("RGB: ");
    tft.print(red);
    tft.print(", ");
    tft.print(green);
    tft.print(", ");
    tft.print(blue);

    // Turn the stepper motor to the closest color section
   // int steps = closestColorIndex * (stepsPerRevolution / NUM_COLORS);
    // myStepper.step(steps);
}

class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {}

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());

    BLEClient*  pClient  = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remote BLE Server.
    pClient->connect(myDevice);
    Serial.println(" - Connected to server");
    pClient->setMTU(517);

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

    // Obtain a reference to the characteristic in the service of the remote BLE server.
    pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
    if (pRemoteCharacteristic == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our characteristic");

    // Read the value of the characteristic.
    if(pRemoteCharacteristic->canRead()) {
      std::string value = pRemoteCharacteristic->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristic->canNotify())
      pRemoteCharacteristic->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Arduino BLE Client application...");
  BLEDevice::init("");

  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

  tft.begin();
  tft.setRotation(3);

  myStepper.setSpeed(0);
}

void loop() {
  if (doConnect == true) {
    if (connectToServer()) {
      Serial.println("We are now connected to the BLE Server.");
    } else {
      Serial.println("We have failed to connect to the server.");
    }
    doConnect = false;
  }

  if (connected) {
    String newValue = "Time since boot: " + String(millis()/1000);
    Serial.println("Setting new characteristic value to \"" + newValue  + "\"");
    pRemoteCharacteristic->writeValue(newValue.c_str(), newValue.length());
  } else if(doScan){
    BLEDevice::getScan()->start(0);
  }

  delay(1000);
}