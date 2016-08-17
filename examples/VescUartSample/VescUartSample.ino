/*
 Name:		VescUartSample.ino
 Created:	9/26/2015 10:12:38 PM
 Author:	AC
*/

 // >>>

// the setup function runs once when you press reset or power the board
// To use VescUartControl stand alone you need to define a config.h file, that should contain the Serial or you have to comment the line
// #include Config.h out in VescUart.h

//Include libraries copied from VESC
#include "VescUart.h"
#include "datatypes.h"


// SD Card
#include <SPI.h>
#include <SD.h>

File myFile;
// ---

#define DEBUG
unsigned long count;

void setup() {
	//Setup UART port
	Serial1.begin(115200);
  #ifdef DEBUG
  	//SEtup debug port
  	Serial.begin(115200);
	#endif
	Serial.println("init ok!!!");

  // SD Card
  if (!SD.begin(4)) {
    Serial.println("sd init failed!");
    return;
  }
  Serial.println("sd init done.");

  myFile = SD.open("test.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }
  // ---
}

struct bldcMeasure measuredValues;

// the loop function runs over and over again until power down or reset
void loop() {
	//int len=0;
	//len = ReceiveUartMessage(message);
	//if (len > 0)
	//{
	//	len = PackSendPayload(message, len);
	//	len = 0;
	//}

  // Vesc values
	if (VescUartGetValue(measuredValues)) {
		Serial.print("Loop: "); Serial.println(count++);
		SerialPrint(measuredValues);

    if (myFile) {
      myFile.println(measuredValues.avgMotorCurrent);
    }
	} else {
		Serial.println("Failed to get data!");
	}

	delay(500);
}
