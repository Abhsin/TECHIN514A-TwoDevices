#include <Arduino.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

Adafruit_AS7341 as7341;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

const int buttonPin = D0; // Pin D0
bool readingsEnabled = false; // Initial state: readings disabled
bool lastButtonState = true; // Variable to store the previous state of the button

#define SERVICE_UUID        "80ce2fe1-195b-437d-b811-9e0bc9dca6ed"
#define CHARACTERISTIC_UUID "41b23795-d294-432a-a5bd-5ddcdf444404"

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

void setup() {
    Serial.begin(115200);

    pinMode(buttonPin, INPUT); // Set the button pin as input

    if (!as7341.begin()){
        Serial.println("Could not find AS7341");
        while (1) { delay(10); }
    }

    as7341.setATIME(100);
    as7341.setASTEP(999);
    as7341.setGain(AS7341_GAIN_256X);

    // Initialize BLE
    BLEDevice::init("AS7341_RGB");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());
    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );
    pCharacteristic->addDescriptor(new BLE2902());
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
}

void loop() {
    // Read the state of the button
    bool buttonState = digitalRead(buttonPin);

    // Check if the button state has changed from HIGH to LOW
    if (buttonState == LOW && lastButtonState == HIGH) {
        readingsEnabled = !readingsEnabled; // Toggle readings state
        if (readingsEnabled) {
            Serial.println("Starting measurements...");
        } else {
            Serial.println("Stopping measurements...");
        }
        delay(100); // Debounce delay
    }

    lastButtonState = buttonState; // Update the last button state

    if (readingsEnabled) {
        // Read all channels at the same time and store in as7341 object
        if (!as7341.readAllChannels()){
            Serial.println("Error reading all channels!");
            return;
        }

        uint16_t rawRed = as7341.getChannel(AS7341_CHANNEL_630nm_F7);
        uint16_t rawGreen = as7341.getChannel(AS7341_CHANNEL_555nm_F5);
        uint16_t rawBlue = as7341.getChannel(AS7341_CHANNEL_480nm_F3);

        // Check if raw values are within the valid range
        if (rawRed >= 0 && rawRed <= 65535 &&
            rawGreen >= 0 && rawGreen <= 65535 &&
            rawBlue >= 0 && rawBlue <= 65535) {

            // Map the raw values to the 0-255 range
            uint16_t red = map(rawRed, 0, 65535, 0, 255);
            uint16_t green = map(rawGreen, 0, 65535, 0, 255);
            uint16_t blue = map(rawBlue, 0, 65535, 0, 255);

            // Convert to string for BLE characteristic
            String rgbData = String(red) + "," + String(green) + "," + String(blue);

            // Update BLE characteristic with RGB data
            if (deviceConnected) {
                pCharacteristic->setValue(rgbData.c_str());
                pCharacteristic->notify();
                Serial.print("Notifying RGB data: ");
                Serial.println(rgbData);
            }

            as7341.setLEDCurrent(12);
            as7341.enableLED(true);
            delay(1000); // Delay for readability, adjust as needed
        } else {
            Serial.println("Invalid raw channel values!");
        }
    }
}
