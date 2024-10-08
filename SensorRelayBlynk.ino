#define BLYNK_TEMPLATE_ID "TMPL67xb9oa88"
#define BLYNK_TEMPLATE_NAME "Smart Farm"
#define BLYNK_AUTH_TOKEN "UN9JwYFhgHSt4SdZX5KUiC5GHlw5WhZW"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "internet";
char pass[] = "123456789";
BlynkTimer timer;


#define SENSOR_PIN A0
#define RELAY_PIN D5
#define ON HIGH
#define OFF LOW

int Value;
unsigned long period = 500;
unsigned long last_time = 0;

bool check = false;

int SensorValue;




void myTimerEvent()
{
  Blynk.virtualWrite(V3, SensorValue);
  Blynk.virtualWrite(V5, millis() / 1000);
  Serial.println("Runtime : " + String(millis() / 1000));
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial.print("V1 Slider value is: ");
  Serial.println(pinValue);
  if (pinValue == 1) {
    check = true;
  }
  if (pinValue == 0) {
    check = false;
    digitalWrite(RELAY_PIN, OFF);
    Blynk.virtualWrite(V2, 0);
  }
}
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  Serial.print("V2 Slider value is: ");
  Serial.println(pinValue);
  if (check == false) {
    if (pinValue == 1) {
      digitalWrite(RELAY_PIN, ON);
    }
    if (pinValue == 0) {
      digitalWrite(RELAY_PIN, OFF);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  for (int Rx = 10; Rx >= 0; Rx--) {
    Serial.println("Start in : " + String(Rx));
    delay(100);
  }
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, ON);
  delay(1000);
  digitalWrite(RELAY_PIN, OFF);
  delay(1000);
  pinMode(SENSOR_PIN, INPUT);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();
  if ( millis() - last_time > period) {
    last_time = millis();
    Value = analogRead(SENSOR_PIN);
    SensorValue = map(Value, 1024, 0, 0, 100);
    Serial.println("SensorValue : " + String(SensorValue));
    if (check == true) {
      if (SensorValue <= 40) {
        digitalWrite(RELAY_PIN, ON);
      }
      if (SensorValue >= 41) {
        digitalWrite(RELAY_PIN, OFF);
      }
    }
  }
}
