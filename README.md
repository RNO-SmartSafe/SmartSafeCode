<img align="right" width="100" height="100" src="[(https://github.com/RNO-SmartSafe/SmartSafeApp/blob/master/Image/logo.jpg?raw=true](https://github.com/RNO-SmartSafe/SmartSafeApp/blob/master/Image/logo.jpg?raw=true)">

# SmartSafe

SmartSafe is an end-to-end solution that
prevents workers from falling at height in
construction sites. According to data analysis from the Institute
for Safety and Security in the construction
industry, these days 47% of all accidents
were caused by workers falling from heights.

The system will identify a worker who is in a “danger zone" when he is not harnessed, and send a message to the safety inspector’s phone via SmartSafe
application.

## Authors

- [@NatanG.](https://github.com/Natan-G)
- [@OfirMagen](https://github.com/ofir-magen)
- [@ReutHadad](https://github.com/Reut212)

## how does it work?

Our system is based on ESP32 microcontrollers communication using WIFI and Bluetooth protocols. An esp32 (microcontroller) will be installed on each worker's harness, where the esp32 will receive
Bluetooth signals from a beacon device placed in a danger zone and when he is not harnessed transmit the data through [Wireless Mesh Network](https://en.wikipedia.org/wiki/Wireless_mesh_network) that the esp32 will create and store it on Firebase db, that connect between the physical system to App.

**Benefits:**
 - Mesh Network which covers a large area without infrastructure, with Self-healing
 - A small, dynamic, and light device
 - [App](https://github.com/RNO-SmartSafe/SmartSafeApp) with user-friendly interface
 - Low-cost and low-power system


## Tech Stack

- [ESP32](http://esp32.net)
- [beacon](https://en.wikipedia.org/wiki/Bluetooth_Low_Energy_beacon)

**programming languages:** c++ for esp32, flutter for App , python for Gateway

**frameworks:** VSCode and PlatformIO extensions for esp32* 

**databases:** Firebase
## architecture
System architecture

![](https://github.com/RNO-SmartSafe/SmartSafeApp/blob/master/Image/design%20architecture.jpg?raw=true)


Mesh architecture
![](https://github.com/RNO-SmartSafe/SmartSafeApp/blob/master/Image/mesh%20architecture.jpg?raw=true)
