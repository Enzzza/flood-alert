#define RED_PIN 16
#define GREEN_LED 19
#define BLUE_PIN 17
#define BUZZER_PIN 13
#define FLOAT_PIN 27

#define ESP32_HOSTNAME "FloodDetectionAlarm"
#define BLYNK_TIMEOUT_MS  500
#define BLYNK_HEARTBEAT   17
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include "config.h"

bool isDeviceOnline = false;

class Flasher
{
    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    int ledState;                 // ledState used to set the LED
    unsigned long previousMillis;   // will store last time LED was updated

    // Constructor - creates a Flasher
    // and initializes the member variables and state
  public:
    Flasher(int pin, long on, long off)
    {
      ledPin = pin;
      pinMode(ledPin, OUTPUT);

      OnTime = on;
      OffTime = off;

      ledState = LOW;
      previousMillis = 0;
    }

    void Update()
    {
      // check to see if it's time to change the state of the LED
      unsigned long currentMillis = millis();

      if ((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
      {
        ledState = LOW;  // Turn it off
        previousMillis = currentMillis;  // Remember the time
        digitalWrite(ledPin, ledState);  // Update the actual LED
      }
      else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
      {
        ledState = HIGH;  // turn it on
        previousMillis = currentMillis;   // Remember the time
        digitalWrite(ledPin, ledState);   // Update the actual LED
      }
    }

    void setOnTime(int on) {
      OnTime = on;
    }

    void setOffTime(int off) {
      OffTime = off;
    }

    void Stop() {
      digitalWrite(ledPin, LOW);
    }
};

class Alarm
{
    Flasher _redLed;
    Flasher _blueLed;


    int _redPin;
    int _bluePin;
    int _buzzerPin;


    static const int _maxNumOfEvents{10};
    static const int _delayTime{10};

    int _sentEvents;
    unsigned long _timeOfLastSentEvent;


    int _buzzerFreq;
    const static int _buzzerChannel {0};
    const static int _buzzerResolution {8};


  public:


    Alarm(int redPin, int bluePin, int buzzerPin):
      _redLed(redPin, 200, 200),
      _blueLed(bluePin, 400, 400){


      _buzzerPin = buzzerPin;
      _redPin = redPin;
      _bluePin = bluePin;

      _sentEvents = 0;
      _timeOfLastSentEvent = 0;

      _buzzerFreq = 2000;
      ledcSetup(_buzzerChannel, _buzzerFreq, _buzzerResolution);
      ledcAttachPin(_buzzerPin, _buzzerChannel);

    }

    void turnOnLeds() {
      _redLed.Update();
      _blueLed.Update();
    }

    void turnOffLeds() {
      _redLed.Stop();
      _blueLed.Stop();
    }

    void sendEvent() {
      unsigned long currentMillis = millis();
      unsigned long timePassed = currentMillis - _timeOfLastSentEvent;
      if (_sentEvents == 0) {
        Blynk.notify("Alert: Water is rising!");
        _sentEvents++;
        _timeOfLastSentEvent = currentMillis;
        Serial.println("Water is rising");
      }
      else if (timePassed  > (_delayTime * 6 * 1000) &&  _sentEvents < _maxNumOfEvents) {
        Blynk.notify("Alert: Water is rising!");
        _sentEvents++;
        _timeOfLastSentEvent = currentMillis;
        Serial.println("Water is rising");
      }

    }

    void turnOnBuzzer() {
      ledcWriteTone(_buzzerChannel, 2000);
    }

    void turnOffBuzzer() {
      ledcWriteTone(_buzzerChannel, LOW);
    }

    void runAlarm() {
      turnOnLeds();
      turnOnBuzzer();
      if (isDeviceOnline == true) {
        sendEvent();
      }

    }

    void stopAlarm() {
      _sentEvents = 0;
      turnOffLeds();
      turnOffBuzzer();
    }

};

BlynkTimer timer;
volatile bool isFlooding = false;



portMUX_TYPE synch = portMUX_INITIALIZER_UNLOCKED;
#define DEBOUNCE_TIME 250
volatile uint32_t DebounceTimer = 0;

void IRAM_ATTR detectsFlooding() {
  portENTER_CRITICAL(&synch);

  if (millis() - DEBOUNCE_TIME >= DebounceTimer) {
    DebounceTimer = millis();
    isFlooding = true;

    Serial.println("FLOOD DETECTED!!!");
    digitalWrite(GREEN_LED, LOW);
  }

  portEXIT_CRITICAL(&synch);
}

Alarm alarmer(RED_PIN, BLUE_PIN, BUZZER_PIN);
int alarmTimerID = -2;
int loopTimerID = -3;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  WiFi.setHostname(ESP32_HOSTNAME);
  WiFi.mode(WIFI_STA);
  Blynk.config(auth);
  CheckConnection();
  timer.setInterval(15000L, CheckConnection);

  pinMode(FLOAT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOAT_PIN), detectsFlooding, FALLING);
  loopTimerID = timer.setInterval(1000L, floodAlertOn);


  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, HIGH);
}

void loop()
{
  if (Blynk.connected()) {
    Blynk.run();
  }
  timer.run();
}


void floodAlertOn() {
  if (alarmTimerID == -2) {
    alarmTimerID = timer.setInterval(50L, alarmCallback);
    if (timer.isEnabled(alarmTimerID)) {
      timer.disable(alarmTimerID);
    }
  }

  if (isFlooding == true) {
    timer.enable(alarmTimerID);
    timer.disable(loopTimerID);
  }


}

void alarmCallback() {
  int state = digitalRead(FLOAT_PIN);
  if (state == 0) {
    isFlooding = true;
  } else {
    isFlooding = false;
  }

  if (isFlooding == false) {
    timer.disable(alarmTimerID);
    timer.enable(loopTimerID);
    alarmer.stopAlarm();
    digitalWrite(GREEN_LED, HIGH);
  } else {
    alarmer.runAlarm();
  }

}


void CheckConnection() {
  if (!Blynk.connected()) {
    isDeviceOnline = false;
    yield();
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("Not connected to WiFi! Connecting...");

      // Configures static IP address
      if (!WiFi.config(esp32_ip, gateway_ip, subnet_mask, primary_DNS, secondary_DNS)) {
        Serial.println("Static IP Failed to configure");
      }

      WiFi.begin(ssid, pass);
      delay(400); //give it some time to connect
      if (WiFi.status() != WL_CONNECTED)
      {
        Serial.println("Can't connect to WiFi!");
        isDeviceOnline = false;
      }
      else
      {
        Serial.println("Connected to WiFi!");
      }
    }

    if ( WiFi.status() == WL_CONNECTED && !Blynk.connected() )
    {
      Serial.println("Not connected to Blynk Server! Connecting...");
      Blynk.connect(); // It has 3 attempts of the defined BLYNK_TIMEOUT_MS to connect to the server, otherwise it goes to the next line
      if (!Blynk.connected()) {
        Serial.println("Connection failed!");
        isDeviceOnline = false;
      }
      else
      {
        isDeviceOnline = true;
      }
    }
  }
  else {
    Serial.println("Connected to Blynk server!");
    isDeviceOnline = true;
  }
}
