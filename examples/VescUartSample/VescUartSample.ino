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

File logFile;
// ---

// #define DEBUG

unsigned long count;

void setup() {
  pinMode(13, OUTPUT);


	//Setup UART port
	Serial1.begin(115200);
  Serial.begin(115200);
  #ifdef DEBUG
  	//SEtup debug port
	  Serial.println("init ok!!!");
  #endif

  // SD Card
  if (!SD.begin(4)) {
    #ifdef DEBUG
      Serial.println("sd init failed!");
    #endif
    return;
  }
  #ifdef DEBUG
    Serial.println("sd init done.");
  #endif
  // ---

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
}

struct bldcMeasure values;

// the loop function runs over and over again until power down or reset
void loop() {
   digitalWrite(13, LOW);
	//int len=0;
	//len = ReceiveUartMessage(message);
	//if (len > 0)
	//{
	//	len = PackSendPayload(message, len);
	//	len = 0;
	//}

  // Vesc values
	if (VescUartGetValue(values)) {
    #ifdef DEBUG
		  Serial.print("Loop: "); Serial.println(count++);
    #endif

		SerialPrint(values);

    logFile = SD.open("vesc_log.txt", FILE_WRITE);
    if (logFile) {
      #ifdef DEBUG
      Serial.print("writing to sd");
      Serial.println("rpm");
      Serial.println(values.rpm);
      Serial.println("avgMotorCurrent");
      Serial.println(values.avgMotorCurrent);
      #endif

      logFile.print("{");
      logFile.print("\"t\":");
      logFile.print(millis());
      logFile.print(",\"avgMotorCurrent\":");
      logFile.print(values.avgMotorCurrent);
      logFile.print(",\"avgInputCurrent\":");
      logFile.print(values.avgInputCurrent);
      logFile.print(",\"dutyCycleNow\":");
      logFile.print(values.dutyCycleNow);
      logFile.print(",\"rpm\":");
      logFile.print(values.rpm);
      logFile.print(",\"inpVoltage\":");
      logFile.print(values.inpVoltage);
      logFile.print(",\"ampHours\":");
      logFile.print(values.ampHours);
      logFile.print(",\"ampHoursCharged\":");
      logFile.print(values.ampHoursCharged);
      logFile.print(",\"tachometer\":");
      logFile.print(values.tachometer);
      logFile.print(",\"tachometerAbs\":");
      logFile.print(values.tachometerAbs);
      logFile.println("}");

      logFile.close();
      #ifdef DEBUG
        Serial.println("done.");
      #endif
    } else {
      #ifdef DEBUG
        Serial.println("error opening vesc_log.txt");
      #endif
      digitalWrite(13, HIGH);
      delay(200);
      digitalWrite(13, LOW);
    }
	} else {
    #ifdef DEBUG
		  Serial.println("Failed to get data!");
    #endif

    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
	}

	delay(20);
}
