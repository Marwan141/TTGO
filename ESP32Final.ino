#include "WiFi.h"
#include <TFT_eSPI.h>
#include <time.h>
#include "HTTPClient.h"
#include <ArduinoJson.h>
//Icons
#include "smallsunrise.h"
#include "sunrise.h"
#include "FullMoon.h"
#include "MoonWaningCrescent.h"
#include "NewMoon.h"
#include "NoMoon.h"
#include "WaningMoon.h"
#include "Waxing Crescent Moon.h"
#include "WaxingMoon.h"
#include "FirstQuarter.h"
//Icons

const char* ntpserver = "time.google.com"; 
const int daylight = 3600;
const long gmt = 0;
const char* ssid = "Glide-US-Resident";
const char* password = "StopBlankSpill";
int count = 0;


const char* test_root_ca = \ 
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n" \
    "TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n" \
    "cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n" \
    "WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n" \
    "ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n" \
    "MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n" \
    "h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n" \
    "0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n" \
    "A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n" \
    "T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n" \
    "B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n" \
    "B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n" \
    "KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n" \
    "OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n" \
    "jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n" \
    "qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n" \
    "rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n" \
    "HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n" \
    "hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n" \
    "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n" \
    "3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n" \
    "NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n" \
    "ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n" \
    "TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n" \
    "jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n" \
    "oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n" \
    "4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n" \
    "mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n" \
    "emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n" \
    "-----END CERTIFICATE-----\n";


DynamicJsonDocument doc(20000);

TFT_eSPI tft = TFT_eSPI();

HTTPClient http;
WiFiClientSecure client; //HTTPS equivalent of WiFiClient 
void printLocalTime(){
  struct tm timeInfo;
  if(!getLocalTime(&timeInfo)){
    Serial.println("Error occured whilst fetching time.");
    return;
  }
  //Generated this using the ArduinoJSON assistant and only selected data needed for my ESP32
  const char* resolvedAddress = doc["resolvedAddress"]; // "London, England, United Kingdom"

  JsonObject days_0 = doc["days"][0];
  float temp = days_0["temp"]; // 12.2
  float days_0_feelslike = days_0["feelslike"]; // 12.2
  int days_0_uvindex = days_0["uvindex"]; // 4
  const char* days_0_sunrise = days_0["sunrise"]; // "07:27:17"
  long days_0_sunriseEpoch = days_0["sunriseEpoch"]; // 1697524037
  const char* sunset = days_0["sunset"]; // "18:03:45"
  long days_0_sunsetEpoch = days_0["sunsetEpoch"]; // 1697562225
  float moonphase = days_0["moonphase"]; // 0.1
  const char* days_0_conditions = days_0["conditions"]; // "Partially cloudy"
  const char* decrp = days_0["description"]; // "Partly cloudy throughout the day."
  const char* days_0_icon = days_0["icon"]; // "partly-cloudy-day"
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(tft.height()/2, 0);
  tft.println(&timeInfo, "%A");
  Serial.println(&timeInfo, "%A");
  Serial.println(&timeInfo, "%B %d %Y");
  Serial.println(&timeInfo, "%H:%M:%S");
  tft.setCursor(tft.height()/2, 15);
  tft.println(&timeInfo, "%B %d %Y");
  tft.setCursor(tft.height()/2, 30);
  tft.println(&timeInfo, "%H:%M:%S");
  tft.drawLine(0,45,500,45,TFT_WHITE);
  tft.setCursor(0,50);
  tft.print("Temperature:");
  tft.setCursor(0, 70);
  tft.print(temp);
  tft.print("C");
  tft.setCursor(tft.height() + 20, 50);
  tft.print("Moonphase:");
  tft.setCursor(tft.height() + 20,70);
  tft.print(moonphase);
  tft.println("");
  tft.setCursor(0, 90);
  tft.print("Sunset time:");
  tft.setCursor(0, 110);
  tft.println(sunset);

  tft.setCursor(40, 110);
  tft.setSwapBytes(true);
  tft.pushImage(70,85,48,48,smallsunrise);
  if (moonphase == 0){
    tft.pushImage(tft.height() + 20,85,48,48,NewMoon);
  }
  else if (moonphase >= 0.1 and moonphase <= 0.24){
    tft.pushImage(tft.height() + 20,85,48,48,WaxingCrescent);
  }
  else if (moonphase == 0.25){
    tft.pushImage(tft.height() + 20,85,48,48,Moon_First_Quarter);
  }
  else if (moonphase >= 0.26 and moonphase <= 0.49){
    tft.pushImage(tft.height() + 20,85,48,48,WaxingMoon);
  }
  else if (moonphase == 0.5){
    tft.pushImage(tft.height() + 20,85,48,48,FullMoon);
  }
  else if (moonphase >= 0.51 and moonphase <= 0.75){
    tft.pushImage(tft.height() + 20,85,48,48,WaningMoon);
  }
  else{
    tft.pushImage(tft.height() + 20,85,48,48,WaningCrescent);
  }
  
}

void getJSON(){
 
  if(WiFi.status() == WL_CONNECTED){
    
    const char* ServerPath = "https://weather.visualcrossing.com/VisualCrossingWebServices/rest/services/timeline/London/today?unitGroup=metric&key=FUEEJL7LSZ9V5EMR2R3CHLXE8&contentType=json";
    client.setCACert(test_root_ca); //Use certificate to connect to the HTTPS server.
    http.useHTTP10(true);
    

    http.begin(client, ServerPath);
    http.GET();

  
    DeserializationError error = deserializeJson(doc, http.getString());
    Serial.print("\n");
    Serial.print(http.getString());



    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
    
    http.end();


}
else{
  Serial.print("WiFi not connected.");
}
}

void IRAM_ATTR addButton(){
  count += 1;
  Serial.print("You've pressed me: ");
  Serial.print(count);
  Serial.print(" times.\n");


}



void setup() {
  tft.init();
  tft.setRotation(1);
  Serial.begin(115200);

  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(("Connecting to WiFi..."));
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  pinMode(0, INPUT);

  attachInterrupt(0, addButton, FALLING);


  configTime(gmt, daylight, ntpserver);
  getJSON();
  printLocalTime();
  
 
}

void loop() {
  delay(1000);
  printLocalTime();

 
}