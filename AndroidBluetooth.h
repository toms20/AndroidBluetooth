/*
Arduino-Android-Bluetoooth, a simple bluetooth socket for communication with Android
Copyright 2013 Brandon Toms

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

-------------
 */


#include <Arduino.h> // Arduino 1.0 or greater is required
#include <stdlib.h>
#include <SoftwareSerial.h>

#ifndef ANDROIDBLUETOOTH_H_
#define ANDROIDBLUETOOTH_H_

// CRLF characters to terminate lines/handshakes in headers.
#define CRLF "\r\n"

#define MESSAGE_LENGTH 254

class AndroidBluetooth : public SoftwareSerial {
public:
    // Constructor.
    AndroidBluetooth (int rx, int tx);
    
    // Callback functions definition.
    typedef void DataCallback(String message);
    typedef void Callback();
    typedef void PinCallback(int type, int pin, int isHigh);
    
    // Start listening for connections.
    void init();
    
    // Main listener for incoming data. Should be called from the loop.
    void listen();
    
    // Callbacks
    void registerDataCallback(DataCallback *callback);
    void registerConnectCallback(Callback *callback);
    void registerDisconnectCallback(Callback *callback);
    void registerPinCallback(PinCallback *callback);
    
	// Are we connected?
	bool isConnected();
	
    // Embeds data in frame and sends to client.
    bool send(String str);
//    bool send(char* data, byte length);
    

private:
    
    enum State {DISCONNECTED, CONNECTED} state;

    // Pointer to the callback function the user should provide
    DataCallback *onData;
    Callback *onConnect;
    Callback *onDisconnect;
    PinCallback *onPin;

    // Disconnect user gracefully.
    void disconnectStream();
    
    String message;
    char recvChar;
    int rxPin, txPin, pin, isHigh, type;
};

#endif