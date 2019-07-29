#include "IRPlug.h"

const int RECV_PIN = 7;
const int OUT_PIN = 8;
const int LED_PIN = 2;

const int IR_SIG = 0x6F58;

boolean on = false;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
	//Serial.begin(115200);
	irrecv.enableIRIn();
	irrecv.blink13(true);
	pinMode(OUT_PIN, OUTPUT);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(OUT_PIN, HIGH);
}

long lst = 0;
boolean lon = false;
void loop() {
	if (irrecv.decode(&results)) {
		if (IR_SIG == results.value) {
			if (on) {
				//Serial.println("OFF");
				digitalWrite(OUT_PIN, LOW);
				on = false;
			} else {
				on = true;
				//Serial.println("ON");
				digitalWrite(OUT_PIN, HIGH);
			}
			delay(500);
		}
		irrecv.resume();
	}

	long ct = millis();
	if (ct - lst > 2000) {
		lst = ct;
		if(lon){
			digitalWrite(LED_PIN, LOW);
			lon = false;
		}else{
			digitalWrite(LED_PIN, HIGH);
			lon = true;

		}
	}
}
