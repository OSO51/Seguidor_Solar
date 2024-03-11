/*
DS3231_test.pde
Eric Ayars
4/11
Test/demo of read routines for a DS3231 RTC.
Turn on the serial monitor after loading this to check if things are
working as they should.
*/

#include <DS3231.h>
#include <Wire.h>

DS3231 myRTC;
bool century = false;
bool h12Flag;
bool pmFlag;

void setup() {
	// Start the I2C interface
	Wire.begin();
 
	// Start the serial interface
	Serial.begin(57600);
}

void loop() {
	// send what's going on to the serial monitor.
	
	// Start with the year
	Serial.print("2");
	if (century) {			// Won't need this for 89 years.
		Serial.print("1");
	} else {
		Serial.print("0");
	}
	Serial.print(myRTC.getYear(), DEC);
	Serial.print(' ');
	
	// then the month
	Serial.print(myRTC.getMonth(century), DEC);
	Serial.print(" ");
  
	// then the date
	Serial.print(myRTC.getDate(), DEC);
	Serial.print(" ");
  
	// and the day of the week
	Serial.print(myRTC.getDoW(), DEC);
	Serial.print(" ");
  
	// Finally the hour, minute, and second
	Serial.print(myRTC.getHour(h12Flag, pmFlag), DEC);
	Serial.print(" ");
	Serial.print(myRTC.getMinute(), DEC);
	Serial.print(" ");
	Serial.print(myRTC.getSecond(), DEC);
 
	// Add AM/PM indicator
	if (h12Flag) {
		if (pmFlag) {
			Serial.print(" PM ");
		} else {
			Serial.print(" AM ");
		}
	} else {
		Serial.print(" 24h ");
	}
 
	// Display the temperature
	Serial.print("T=");
	Serial.print(myRTC.getTemperature(), 2);
  
	// Tell whether the time is (likely to be) valid
	if (myRTC.oscillatorCheck()) {
		Serial.print(" O+");
	} else {
		Serial.print(" O-");
	}

	Serial.println();
	delay(1000);
}