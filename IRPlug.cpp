#include "IRPlug.h"

const uint8_t RECV_PIN = 7;
const uint8_t RBEEP_PIN = 9;
const uint8_t RELAY_PIN = 10;
const uint8_t LED_PIN = 2;
const uint16_t IR_ON_CODE = 0x6F58;
const uint16_t BLINK_FREQ_MS = 5000;

const uint16_t AUTO_OFF_BEEP_MS = 10800000; // 3h
const uint16_t AUTO_OFF_DELAY_MS = 14400000; // 4h
const uint16_t BEEP_REPEAT_MS = 1000;

boolean realyOn = false;
IRrecv irrecv(RECV_PIN);
decode_results irDecode;
uint32_t lastBlinkMs = 0;
uint32_t lastIRSigMs = 0;
uint32_t lastBeepMs = 0;
boolean blinkOn = false;

void setup() {
	//Serial.begin(115200);
	irrecv.enableIRIn();
	irrecv.blink13(true);
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);

	pinMode(LED_PIN, OUTPUT);

}

void blink() {
	if (blinkOn) {
		digitalWrite(LED_PIN, LOW);
		blinkOn = false;
	} else {
		digitalWrite(LED_PIN, HIGH);
		blinkOn = true;

	}
}
void switchRealyOn() {
	realyOn = true;
	//Serial.println("ON");
	digitalWrite(RELAY_PIN, HIGH);
}

void switchRealyOff() {
	//Serial.println("OFF");
	digitalWrite(RELAY_PIN, LOW);
	realyOn = false;
}

void loop() {
	long ms = millis();

	if (irrecv.decode(&irDecode)) {
		lastIRSigMs = ms;
		blink();
		if (IR_ON_CODE == irDecode.value) {
			if (realyOn) {
				switchRealyOff();
			} else {
				switchRealyOn();
			}
			delay(200);
		}
		irrecv.resume();
	}

	if (ms - lastBlinkMs > BLINK_FREQ_MS) {
		lastBlinkMs = ms;
		blink();
	}

	if (realyOn && ms - lastIRSigMs > AUTO_OFF_BEEP_MS) {
		if (ms - lastBeepMs > BEEP_REPEAT_MS) {
			tone(RBEEP_PIN, 1000, 200);
			lastBeepMs = ms;
		}
		if (ms - lastIRSigMs > AUTO_OFF_DELAY_MS) {
			switchRealyOff();
		}
	}
}
