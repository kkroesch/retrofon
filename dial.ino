/* *************************************************
 *
 * DIAL.CPP - Class for recognize dial mechanic.
 *
 *
 * Author: Karsten Kroesch <karsten@kroesch.ch>
 *
 * *************************************************/

#include "Arduino.h"
#include "Switch.h"
#include "TM1638.h"

const byte DIAL_PULSE_PIN = 2;
const byte DIAL_READY_PIN = 3;
const byte PHONE_HOOK_PIN = 4;

const byte LED_STROBE_PIN = 6;
const byte LED_DATA_PIN = 7;
const byte LED_CLOCK_PIN = 8;



// Button to GND, use internal 20K pullup resistor
Switch dialReadyContact = Switch(DIAL_READY_PIN);
Switch dialPulseContact = Switch(DIAL_PULSE_PIN);
Switch phoneHookContact = Switch(PHONE_HOOK_PIN);

TM1638 module(LED_CLOCK_PIN, LED_DATA_PIN, LED_STROBE_PIN);

int count = 1;
int phoneNumber[] = {};

int digit = 0;

void setup() {
	Serial.begin(9600);
    module.clearDisplay();
    module.setupDisplay(true, 2);
}

void loop() {
	phoneHookContact.poll();
	// Hook switch is open when down
	if (phoneHookContact.released()) {
		Serial.println("AT H0");
	}
	
	dialReadyContact.poll();
	
	if (dialReadyContact.on()){
		dialPulseContact.poll();
		if (dialPulseContact.pushed()) {
			count ++;
		}
	}
	
	if (dialReadyContact.released()){
		// Number zero gives 10 pulses, so set back to zero:
        count = count % 10;
        Serial.println(count);
        module.setDisplayDigit(count, digit, false);
        digit ++;
		count = 0;
	}
}
