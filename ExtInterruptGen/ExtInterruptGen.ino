/*
 Name:		ExtInterruptGen.ino
 Created:	1/23/2018 4:46:46 PM
 Author:	sitymchenko
*/

#include "TimerOne.h"

const long  DR_HARDWARE_COM = 38400;	// Data rate for hardware COM, bps
const int	PWM_PIN			= 9;		// Use PWM on this pin
const int	PWM_DUTY		= 256;		// PWM duty cycle, 25%

long	lTimerPeriod	= 1400;			// Timer1 period, microsec.
bool	state = false;					// State for calculation time interval 
float	fTick_ms = lTimerPeriod/1000.0f;// Millisecs in timer tick

volatile long	lCounter = 0;			// Timer's tick counter
long			lCountStart, lCountStop;// For save timer's ticks  
	Serial.begin(DR_HARDWARE_COM);				Serial.println();
	Serial.print(F("\nPWM on PIN =\t\t"));		Serial.println(PWM_PIN);
	Serial.print(F("Timer1 period, mcs =\t"));	Serial.println(lTimerPeriod);
	Serial.print(F("Q, m3/h =\t\t"));			Serial.println(3600.0f / lTimerPeriod,3);
	Serial.print(F("Pulse, ms =\t\t"));			Serial.println((PWM_DUTY / 1024.0) * lTimerPeriod / 1000, 2);
}

void callback(){ ++lCounter;}


	// Ожидание команды из последовательного порта (байт)
	if ((nInpByte = Serial.read()) > 0) 
	{
		Serial.print(" PRESSED: ");	Serial.print(nInpByte);
		
		// анализируем принятый байт
		switch(nInpByte)
		{
			case 32:	// SPACE - start/stop calculate elapsed time
				if (!state) // state false - starting counting ticks
				{ 
					noInterrupts();
					lCountStart = lCounter;	// save start time
					interrupts();
				}
				else		// state true - stop counting
				{		
					noInterrupts();
					lCountStop = lCounter;	// save current time
					interrupts();
					
					Serial.print("\tElapsed time, ms = ");	
					Serial.println((unsigned long)((lCountStop-lCountStart)*fTick_ms));
				}
				state = !state;				// change state
			noInterrupts();
			Serial.print(F("\nPWM on PIN =\t\t"));		Serial.println(PWM_PIN);
			Serial.print(F("Timer1 period, mcs =\t"));	Serial.println(lTimerPeriod);
			Serial.print(F("Q, m3/h =\t\t"));			Serial.println(3600.0f / lTimerPeriod, 3);
			Serial.print(F("Pulse, ms =\t\t"));			Serial.println((PWM_DUTY / 1024.0) * lTimerPeriod / 1000, 2);