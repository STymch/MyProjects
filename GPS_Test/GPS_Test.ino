/*
 Name:		GPS_Test.ino
 Created:	2/18/2018 11:18:50 PM
 Author:	stymc
*/


#include <SoftwareSerial.h>
#include <TinyGPS.h>

/* This sample code demonstrates the normal use of a TinyGPS object.
It requires the use of SoftwareSerial, and assumes that you have a
4800-baud serial GPS device hooked up on pins 12(rx) and 13(tx).
*/

TinyGPS gps;
SoftwareSerial ss(12, 13);
uint8_t	state = LOW;
uint8_t	LED = 13;

void setup()
{
	Serial.begin(38400);
	ss.begin(9600);

	Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
	Serial.println("by Mikal Hart");
	Serial.println();
}

void loop()
{
	bool newData = false;
	unsigned long chars;
	unsigned short sentences, failed;

	// For one second we parse GPS data and report some key values
	for (unsigned long start = millis(); millis() - start < 1000;)
	{
		while (ss.available())
		{
			char c = ss.read();
//			Serial.print("Read GPS="); Serial.println(c);

			// Serial.write(c); // uncomment this line if you want to see the GPS data flowing
			if (gps.encode(c)) // Did a new valid sentence come in?
				newData = true;
		}
	}

	if (newData)
	{
		float flat, flon;
		unsigned long age;
		gps.f_get_position(&flat, &flon, &age);
		Serial.print("LAT=");	// Latitude, Широта
		Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
		Serial.print(" LON=");	// Longitude, Долгота
		Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
		Serial.print(" SAT=");
		Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
		Serial.print(" PREC=");
		Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());

		digitalWrite(LED, state);
		state = !state;

	}

	gps.stats(&chars, &sentences, &failed);
	Serial.print(" CHARS=");
	Serial.print(chars);
	Serial.print(" SENTENCES=");
	Serial.print(sentences);
	Serial.print(" CSUM ERR=");
	Serial.println(failed);
	if (chars == 0)
		Serial.println("** No characters received from GPS: check wiring **");
}
