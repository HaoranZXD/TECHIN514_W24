# Lab 5 - ESP32 Power Management Lab

## Lab Instructions

In this lab, you'll learn the basics about how to measure the power consumption of your Seeeduino XIAO ESP32S3 microcontroller in differeng usage scenarios. You'll also learn how to develop your own strategy of keeping your microcontroller to be energy-friendly.

Follow the instruction written in[ Lab5: Power Mangement.md](https://github.com/GIXLabs/TECHIN514_W24/blob/main/Lab5_power-management/Lab5_power-management.md) to:

* Connect your ESP32S3 to the UW MPSK Wifi
* Establish connection between your ESP32S3 and Firebase Realtime Database (RTDB)
* Transmitting your data from your device to the RTDB
* Use nRF Power Profiler and its kit to measure the power consumption of your ESP32S3
* Developing your own strategy based on different *mode* of ESP32S3 to save energy

Some useful external resources:

* [nRF Connect Download](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-Desktop/Download#infotabs)
* [J-Link Download Page](https://www.segger.com/downloads/jlink/)
* [UW MPSK Setup Instructions](https://itconnect.uw.edu/tools-services-support/networks-connectivity/uw-networks/campus-wi-fi/uw-mpsk/)
* [Firebase Setup Instructions](https://randomnerdtutorials.com/esp32-firebase-realtime-database/)

## Lab Requirements

### 5.1 (Preliminary) Connect your ESP32S3 to the UW MPSK Wi-fi and then Firebase

* Nothing requires to be submiited here.

### 5.2 Transmitting your HC-SR04 Data to Firebase

* Take a screenshot on your Firebase RTDB's webpage to show that your data has been successfully uploaded.

### 5.3 Power Consumption Measuring

* Take a screenshot of the plot on your power profiler app to show the power consumption change during **1 minute with 5 different *mode or stages*** of ESP32S3. Annnotate what each stage stands for.
* Set the time window **from 1 minute to 10 seconds** and take a screenshot for *each* power-using mode accordingly. Calculate how long would a 500mAh battery last when the ESP32S3 is in each mode.
* Rewrite your code and measure the power consumption in different data transmitting rates. Take screenshots and do the calculations as above. Plot a figure to demonstrate the correlation between the data transmitting rate and power consumption.

### 5.4 Create your own power-saving strategy

* Describe your own policy/strategy based on the five modes presented above to make sure a 500mAh battery can support it without being recharged for at least 24 hours
* Upload your code with your strategy to the board. Simulate your use case and demonstrate them with the plots on your power profiler app. Annotate each stage on the plot.
* Upload your code to GitHub.

## What to be submitted on Canvas

Submit a single PDF of all screenshots as specified below.

1. The link to your edited code with your power-saving strategy on GitHub. You can either create a new repo or push the updates to your existing repo for all labs (recommended).
2. Screenshot of your firebase RTDB receiving the ultrasonic sensor readouts.
3. Annotated screenshot of the plot on your power profiler app to show the power consumption in 5 different stages of ESP32S3's usage.
4. Screenshots and calculations of each stage's power consumption and estimated battery-lasting time, respectively.
5. Description of your own power-saving strategies/policies.
6. Annotated screenshots of the power consumption plot of your power-saving strategy's use case.
