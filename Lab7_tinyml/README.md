# TinyML Lab

## **Introduction**

Welcome to the TinyML lab session. This lab is designed to provide hands-on experience with integrating machine learning models into small, low-power devices using the ESP32S3 module. Tiny Machine Learning (TinyML) is an emerging field that combines embedded systems with machine learning capabilities, enabling intelligent decision-making on edge devices.

## **Overview of the Lab**

The primary goal of this lab is to guide you through the process of collecting data, training a machine learning model, and deploying this model to perform real-time inference on an ESP32S3. You will start by setting up a Firebase project for storing and managing image data. Following this, you will configure an ESP32S3 with a switch to capture images, which will serve as the dataset for training your model. Using Edge Impulse, you will then train a machine learning model with your collected dataset. Finally, you will deploy this model to the ESP32S3 to classify images in real-time.

## **Objectives**

By the end of this lab, you will be able to:

- Set up and manage a Firebase project for image storage.
- Configure an ESP32S3 module to capture and upload images.
- Use the Google Cloud SDK to manage and download image data.
- Train a machine learning model with Edge Impulse using your dataset.
- Deploy and test the machine learning model on the ESP32S3.

## **Tools and Materials Needed**

- ESP32S3 module
- Switch/button
- Jumper wires

This lab is structured to enhance your understanding and skills in applying machine learning algorithms to real-world IoT devices. Let's proceed with the setup and exploration of TinyML capabilities.

## Part 1: Setting Up Your Firebase Project

### **1. Create a Firebase Project**

**1)** Go to [Firebase](https://firebase.google.com/) and sign in using a Google Account.

**2)** Click **Get Started** and then **Add project** to create a new project.

**3)** Give a name to your project, for example: *ESP Firebase Demo*.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Create-firebase-project-1.png?resize=750%2C687&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Create-firebase-project-1.png?resize=750%2C687&quality=100&strip=all&ssl=1)

**4)** Disable the option *Enable Google Analytics* for this project as it is not needed and click **Create project**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/08/2-Set-Up-Firebase-Project-ESP32-ESP8266.png?resize=750%2C691&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/08/2-Set-Up-Firebase-Project-ESP32-ESP8266.png?resize=750%2C691&quality=100&strip=all&ssl=1)

**5)** It will take a few seconds to set up your project. Then, click *Continue* when it’s ready.

**6)** You’ll be redirected to your Project console page.

### **2. Set Authentication Methods**

To allow authentication with email and password, first, you need to set authentication methods for your app.

“Most apps need to know the identity of a user. In other words, it takes care of logging in and identifying the users (in this case, the ESP32S3). Knowing a user’s identity allows an app to securely save user data in the cloud and provide the same personalized experience across all of the user’s devices.” To learn more about the authentication methods, you can [read the documentation](https://firebase.google.com/docs/auth).

**1)** On the left sidebar, click on **Authentication** and then on **Get started**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-Project-Authentication.png?resize=750%2C359&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-Project-Authentication.png?resize=750%2C359&quality=100&strip=all&ssl=1)

**2)** Select the Option **Email/Password**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Authentication-methods.png?resize=750%2C394&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Authentication-methods.png?resize=750%2C394&quality=100&strip=all&ssl=1)

**3)** Enable that authentication method and click **Save**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-email-password-sign-in-provider.png?resize=750%2C323&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-email-password-sign-in-provider.png?resize=750%2C323&quality=100&strip=all&ssl=1)

**4)** The authentication with email and password should now be enabled.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/email-password-enabled.png?resize=668%2C208&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/email-password-enabled.png?resize=668%2C208&quality=100&strip=all&ssl=1)

**5)** Now, you need to add a user. Still on the **Authentication** tab, select the **Users** tab at the top. Then, click on **Add User**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-Add-New-User.png?resize=750%2C335&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-Add-New-User.png?resize=750%2C335&quality=100&strip=all&ssl=1)

**6)** Add a FAKE email address for the authorized user. It can be any random FAKE email. Add a password that will allow you to sign in to your app and access the storage files. Don’t forget to save the password in a safe place because you’ll need it later. When you’re done, click **Add user**.

<aside>
💡 **NOTE** : DO NOT USE YOUR OWN EMAIL ADDRESS HERE. YOU CAN USE ANY FAKE CREDENTIALS FOR CREATING THIS USER eg.
Email : `test@example.com` 
Password : `123456`

</aside>

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-Add-User-Email-Password.png?resize=750%2C494&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Firebase-Add-User-Email-Password.png?resize=750%2C494&quality=100&strip=all&ssl=1)

**7)** A new user was successfully created and added to the **Users** table.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Users-Table-Firebase.png?resize=750%2C188&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/10/Users-Table-Firebase.png?resize=750%2C188&quality=100&strip=all&ssl=1)

Notice that Firebase creates a unique UID for each registered user. The user UID allows us to identify the user and keep track of the user to provide or deny access to the project or the database. There’s also a column that registers the date of the last sign-in. At the moment, it is empty because we haven’t signed in with that user yet.

### **3. Create Storage Bucket**

**1)** On the left sidebar, click on **Storage** and then on **Get started**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Storage-Get-Started.png?resize=750%2C365&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Storage-Get-Started.png?resize=750%2C365&quality=100&strip=all&ssl=1)

**2)** Select *Start in **test mode***—click **Next**. We’ll change the storage rules later on.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2022/01/Create-storage-bucket-Firebase.png?resize=750%2C497&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2022/01/Create-storage-bucket-Firebase.png?resize=750%2C497&quality=100&strip=all&ssl=1)

**3)** Select your storage location—it should be the closest to your country then press Done.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/firebase-storage-location.png?resize=750%2C424&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/firebase-storage-location.png?resize=750%2C424&quality=100&strip=all&ssl=1)

**4)** Wait a few seconds while it creates the storage bucket.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Creating-Storage-Bucket.png?resize=750%2C409&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Creating-Storage-Bucket.png?resize=750%2C409&quality=100&strip=all&ssl=1)

**5)** The storage bucket is now set up. Copy the storage bucket ID because you’ll need it later (copy only the section highlighted with a red rectangle as shown below).

<aside>
💡 Note : make sure to remove the `gs://` part before the bucketID

</aside>

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Storage-Bucket-Set-Up-ID.png?resize=825%2C365&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/11/Firebase-Storage-Bucket-Set-Up-ID.png?resize=825%2C365&quality=100&strip=all&ssl=1)

#### **Storage Rules**

We’ll change the storage rules so that only authenticated users can upload files to the storage bucket. Select the **Rules** tab.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2022/10/storage-rules.png?resize=704%2C401&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2022/10/storage-rules.png?resize=704%2C401&quality=100&strip=all&ssl=1)

Change your database rules. Copy the code below, paste it over the existing code, and continue.

```jsx
rules_version = '2';
service firebase.storage {
  match /b/{bucket}/o {
    match /{allPaths=**} {
      allow read, write: if request.auth !=null;
    }
  }
}
```

### **4. Get Project API Key**

To interface with your Firebase project using the ESP32S3, you need to get your project API key. Follow the next steps to get your project API key.

**1)** On the left sidebar, click on **Project Settings**.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-project-settings.png?resize=750%2C350&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-project-settings.png?resize=750%2C350&quality=100&strip=all&ssl=1)

**2)** Copy the Web API Key to a safe place because you’ll need it later.

![https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-Project-Settings-Web-API-Key.png?resize=750%2C393&quality=100&strip=all&ssl=1](https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Firebase-Project-Settings-Web-API-Key.png?resize=750%2C393&quality=100&strip=all&ssl=1)

## Part 2: Preparing the ESP32S3

### Create a PlatformIO project

By this point you already know how to setup a new ESP32 project for the XIAO ESP32S3 board.

### **Installing the Firebase ESP32 library**

The [Firebase-ESP-Client library](https://github.com/mobizt/Firebase-ESP-Client) provides several examples to interface with Firebase services. It provides an example that shows how to send files to Firebase Storage. Our code will be based on that example. So, you need to make sure you have that library installed.

#### **Installation – VS Code + PlatformIO**

If you’re using VS Code with the PlatformIO extension, click on the **PIO Home** icon and select the **Libraries tab**. Search for “**Firebase ESP Client “**. Select the **Firebase Arduino Client Library for ESP8266 and ESP32**.

![https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-Library-VS-Code-1.png?resize=828%2C745&quality=100&strip=all&ssl=1](https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-Library-VS-Code-1.png?resize=828%2C745&quality=100&strip=all&ssl=1)

Then, click **Add to Project** and select the project you’re working on.

![https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-Library-VS-Code-2.png?resize=828%2C411&quality=100&strip=all&ssl=1](https://i1.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-Library-VS-Code-2.png?resize=828%2C411&quality=100&strip=all&ssl=1)

Also, change the monitor speed to 115200 by adding the following line to the platformio.ini file of your project:

```
monitor_speed = 115200
```

#### **Installation – Arduino IDE**

If you’re using Arduino IDE, follow the next steps to install the library.

1. Go to **Sketch** > **Include Library** > **Manage Libraries**
2. Search for *Firebase ESP Client* and install the *Firebase Arduino Client Library for ESP8266 and ESP32* by Mobizt.

![https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-ESP-Client-Library-Arduino-IDE-f.png?resize=786%2C443&quality=100&strip=all&ssl=1](https://i2.wp.com/randomnerdtutorials.com/wp-content/uploads/2021/09/Install-Firebase-ESP-Client-Library-Arduino-IDE-f.png?resize=786%2C443&quality=100&strip=all&ssl=1)

Now, you’re all set to start programming the ESP32S3 board to send pictures to Firebase Storage.

### Code Setup

1. From the [code in the repository](https://github.com/GIXLabs/TECHIN514_W24/tree/lab7_tinyml/Lab7_tinyml) copy and paste the contents of `main.cpp` file.
2. You will notice that there is another file `src/camera_pins.h` . This file is required to setup the camera pins with the ESP32.
3. Copy the `camera_pins.h` file in the `src/` folder alongside your `main.cpp` file.

### Setting up a switch with the ESP32

1. Connect the switch pins to GPIO1 and GND on the ESP32.
2. Set the `BUTTON_PIN` to the GPIO number we connected to.

```cpp
#define BUTTON_PIN 1
```

## Part 3: Capturing and Uploading Images

### Configuring ESP32S3 for Firebase

#### **Network Credentials**

Insert your network credentials in the following variables so that the ESP can connect to the internet and communicate with Firebase.

<aside>
💡 Note: You need to register your ESP32 with UW MPSK network. You

</aside>

```c
//Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```

#### **Firebase Project API Key**

Insert your Firebase project API key—see this section: [4) Get Project API Key](https://randomnerdtutorials.com/esp32-cam-save-picture-firebase-storage/#get-API-Key).

```c
// Insert Firebase project API Key
#define API_KEY "REPLACE_WITH_YOUR_FIREBASE_PROJECT_API_KEY."
```

#### **User Email and Password**

Insert the authorized email and the corresponding password—see this section: [2) Set Authentication Methods](https://randomnerdtutorials.com/esp32-cam-save-picture-firebase-storage/#set-authentication-methods).

```c
#define USER_EMAIL "REPLACE_WITH_THE_AUTHORIZED_USER_EMAIL"
#define USER_PASSWORD "REPLACE_WITH_THE_AUTHORIZED_USER_PASSWORD"
```

#### **Firebase Storage Bucket ID**

Insert the Firebase storage bucket ID, e.g *bucket-name.appspot.com*. In my case, it is esp-firebase-demo.appspot.com. (remove any slashes “/” at the end or at the beginning of the bucket ID, otherwise, it will not work).

```c
define STORAGE_BUCKET_ID "REPLACE_WITH_YOUR_STORAGE_BUCKET_ID"
```

#### **Picture Path**

The FILE_PHOTO_PATH variable defines the LittleFS path where the picture will be saved. It will be saved with the name photo.jpg.

<aside>
💡 **Note:** LittleFS is a simple way for small devices like the ESP32S3 to save and manage files. It's made to work well even with limited space and to be very reliable.

</aside>

```c
#define FILE_PHOTO "/photo.jpg"
```

We also have a variable to hold the path where the picture will be saved on the Storage Bucket on Firebase.

```c
#define BUCKET_PHOTO "/data/photo.jpg"
```

#### Build and Upload

Now your code is ready to build and upload to the ESP32. You know how to do this.

<aside>
💡 Note : Once your code is uploaded disconnect your ESP32 from your laptop reconnect it again. We have noticed this helps with Wifi connectivity issues.

</aside>

#### Capturing and Uploading Images with the Button/Switch

- Now when you click the button or the switch you should see your images being uploaded to your firebase

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled.png)

## Part 4: Downloading Images for Model Training

This part of the lab guide provides instructions on how to use **`gsutil`** from the Google Cloud software development kit (SDK) to download your dataset images from Firebase Storage. This is a crucial step for preparing your dataset for model training with Edge Impulse.

### **Setting Up Google Cloud SDK**

Before downloading your images, you must install the Google Cloud SDK, which includes **`gsutil`**, a powerful tool for interacting with Google Cloud Storage.

#### For Mac Users:

**Downloading and Installing `gsutil`**

1. Download the Cloud SDK installer from the [Google Cloud **`gsutil`**](https://cloud.google.com/storage/docs/gsutil_install) [installation page](https://cloud.google.com/storage/docs/gsutil_install#mac).
2. Extract the downloaded **`google-cloud-sdk.tar.gz`** file.
3. Navigate to the extracted **`google-cloud-sdk`** folder and run the **`install.sh`** script.
4. Open a new terminal window to ensure the PATH is updated.

#### Logging in to Google Cloud

- Authenticate your Google Cloud session by running **`gcloud auth login`** in the terminal.
- Use **`gsutil -m cp -r gs://your-storage-bucket-url ~/Downloads`**  to download images from your Firebase storage bucket.

#### **For Windows Users:**

**Downloading and Installing `gsutil`**

1. Download the Cloud SDK installer for Windows from the [Google Cloud **`gsutil`**](https://cloud.google.com/storage/docs/gsutil_install) [installation page](https://cloud.google.com/storage/docs/gsutil_install#windows).
2. Run the downloaded **`google-cloud-sdk-installer.exe`** file and follow the on-screen instructions. The installer will add **`gsutil`** to your PATH automatically.
3. Click `Next->` and follow through the installation process.
4. Once you click `Finish` a new shell will open asking you to login to Google Cloud CLI.
5. Press `y` and login with your google account you used to setup your Firebase account.

#### Logging in to Google Cloud

- Initialize the Cloud SDK by opening a new command prompt.
- Go into your Documents folder using `cd Documents`
- Download images to your machine using
  - **`gsutil -m cp -r gs://your-storage-bucket-url .`**
    eg . `gsutil -m cp -r gs://esp32cam-5b8b7.appspot.com .`
- It should look like this

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%201.png)

### **Notes**

- Ensure you have the necessary permissions to access the specified Cloud Storage bucket.
- The **`m`** flag in the **`gsutil`** command enables parallel transfers, making the download process faster.

By following these steps, you will have all the images needed for the next phase of the lab, where you will upload these images to Edge Impulse for training your machine learning model.

## Part 5: Training Your ML Model with Edge Impulse

### Creating an Account and a New Project

Please set your project as public so it will be accessible to us during grading.

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%202.png)

### Uploading Your Dataset

- Preparing Your Dataset for Upload
- Uploading Images to Edge Impulse

Click *Add existing data* to upload your collected data to Edge Impulse

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%203.png)

Then click *Upload Data:*

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%204.png)

Upload the images you collected to Edge Impulse. **Please set them as training data ONLY,** as we will give you our test dataset to be uploaded. Finally, set the labels of the images you uploaded (cup/nocup).

Finally, please upload the test data we provide to you (of course, **as the testing dataset**). Make sure you set the labels accordingly.

### Create Impulse

After you uploading all your collected training data, you can go to *Impulse design → Create Impulse*

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%205.png)

Now you should add the three blocks (input, processing, and learning) accordingly:

1. For the input block, select **Images**, and then set the width and height to **either 96 or 160**
   - Intuitively, images with higher resolution will get **relatively higher model accuracy/performance**, but it would **consume more time and memory** during inference
   - You can also try-out different resize modes here: fit shortest/longest axis, or squash. There’s no best solution for this selection. You just need to try them out if you want to improve your model performance.
2. For the processing block, select **Images** as well
3. Finally, select the learning block, which is the machine learning model backbone you’re gonna use:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%206.png)

Here, we recommend you choose the first one (transfer learning for Images) as the learning block, as it is the only option here provided by Edge Impulse for image classification tasks.

Finally, make sure you’ve clicked **Save Impulse** on the right of your screen so that all your configurations would be saved.

### Generate Features

Next, go to the processing block and generate the features from the image data:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%207.png)

If everything goes correctly, you’ll see the screen below:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%208.png)

The scatter graph on the right would be helpful for you to check the data distribution of your collected data against different labels (a.k.a. ground truths).

### Start Model Training

Afterward, you can go to the Transfer Learning block to start the model training:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%209.png)

There are mainly three hyperparameters you can adjust here:

- Training epochs: how many time will be full data being applied to the model for training. The more the epochs, the longer training time it would consume, and the more the model would be **converged to the training data.**
  - Attention: model converging to the training data doesn’t ensure high accuracy on the validation/testing data, as **overfitting is definitely going to occur as the training goes on**
- Learning rate: intuitively this determines how sensitive the training would be (how fast the convergence would happen).
  - Higher learning rate would make the converge be faster at start, but the model would be more easily to converge to a local optimal rather than a global optimal
- Data augmentation: whether automatically generating augmented data (by simple transformation) based on the training data.
  - This is a super effective technique of tackling the overfitting issue. We recommend you turn this feature off for your first training. If you observe that overfitting occurs, then you can turn this feature on and see the difference.

In the meantime, you can also click the *Choose a different model* on the bottom-left of your screen to try-out different models:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2010.png)

Make sure the model’s expected image resolution matches your input block’s setting. In the meantime, you can check the expected RAM usage of different models. Models consuming more RAM would probably perform better.

After setting all the hyperparameters, you can click **Start Training** to train your model.

### Evaluating the model

For evaluation, **make sure you have uploaded the [test dataset](https://github.com/GIXLabs/TECHIN514_W24/tree/main/Lab7_tinyml/test_dataset) provided by us first**. Then you can click *Model testing* on the left, then click the **Classify all** button.

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2011.png)

## Part 6: Deploying Your ML Model

### Deployment Options in Edge Impulse

Now you can click the *Deployment* tab on the left to deploy your model to your ESP32S3!

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2012.png)

Make sure you select the **Arduino library** as the deployment option.

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2013.png)

For model optimizations, you can select either the quantized (int8) model or unoptimized (float32) model. The former one would cost less time and RAM during inference, and the latter one would probably get higher accuracy.

You can check the **estimated total latency and RAM consumption** from the table. By default, we recommend you select the unoptimized (float32) model as it would give you better accuracy, and the time/memory consumption is also suitable for ESP32S3.

After you click the **Build** button, the library will be built and then automatically downloaded to your laptop.

### Preparing the ESP32S3 for Deployment

1. Unzip the library you just downloaded from Edge Impulse
2. Copy the files/folders below to the camera-ml project’s *src* folder from our GitHub repo:
   - xxxxxx.h (the name should be your Edge Impulse project’s name)
   - edge-impulse-sdk
   - model-parameters
   - tflite-model
3. Edit the first line of code to include your xxxxxx.h header file.

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2014.png)

1. Go to src/edge-impulse-sdk/porting/ei_classifier_porting.h, and edit the value of **EI_MAX_OVERFLOW_BUFFER_COUNT** as 8192 (because your ESP32S3 has 8M PSRAM!)

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2015.png)

1. Try building and uploading the project to deploy your model to your ESP32S3
   - Attention: **there could be at least two errors occurring after your first build**. Try solving them on yourselves by simply reading the build log and error prompts. Those issues are relatively easy to be solved.
2. If everything goes correctly, you’ll see the following outputs from your Serial monitor:

![Untitled](TinyML%20Lab%20c64254f27b174ac1b2b765f6a060623a/Untitled%2016.png)

## What to be Submitted on Canvas

A single PDF, containing:

* Screenshot of your serial monitor output when an image is captured and uploaded by your ESP32 to Firebase
* Screenshot of your firebase storage showing your uploaded images
* Screenshot of your terminal output of your images being downloaded from Firebase using Google Cloud CLI
* Screenshot of the scatter plot after you generate features on Edge Impulse
* Screenshot of your model's accuracy on the test data set
  * Also describe what pre-process setups and hyperparameters you have tried. What's their influence on the model's performance.
* Screenshot of inference running on your ESP32
  * Describe what bugs you faced when deploying the model and how you fixed them.

## External Resources

* [ESP32-CAM Save Picture to Cloud Storage](https://randomnerdtutorials.com/esp32-cam-save-picture-firebase-storage/)
* [Edge Impulse Image Classification Tutorial](https://docs.edgeimpulse.com/docs/tutorials/end-to-end-tutorials/image-classification)
* [Google Cloud SDK Cheat Sheet](https://cloud.google.com/sdk/docs/cheatsheet)
