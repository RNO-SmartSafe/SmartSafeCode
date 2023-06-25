//**** date version : 7/6 **** 
////////////////////////////////////////////////////1
////// Authors: Natan Gorshomov, Ofir Magen ////////12311
////////////////////////////////////////////////////

#include <chrono>
#include <ctime>    
#include "painlessMesh.h"
#include <NimBLEDevice.h>

//for mesh init
#define   MESH_PREFIX     "SmartSafe"
#define   MESH_PASSWORD   "RNO123456"
#define   MESH_PORT       5555

#define ENDIAN_CHANGE_U16(x) ((((x)&0xFF00) >> 8) + (((x)&0xFF) << 8))

#define   scanTime               5  // In seconds
#define   MAX_BEACON_DISTANCE   -50
#define   Pin                    25 // the number of the chakel pin

Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;
BLEScan *pBLEScan;
std::string IOT_Beacon_Name;

auto start = std::chrono::system_clock::now();
int nodeMassageNumber = 0;
std::string message_type = "Log";

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

bool checkShakelConnection(){
  Serial.println("inside check");
  return digitalRead(Pin); // 1 connectd 0 disconnectd
}

// BLE class
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
      }
      //if found beacon && the distance is under 60 db && the chakel isent connected!
      if(temp == "Holy-IOT" && advertisedDevice->getRSSI() > MAX_BEACON_DISTANCE && !checkShakelConnection()){
        Serial.println(advertisedDevice->getRSSI());
        Serial.println("In dangerous zone!!! Mac address: ");
        message_type = "dangerous zone";
        IOT_Beacon_Name = advertisedDevice->getAddress().toString().c_str();
       
      }
  }
};

//********** mesh functions *************
void sendMessage() {
  nodeMassageNumber++;
  std::string m = std::to_string(nodeMassageNumber);
  std::string msg = "{ \"message type\":\"" + message_type + "\", " + "\"IOT\":\"" + IOT_Beacon_Name + "\", " + "\"messege number\":\"" + m + "\", " + "\"from\":";
  mesh.sendBroadcast( msg.c_str() );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
  IOT_Beacon_Name="";
  message_type = "Log";
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("%s\"%u\"}\n" , msg.c_str(), from);
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);
  pinMode(Pin, INPUT);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages
  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100); //  
  pBLEScan->setWindow(99); // less or equal setInterval value  
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  
  if(elapsed_seconds.count() >= 20.0){ 
    Serial.print("----- start beacon scan  ----- \n");
    BLEScanResults foundDevices = pBLEScan->start(scanTime , false);
    pBLEScan->clearResults(); // delete results fromBLEScan buffer to release memory
    Serial.print("----- End beacon scan  ------ \n");
    start = std::chrono::system_clock::now();
  }
}
