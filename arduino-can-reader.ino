#include <SPI.h>
#include <mcp2515_can.h>    // https://github.com/Seeed-Studio/Seeed_Arduino_CAN


/*///////////////////////////////////////////////////////
  Opel/Vauxhall configs
    HSCAN - Dual-wire,   High Speed CAN-BUS,   500 kbps
    MSCAN - Dual-wire,   Medium Speed CAN-BUS, 95 kbps
    SWCAN - Single-wire, Low Speed CAN-BUS,    33.3 kbps
*////////////////////////////////////////////////////////


///////////////////
// Configuration //
///////////////////

// CS pin for CAN bus shield.
// The default pin depends on the shield's version:
//  - 1.0: digital pin 10
//  - 1.1 and newer: digital pin 9
const int CS_PIN = 9;

// Serial port data rate
const long SERIAL_SPEED = 115200;

///////////////////

mcp2515_can CAN(CS_PIN);    // Set CS pin
unsigned char len = 0;
byte buffer[8];

void setup() {
    Serial.begin(SERIAL_SPEED);

    while (CAN.begin(CAN_95KBPS, MCP_16MHz) != CAN_OK) {
      delay(100);
    }

    CAN.setMode(MODE_LISTENONLY);  // Don't send the ACK bit, this may disturb or change the behavior of the system we are trying to sniff (silent mode)
}

void loop() {
    if (CAN.checkReceive() == CAN_MSGAVAIL) {
        CAN.readMsgBuf(&len, buffer);

        Serial.print("FRAME:ID=");
        Serial.print(CAN.getCanId());
        Serial.print(":LEN=");
        Serial.print(len);

        char tmp[3];
        for(int i = 0; i<len; i++) {
            Serial.print(":");

            snprintf(tmp, 3, "%02X", buffer[i]);

            Serial.print(tmp);
        }

        Serial.println();
    }
}


