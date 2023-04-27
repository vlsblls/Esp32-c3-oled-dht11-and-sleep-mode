#include "DHT.h"
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 

Adafruit_SSD1306 myDisplay(128, 64, &Wire);
#define uS_TO_S_FACTOR 1000000ULL
#define TIME_TO_SLEEP  10 

RTC_DATA_ATTR int bootCount = 0;

#define DHTPIN 7   

#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200); 
   while (!Serial);

  Serial.println("DHTxx test!");
 
  dht.begin();
  myDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

}

void loop() {
  delay(1000);

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.println();

  myDisplay.clearDisplay();

   myDisplay.setTextSize(2);
   myDisplay.setCursor(10,0);
   myDisplay.setTextColor(WHITE);
   myDisplay.println("T = ");
   myDisplay.setTextSize(2);
   myDisplay.setCursor(50,0);
   myDisplay.println(t);
   myDisplay.setTextSize(2);
   myDisplay.setCursor(10,40);
   myDisplay.setTextColor(WHITE);
   myDisplay.println("H = ");
   myDisplay.setTextSize(2);
   myDisplay.setCursor(50,40);
   myDisplay.println(h);
    myDisplay.display();

  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  print_wakeup_reason();

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +
  " Seconds");

  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}  
