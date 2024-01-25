#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <stdlib.h>

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
unsigned long previousMillis = 0;
const long interval = 1000;

// TODO: add new global variables for your sensor readings and processed data
// HC-SR04 Pins
const int trigPin = D0;
const int echoPin = D1;

// Sensor data and DSP variables
float distance;
const int numReadings = 5;
float readings[numReadings];      // the readings from the sensor
int readIndex = 0;                // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

// TODO: Change the UUID to your own (any specific one works, but make sure they're different from others'). You can generate one here: https://www.uuidgenerator.net/
#define SERVICE_UUID        "d4d8b28b-8928-4044-b3b2-fbed8f587fd0"
#define CHARACTERISTIC_UUID "c8e44563-c8f3-4822-8a41-9f4df10fa9ac"

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
    }
};

// TODO: add DSP algorithm functions here
// Function to calculate moving average
float calculateMovingAverage(float newDistance) {
    // subtract the last reading:
    total -= readings[readIndex];
    // read from the sensor:
    readings[readIndex] = newDistance;
    // add the reading to the total:
    total += readings[readIndex];
    // advance to the next position in the array:
    readIndex = (readIndex + 1) % numReadings;

    // calculate the average:
    return total / numReadings;
}

void setup() {
    Serial.begin(115200);
    Serial.println("Starting BLE work!");

    // TODO: add codes for handling your sensor setup (pinMode, etc.)
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
        readings[thisReading] = 0; // initialize all the readings to 0
    }

    // TODO: name your device to avoid conflictions
    BLEDevice::init("HaoranServer");
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
    pCharacteristic->setValue("Hello World");
    pService->start();
    // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
    // TODO: add codes for handling your sensor readings (analogRead, etc.)
    
    // Clears the trigPin
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // Sets the trigPin on HIGH state for 10 micro seconds
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    long duration = pulseIn(echoPin, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)


    // TODO: use your defined DSP algorithm to process the readings
    // Calculate moving average
    average = calculateMovingAverage(distance);

    Serial.print("Raw Distance: ");
    Serial.print(distance);
    Serial.print(" cm, Denoised: ");
    Serial.print(average);
    Serial.println(" cm");

if (deviceConnected) {
    // Send new readings to database only if the denoised distance is less than 30 cm
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        if (average < 30) { // Check if the denoised distance is less than 30 cm
            // Prepare the message with the average distance
            String message = "Distance: " + String(average, 2) + " cm";
            // Set the characteristic's value to the message
            pCharacteristic->setValue(message.c_str());
            // Notify the connected device with the new value
            pCharacteristic->notify();
            // Print the message to the Serial monitor for debugging
            Serial.println("BLE Notify value: " + message);
        } else {
            // If the distance is 30 cm or more, you can handle it differently or do nothing
            Serial.println("Distance is 30 cm or more, not sending over BLE.");
        }
        // Update the time of the last transmission
        previousMillis = currentMillis;
    }
}

    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500);  // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising();  // advertise again
        Serial.println("Start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    delay(1000);
}
