/** NimBLE differences highlighted in comment blocks **/

/*******original********
  #include <BLEDevice.h>
  #include <BLEUtils.h>
  #include <BLEScan.h>
  #include <BLEAdvertisedDevice.h>
  #include "BLEEddystoneURL.h"
  #include "BLEEddystoneTLM.h"
  #include "BLEBeacon.h"
***********************/
//#include <BLEDevice.h>
#include <Arduino.h>

#include <NimBLEDevice.h>
#include <NimBLEAdvertisedDevice.h>
#include "NimBLEBeacon.h"

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

BLEScan *pBLEScan;
int scanTime = 5; //In seconds
int MAX_BEACON_DISTANCE = -40;//TODO::: search optimal number
std::vector<std::string> beacon_list;
const int Pin = 25; // the number of the LED pin

bool checkShakelConnection(){
  Serial.println("inside check");
  
  return digitalRead(Pin); // 1 connectd 0 not connectd
}



class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    /*** Only a reference to the advertised device is passed now
      void onResult(BLEAdvertisedDevice advertisedDevice) { **/
    void onResult(BLEAdvertisedDevice *advertisedDevice)
    {
      std::string temp = " "; // name of the beacon
      if (advertisedDevice->haveName())
      {
        temp = advertisedDevice->getName().c_str();   
       Serial.println(temp.c_str());
      }

      //found beacon
      if(temp == "Holy-IOT" && advertisedDevice->getRSSI() > MAX_BEACON_DISTANCE && !checkShakelConnection()){
        beacon_list.push_back(advertisedDevice->getAddress().toString());
        Serial.println(temp.c_str());
         Serial.print("Device name: ");
        Serial.println(temp.c_str());

        }
    
  }
};

void setup()
{
  Serial.begin(115200);
  pinMode(Pin, INPUT);
  std::vector<String> beacon_list;

  //Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  
  
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); //  

  pBLEScan->setWindow(99); // less or equal setInterval value    
  
  Serial.println("End init");

   
}

void loop()
{
  Serial.println("=== start loop! ===");
  
  // put your main code here, to run repeatedly:
      BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
     // Serial.println("Scan done!");

      for (auto i = 0; i < beacon_list.size(); i++){ // get the MAC adrres of rhe Beacon
        Serial.print("MAC adrres of Beacon: ");
        Serial.println(beacon_list.at(i).c_str());
      }

      pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
      beacon_list.clear();
      //delay(1000);
      Serial.println("    Loop done!");
}
