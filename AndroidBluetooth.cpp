#include "AndroidBluetooth.h"

#define DEBUG

AndroidBluetooth::AndroidBluetooth(int rx, int tx) : SoftwareSerial(rx, tx) {
    state = DISCONNECTED;
    onConnect = NULL;
    onData = NULL;
    onDisconnect = NULL;
    onPin = NULL;
    pinMode(tx, OUTPUT);
    pinMode(rx, INPUT);
    Serial.println("Android Bluetooth Initiated");
    rxPin = rx;
    txPin = tx;
}


void AndroidBluetooth::init() {
    begin(38400);                           //Set BluetoothBee BaudRate to default baud rate 38400
    print("\r\n+STWMOD=0\r\n");             //set the bluetooth work in slave mode
    print("\r\n+STNA=SeeedBTSlave\r\n");    //set the bluetooth name as "SeeedBTSlave"
    print("\r\n+STOAUT=1\r\n");             // Permit Paired device to connect to me
    print("\r\n+STAUTO=0\r\n");             // Auto-connection should be forbidden here
    delay(2000);                            // This delay is required.
    print("\r\n+INQ=1\r\n");                //make the slave bluetooth inquirable
    Serial.println("The slave bluetooth is inquirable!");
    delay(2000);                            // This delay is required.
    flush();
}


void AndroidBluetooth::listen() {
    if(available()){//check if there's any data sent from the remote bluetooth shield
        recvChar = read();
        //      Serial.print(recvChar);
        if(message.length() < MESSAGE_LENGTH) {
            message.concat(recvChar);
        }
    } else if(message.length() > 0){
            if(message.indexOf("+BTSTATE:1") >= 0) {
                state = DISCONNECTED;
                if(onDisconnect) onDisconnect();
            } else if(message.indexOf("+BTSTATE:4") >= 0) {
                state = CONNECTED;
                if(onConnect) onConnect();
                String temp = "blpins:D";
                temp.concat(rxPin);
                temp.concat("&D");
                temp.concat(txPin);
                send(temp);
                temp = "";
            } else if(message.indexOf("+BTSTATE:3") >= 0) {
            
            } else if(message.indexOf("CONNECT:OK") >= 0) {
                
            } else if(message.indexOf("ERROR") >= 0) {
                
            } else if(message.indexOf("pRQST") >= 0) {
                //Get Type Analog vs Digital
                (message.charAt(5) == 'A') ? type = 1 : type = 0;
                //Get state HIGH vs LOW
                if(message.indexOf("false") >= 0) isHigh = 0;
                else isHigh = 1;
                //Get Pin Number
                char *buffer = {"h"};
                message.substring(6,8).toCharArray(buffer, 3);
                pin = atoi(buffer);
                //Send data to callback
                if(onPin) onPin(type, pin, isHigh);
                if(type == 0) {
                    Serial.print("Requested:");
                    Serial.print("Digital Pin ");
                    Serial.print(pin);
                    Serial.print(" to ");
                    if(isHigh) Serial.println("HIGH");
                    else Serial.println("LOW");
                }
                else {
                    send((String)analogRead(pin));
                    Serial.print("Analog Pin ");
                    Serial.print(pin);
                    Serial.print(" Read ");
                    Serial.println(analogRead(pin));
                }
                buffer = NULL;
            }
            else if (onData) onData(message);
            message = "";
    }
}


bool AndroidBluetooth::isConnected() {
	return (state == CONNECTED) ? true : false;
}


void AndroidBluetooth::disconnectStream() {
   //TODO Gracefully Disconnect Here
}

void AndroidBluetooth::registerConnectCallback(Callback *callback) {
    onConnect = callback;
}
void AndroidBluetooth::registerDataCallback(DataCallback *callback) {
    onData = callback;
}
void AndroidBluetooth::registerDisconnectCallback(Callback *callback) {
    onDisconnect = callback;
}
void AndroidBluetooth::registerPinCallback(PinCallback *callback) {
    onPin = callback;
}


bool AndroidBluetooth::send(String test) {
	if (state == CONNECTED) {
        char *buffer = {"hello"};
        test.toCharArray(buffer, test.length()+1);
//        write('~');
//        write('#');
        for (int i = 0; i < test.length(); i++) {
            print(buffer[i]);
        }
//        write('#');
        write('~');
		delay(1);
        return true;
    }
#ifdef DEBUG
    Serial.println("Not connected, no data sent.");
#endif
    return false;
}
