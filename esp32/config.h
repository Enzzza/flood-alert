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
