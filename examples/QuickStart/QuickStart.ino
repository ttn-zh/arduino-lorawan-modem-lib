#include <TheThingsNetwork.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, /* TTN_FP_EU868 or TTN_FP_US915 */);

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  // Set callback for incoming messages
  ttn.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);
}

void loop() {
  debugSerial.println("-- LOOP");

  // Prepare payload of 1 byte to indicate LED status
  byte payload[1];
  payload[0] = (digitalRead(LED_BUILTIN) == HIGH) ? 1 : 0;

  // Send it off
  ttn.sendBytes(payload, sizeof(payload));

  delay(10000);
}

void message(const byte* payload, size_t length, uint8_t port) {
  debugSerial.println("-- MESSAGE");

  // Only handle messages of a single byte
  if (length != 1) {
    return;
  }

  if (payload[0] == 0) {
    debugSerial.println("LED: off");
    digitalWrite(LED_BUILTIN, LOW);

  } else if (payload[0] == 1) {
    debugSerial.println("LED: on");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}