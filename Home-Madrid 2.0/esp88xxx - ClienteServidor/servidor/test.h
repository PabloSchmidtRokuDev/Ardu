#include "WiFiTask.h"
#include "FS.h"
#include "CloudTask.h"
#include <cstring>

static const char WIFINETS_JSON_FILE[] PROGMEM = "/conf/wifinets.json";

WiFiTask::WiFiTask() : Task(), lastCheck(TimeKeeper::tkNow()), firstRun(true) {
  
}

void WiFiTask::loop()  {
  if (ServerTask::initializationFinished()) {
      time_t nowTime = TimeKeeper::tkNow();
      const time_t diffTime = nowTime - lastCheck;
      if (diffTime > WIFI_CHECK_SECONDS || firstRun) { //should see if we are connected
          firstRun = false;
          if(WiFi.status() != WL_CONNECTED) {
              yield();

              String fileName = String(FPSTR(WIFINETS_JSON_FILE));
              loopWiFis(fileName);
          } else {
            CloudConf conf;
            bool readOk = CloudTask::readCloudConf(conf);
            if (!readOk) { 
              Serial.print(F("Could not read cloud conf file at WiFiTask::loop()"));
            } else if (!conf.isAllValid()) {
              Serial.print(F("Cloud conf file is not valid at WiFiTask::loop()"));
            } else if (!conf.enabled) {
              Serial.print(F("Cloud access is disabled at conf file (WiFiTask::loop())"));
            } else {
              //will check service availability
              if(!CloudTask::cloudServiceIsReachable()) {
                yield();
                //our service is not available, see if we are connected to the internet
                if(!CloudTask::isInternetConnected()) {
                  yield();
                  //reconnect to wifi to try again later
                  String fileName = String(FPSTR(WIFINETS_JSON_FILE));
                  loopWiFis(fileName);
                }
              }
            }
          }
          lastCheck= nowTime;
      } else {
          this->delay((WIFI_CHECK_SECONDS - diffTime)*1000);
      }
  } else yield();
}

int WiFiTask::isAvailable(const char* ssid) {
  String ssidStr = String(ssid);
  int nNets = WiFi.scanComplete();
  for (int i = 0; i < nNets; i++) {
    String ssidRet = String(WiFi.SSID(i));
    if (ssidStr.compareTo(ssidRet) == 0) {
        return i;
    }
  }
  return -1;
}

bool WiFiTask::addToTop(const String& newssid, const String& newpass) {
  String fileName = String(FPSTR(WIFINETS_JSON_FILE));
  File confFile = SPIFFS.open(fileName, "r");
  char ssids[10][33];
  memset(ssids, 0, 10*33*sizeof(char));
  char pass[10][65];
  memset(pass, 0, 10*65*sizeof(char));
  int ssidFoundAt = -1;
  int arraySize = 0;
  const size_t capacity = JSON_ARRAY_SIZE(MAX_WIFI_ELEMS) + JSON_OBJECT_SIZE(1) + MAX_WIFI_ELEMS*JSON_OBJECT_SIZE(2) + 121*MAX_WIFI_ELEMS;
  if (confFile) {
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject& root = jsonBuffer.parseObject(confFile);
    confFile.close();
    JsonArray& wifinets = root["wifinets"];
    arraySize = wifinets.size();
    for (int i = 0; i < arraySize && i < 10; i++) {
      strncpy(ssids[i], wifinets[i]["ssid"], 32);
      strncpy(pass[i], wifinets[i]["pass"], 64);
      if (strncmp(ssids[i], newssid.c_str(), 32) == 0 && ssidFoundAt == -1) {
        ssidFoundAt = i;
      }
    }
  }
  DynamicJsonBuffer jsonBuffer(capacity);
  JsonObject& root = jsonBuffer.createObject();
  JsonArray& wifinets = root.createNestedArray("wifinets");
  JsonObject& firstEntry = jsonBuffer.createObject();
  firstEntry["ssid"] = newssid;
  firstEntry["pass"] = newpass;
  wifinets.add(firstEntry);
  int copied = 0;
  for (int i = 0; i < arraySize && copied < 10 && i < 10; i++) {
    if (i == ssidFoundAt) continue;
    JsonObject& entry = jsonBuffer.createObject();
    entry["ssid"] = ssids[i];
    entry["pass"] = pass[i];
    wifinets.add(entry);
    copied++;
  }
  confFile = SPIFFS.open(fileName, "w");
  if (!confFile) return false;
  root.printTo(confFile);
  confFile.flush();
  confFile.close();
  return true; 
}

void WiFiTask::loopWiFis(String& fileName) {
    File confFile = SPIFFS.open(fileName, "r");
    if (!confFile) return;
    const size_t capacity = JSON_ARRAY_SIZE(MAX_WIFI_ELEMS) + JSON_OBJECT_SIZE(1) + MAX_WIFI_ELEMS*JSON_OBJECT_SIZE(2) + 121*MAX_WIFI_ELEMS;
    DynamicJsonBuffer jsonBuffer(capacity);
    JsonObject& root = jsonBuffer.parseObject(confFile);
    confFile.close();
    JsonArray& wifinets = root["wifinets"];
    const size_t arraySize = wifinets.size();
    if (arraySize > 0) {
      WiFi.mode(WIFI_AP_STA);
      WiFi.disconnect();
      this->delay(100);
      WiFi.scanNetworks(true);
      this->delay(1000);
      int8_t scanResult = WiFi.scanComplete();
      for (int tryCount = 0; tryCount < 15; tryCount++) {
        if (scanResult >= 0) break;
        else {
          this->delay(1000); 
          scanResult = WiFi.scanComplete();
        }
      }
      if (scanResult > 0) {
        for (size_t i = 0; i < arraySize; i++) {
          const char *ssid = wifinets[i]["ssid"];
          const char *pass = wifinets[i]["pass"];
          int pos = isAvailable(ssid);
          if (pos >= 0) {
            if (strlen(pass) > 0) {
              WiFi.begin(ssid, pass);
            } else {
              WiFi.begin(ssid);
            }
            const time_t startMonit = TimeKeeper::tkNow();
            while ((WiFi.status() != WL_CONNECTED) && (TimeKeeper::tkNow() - startMonit) < 10) {
              this->delay(500);
            }
            if (WiFi.status() == WL_CONNECTED) break;
          }
        }
      }
      WiFi.scanDelete();
    }
}