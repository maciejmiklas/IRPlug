#include "IRPlug.h"

const int RECV_PIN = 7;
const int OUT_PIN = 8;

const int IR_SIG = 0x6F58;

boolean on = false;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
	//Serial.begin(115200);
	irrecv.enableIRIn();
	irrecv.blink13(true);
	pinMode(OUT_PIN, OUTPUT);
}

long lst = 0;

void loop() {
	if (irrecv.decode(&results)) {
		if (IR_SIG == results.value) {
			long ct = millis();
			if (ct - lst < 500){
				return;
			}
			lst = ct;
			if (on) {
				//Serial.println("OFF");
				digitalWrite(OUT_PIN, LOW);
				on = false;
			} else {
				on = true;
				//Serial.println("ON");
				digitalWrite(OUT_PIN, HIGH);
			}
		}
		irrecv.resume();
	}
}
