#include <SocketIOClient.h>

const char* ssid = "Tang-4";
const char* password = "!@#$%^&*o9";

// Server Ip
String host = "192.168.2.110";
// Server port
int port = 3000;

SocketIOClient socket;

void setupNetwork() {
    WiFi.begin(ssid, password);
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 20) delay(500);
    if (i == 21) {
        Serial.println("Can not connect wifi!");
        while (1) delay(500);
    }

    Serial.println("Wifi connected!");
}

void changeLedState(String data) {
    Serial.println(data);
    if (data == "[\"led-change\",\"off\"]") {
        digitalWrite(LED_BUILTIN, HIGH);
        Serial.println("Led off!");
    } else {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.println("Led on!");
    }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.begin(115200);
    setupNetwork();
    socket.on("led-change", changeLedState);
    socket.connect(host, port);
}

void loop() {
    socket.monitor();
}










