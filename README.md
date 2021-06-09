## _Flood detection system_
 
Last winter we almost had a problem when the autumn leaves clogged the drain, luckily I reacted on time. That's why i decided to build flood alert system that would notify me over WiFi if there is flood detection.

Parts needed: 

- Esp32 Microcontroller 
- PP Float Switch
- Active Buzzer
- Leds
- 220Ω Resistors
- Cables
<br>
<br>
***
<br>

![img01](https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img01.jpg)

<p align = "center">
Basic schematic diagram
</p>

Idea behind this system is when PP Float Switch detect water rising it sends HIGH signal to ESP32 microcontroller. After ESP32 detects this signal it will send push notification over WiFi to the Blynk server, Blynk server will send notification to our device. Also ESP32 will play alarm sound  and it will turn on led lights. When ESP32 is not detecting flood it will indicate that with green led light.

<br>

Next step is to build prototype.
<br>

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img02.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img02.jpg" width="400"/>

<p align = "center">
Prototype
</p>

<br>

Now we will put it all together. I will use IP55 enclosure box because this box won't be directly exposed to the rain. If your box will be exposed to rain use IP66, IP67 or IP68 type.


<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img03.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img03.jpg" width="400"/>

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img04.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img04.jpg" width="400"/>

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img05.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img05.jpg" width="400"/>


<br>
<br>

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img06.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img06.jpg" width="400" />

<p align = "center">
Mounting box on the wall
</p>

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img07.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img07.jpg" width="400" />

<p align = "center">
Mounting sensor to the bracket
</p>

After we finished hardware part of project now is time to setup [Blynk](https://docs.blynk.cc/) server.

Download Blynk App for your smartphone.


1. Create a Blynk Account
After you download the Blynk App, you’ll need to create a New Blynk account. This account is separate from the accounts used for the Blynk Forums, in case you already have one.


<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img08.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img08.jpg" width="200" height="400" />

2. Create a New Project
After you’ve successfully logged into your account, start by creating a new project.


<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img09.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img09.jpg" width="200" height="400" />


3. Choose Your Hardware
We will use Esp32

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img10.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img10.jpg" width="200" height="400" />

4. Auth Token
Auth Token is a unique identifier which is needed to connect your hardware to your smartphone. Every new project you create will have its own Auth Token. You’ll get Auth Token automatically on your email after project creation. You can also copy it manually. Click on devices section and selected required device :

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img11.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img11.jpg" width="200" height="400" />

And you’ll see token :

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img12.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img12.jpg" width="200" height="400" />

NOTE: Don’t share your Auth Token with anyone, unless you want someone to have access to your hardware.


Now press the “Create” button.

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img13.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img13.jpg" width="200" height="400" />

5. Add a Widget

We will use notification widget

Push Notification widget allows you to send push notification from your hardware to your device.
<br>
<br>
Example code:

```powershell
Blynk.notify("Hey, Blynkers! My hardware can push now!");
```


In esp32 folder there is config.h file copy token you got from step 4 into this file.

```powershell
char ssid[]            = "YOUR SSID";
char pass[]            = "YOUR PASSWORD";
char auth[]            = "YOUR BLYNK AUTH TOKEN";
char serveraddr[]      = "blynk-cloud.com";
unsigned int port      = 80;

// Uncomment this if you are hosting your blynk server  locally
//IPAddress serveraddr  (192,   168,   0,   55); // local server ip 
//unsigned int port      = 1234; // port of your local server

// Static IP
IPAddress esp32_ip ( 192,  168,   0,  100);
IPAddress gateway_ip ( 192,  168,   0,   1);
IPAddress subnet_mask(255, 255, 255,     0);
IPAddress primary_DNS(8, 8, 8, 8);   //optional
IPAddress secondary_DNS(8, 8, 4, 4); //optional

```

Now we can recive push notifications on our smartphone.

<img src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img14.jpg" data-canonical-src="https://raw.githubusercontent.com/Enzzza/flood-alert/main/media/img14.jpg" width="200" height="400" />