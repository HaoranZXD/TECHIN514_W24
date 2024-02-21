/*********
  Rui Santos
  Complete instructions at: https://RandomNerdTutorials.com/esp32-cam-save-picture-firebase-storage/

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  Based on the example provided by the ESP Firebase Client Library
*********/

#include "Arduino.h"
#include "WiFi.h"
#include "soc/soc.h"          // Disable brownout problems
#include "soc/rtc_cntl_reg.h" // Disable brownout problems
#include "driver/rtc_io.h"
#include <LittleFS.h>
#include <Firebase_ESP_Client.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>
#include "esp_camera.h"
#include "FS.h"
#include <time.h>

#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
#define BUTTON_PIN 1

#include "camera_pins.h"

// Replace with your wifi password that you got from registering your device
// on the UW MPSK network
const char *ssid = "UW MPSK";
const char *password = "<Your-Device-MAC-address>";

// Insert Firebase project API Key
#define API_KEY "<YOUR-PROJECT-API-KEY>"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "<Your-created-user-email>"
#define USER_PASSWORD "<Your-created-user-password>"

// Insert Firebase storage bucket ID e.g bucket-name.appspot.com
#define STORAGE_BUCKET_ID "<Your-storage-bucket-id>"

// Photo File Name to save in LittleFS
#define FILE_PHOTO_PATH "/photo.jpg"
#define BUCKET_PHOTO "/data/photo.jpg"

boolean takeNewPhoto = true;
bool camera_sign = false; // Check camera status

// Define Firebase Data objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configF;

void fcsUploadCallback(FCS_UploadStatusInfo info);

bool taskCompleted = false;

// Capture Photo and Save it to LittleFS
void capturePhotoSaveLittleFS(void)
{
  // Dispose first pictures because of bad quality
  camera_fb_t *fb = NULL;
  // Skip first 3 frames (increase/decrease number as needed).
  for (int i = 0; i < 4; i++)
  {
    fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
    fb = NULL;
  }

  // Take a new photo
  fb = NULL;
  fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  // Photo file name
  Serial.printf("Picture file name: %s\n", FILE_PHOTO_PATH);
  File file = LittleFS.open(FILE_PHOTO_PATH, FILE_WRITE);

  // Insert the data in the photo file
  if (!file)
  {
    Serial.println("Failed to open file in writing mode");
  }
  else
  {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.print("The picture has been saved in ");
    Serial.print(FILE_PHOTO_PATH);
    Serial.print(" - Size: ");
    Serial.print(fb->len);
    Serial.println(" bytes");
  }
  // Close the file
  file.close();
  esp_camera_fb_return(fb);
}

void initWiFi()
{
  Serial.println(WiFi.macAddress());
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void initLittleFS()
{
  if (!LittleFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting LittleFS");
    ESP.restart();
  }
  else
  {
    delay(500);
    Serial.println("LittleFS mounted successfully");
  }
}

String generateFileName()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return String("photo_") + String(time(nullptr)) + String(".jpg"); // Fallback if NTP time not set
  }
  char buffer[30];
  strftime(buffer, sizeof(buffer), "/photo_%Y%m%d%H%M%S.jpg", &timeinfo);
  return String(buffer);
}

void initCamera()
{
  Serial.println("Initializing Camera");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_QVGA;
  config.pixel_format = PIXFORMAT_JPEG; // for streaming
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG)
  {
    if (psramFound())
    {
      config.jpeg_quality = 10;
      config.fb_count = 1;
      config.grab_mode = CAMERA_GRAB_LATEST;
    }
    else
    {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  }
  else
  {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 1;
#endif
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK)
  {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  camera_sign = true; // Camera initialization check passes
}

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  initWiFi();
  initLittleFS();
  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  initCamera();
  Serial.println("Camera Ready!");
  // Firebase
  //  Assign the api key
  configF.api_key = API_KEY;
  // Assign the user sign in credentials
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  // Assign the callback function for the long running token generation task
  configF.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&configF, &auth);
  Firebase.reconnectWiFi(true);
  Serial.print("Firebase ready: ");
  Serial.println(Firebase.ready());
}

void loop()
{
  // Replace or modify your existing loop code with this:
  static unsigned long lastButtonPress = 0;
  if (digitalRead(BUTTON_PIN) == LOW)
  { // Check if button is pressed
    unsigned long currentTime = millis();
    if (currentTime - lastButtonPress > 500)
    { // Simple debounce
      lastButtonPress = currentTime;
      Serial.println("Button pressed!");

      // Capture and upload photo
      if (camera_sign)
      { // Check if camera initialized successfully
        takeNewPhoto = true;
        if (takeNewPhoto)
        {
          capturePhotoSaveLittleFS();
          takeNewPhoto = false;
        }
        // Attempt to upload if Firebase is ready and last task completed
        if (Firebase.ready())
        {
          taskCompleted = true;

          String fileName = generateFileName();       // Generate unique file name
          String bucketFilePath = "/data" + fileName; // Modify the path as needed

          Serial.print("Uploading picture as: ");
          Serial.println(fileName);

          // MIME type should be valid to avoid the download problem.
          // The file systems for flash and SD/SDMMC can be changed in FirebaseFS.h.
          if (Firebase.Storage.upload(&fbdo, STORAGE_BUCKET_ID, FILE_PHOTO_PATH, mem_storage_type_flash, bucketFilePath, "image/jpeg", fcsUploadCallback))
          {
            Serial.printf("\nDownload URL: %s\n", fbdo.downloadURL().c_str());
          }
          else
          {
            Serial.println(fbdo.errorReason());
          }
        }
      }
    }
  }
  delay(1); // Small delay to prevent watchdog timer issues
}

// The Firebase Storage upload callback function
void fcsUploadCallback(FCS_UploadStatusInfo info)
{
  if (info.status == firebase_fcs_upload_status_init)
  {
    Serial.printf("Uploading file %s (%d) to %s\n", info.localFileName.c_str(), info.fileSize, info.remoteFileName.c_str());
  }
  else if (info.status == firebase_fcs_upload_status_upload)
  {
    Serial.printf("Uploaded %d%s, Elapsed time %d ms\n", (int)info.progress, "%", info.elapsedTime);
  }
  else if (info.status == firebase_fcs_upload_status_complete)
  {
    Serial.println("Upload completed\n");
    FileMetaInfo meta = fbdo.metaData();
    Serial.printf("Name: %s\n", meta.name.c_str());
    Serial.printf("Bucket: %s\n", meta.bucket.c_str());
    Serial.printf("contentType: %s\n", meta.contentType.c_str());
    Serial.printf("Size: %d\n", meta.size);
    Serial.printf("Generation: %lu\n", meta.generation);
    Serial.printf("Metageneration: %lu\n", meta.metageneration);
    Serial.printf("ETag: %s\n", meta.etag.c_str());
    Serial.printf("CRC32: %s\n", meta.crc32.c_str());
    Serial.printf("Tokens: %s\n", meta.downloadTokens.c_str());
    Serial.printf("Download URL: %s\n\n", fbdo.downloadURL().c_str());
  }
  else if (info.status == firebase_fcs_upload_status_error)
  {
    Serial.printf("Upload failed, %s\n", info.errorMsg.c_str());
  }
}