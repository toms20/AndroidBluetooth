
//Import the required libraries
#include <SoftwareSerial.h>
#include <AndroidBluetooth.h>

//Define our RX and TX pins for serial communication (6 and 7 are arduino uno safe)
#define rxPin 6
#define txPin 7

//Create our bluetooth object! ('androidBT' can be any name you want)
AndroidBluetooth androidBT(rxPin, txPin);

void setup() {
  Serial.begin(38400);
  //Register our callbacks (all are optional)
  androidBT.registerConnectCallback(&onConnect);  //Get notified when a bluetooth connection is established
  androidBT.registerDisconnectCallback(&onDisconnect);  //Get notified when a bluetooth connection gets terminated
  androidBT.registerDataCallback(&onData);  //Get any incomming data from the connected bluetooth device
  androidBT.registerPinCallback(&onPin);  //Get pin state request from the connected bluetooth device
  
  //Call this 'init()' function to set up the SeeedStudios bluetooth shield
  androidBT.init();
  
  //Lets set the arduino onboard led to output so we can see it change via our Android App
  pinMode(13, OUTPUT);
}

void loop() {
  //Listen needs to be called continuously in order to recieve incoming data from the bluetooth device
  androidBT.listen();
  
  //This is where we can perform whatever else we want to do
  //An example would be to listen to the serial port for data and send it back to our Android App!
  if(Serial.available()) {
    androidBT.write(Serial.read());
  }
}



//Android Bluetooth Callback Functions
//These are the functions we registered earlier in our setup()
void onConnect() {
  Serial.println("Bluetooth Connected");
}

void onDisconnect() {
  Serial.println("Bluetooth Disconnected");
}

void onData(String msg) {
  Serial.print("Msg: ");Serial.println(msg);
}


// type = 0 (Digital) || type = 1 (Analog)
// pin = 0, 1, ... 12, 13 
// state = 0 (LOW) || state = 1 (HIGH)
void onPin(int type, int pin, int state) {
  Serial.print("onPin Called:");Serial.print(type);Serial.print(":");Serial.print(pin);Serial.print(":");Serial.println(state);
  
  //Check to see if the pin request is for our digital pin 13
  if(type == 0 && pin == 13) {
    //It is! so lets change the state to the received value...
    digitalWrite(pin, state);
  }
}
