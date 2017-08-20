#include <stdint.h>
#include "coap_packet.h"

#define PORT 5683
static uint8_t mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

//uint8_t packetbuf[] = {0x40, 0x01, 0x9c, 0xa1, 0xb1, 0x7e, 0x06, 0x69, 0x6e, 0x2d, 0x63, 0x73, 0x65};
//uint8_t packetbuf[] = {0x40, 0x02, 0x9c, 0xa2, 0xb1, 0x7e, 0x06, 0x69, 0x6e, 0x2d, 0x63, 0x73, 0x65, 0xff, 0x74, 0x65, 0x6d, 0x3d, 0x37, 0x26, 0x74, 0x69, 0x6d, 0x65, 0x3d, 0x32, 0x30, 0x31, 0x37, 0x2d, 0x30, 0x33};
//uint8_t packetbuf[] = {0x46, 0x02, 0x54, 0xed, 0x61, 0x62, 0x63, 0x78, 0x79, 0x7a, 0xb6, 0x73, 0x65, 0x6e, 0x73, 0x6f, 0x72, 0x11, 0x32, 0x51, 0x32, 0xff, 0x7b, 0x22, 0x61, 0x22, 0x3a, 0x22, 0x62, 0x22, 0x7d};
//uint8_t packetbuf[] = {0x42, 0x01, 0xc0, 0x5c, 0x5b, 0xa0, 0xb8, 0x73, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x65, 0x11, 0x32, 0x33, 0x61, 0x3d, 0x62, 0x21, 0x32, 0x61, 0x02};
uint8_t packetbuf[] = {0x42, 0x01, 0xec, 0x46, 0x5b, 0xa0, 0xb8, 0x73, 0x65, 0x70, 0x61, 0x72, 0x61, 0x74, 0x65, 0x03, 0x78, 0x79, 0x7a, 0x11, 0x32, 0x33, 0x61, 0x3d, 0x62, 0x21, 0x32, 0x61, 0x02};
static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof (scratch_raw)};

void setup() {
    int i;
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect.
    }

    Serial.print("My IP address: ");
    for (i = 0; i < 4; i++) {
        Serial.print(".");
    }
    Serial.println();

    coap_setup();
    endpoint_setup();
}

void udp_send(const uint8_t *buf, int buflen) {
    int i;
    for (i = 0; i < buflen; i++) {
        Serial.print(buf[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
    int sz = sizeof (packetbuf);
    int rc;
    coap_packet_t pkt;
    int i;

    Serial.println("Packet:");
    for (i = 0; i < sz; i++) {
        Serial.print(packetbuf[i], HEX);
        Serial.print(" ");
    }
    Serial.println("");

    if (0 != (rc = coap_parse(&pkt, packetbuf, sz))) {
        Serial.print("Bad packet rc=");
        Serial.println(rc, DEC);
    } else {
        Serial.println("coap parse sucess");

        Serial.print("Version: ");
        Serial.println(pkt.hdr.ver, HEX);

        Serial.print("Type: ");
        Serial.println(pkt.hdr.t, HEX);

        Serial.print("Tkl: ");
        Serial.println(pkt.hdr.tkl, HEX);

        Serial.print("Code: ");
        Serial.println(pkt.hdr.code, HEX);

        Serial.print("Transaction ID: ");
        Serial.print(pkt.hdr.id[0], HEX);
        Serial.println(pkt.hdr.id[1], HEX);

        Serial.println("Token:");
        for (i = 0; i < pkt.tok.len; i++) {
            if (pkt.tok.p[i] < 10) {
                Serial.print("0");
            }
            Serial.print(pkt.tok.p[i], HEX);
            Serial.print(" ");
        }
        Serial.println();

        for (i = 0; i < pkt.numopts; i++) {
            Serial.print("Opt: ");
            Serial.println(i);
            Serial.print("Num: ");
            Serial.println(pkt.opts[i].num);
            Serial.print("Len: ");
            Serial.println(pkt.opts[i].buf.len);
            byte k;
            for (k = 0; k < pkt.opts[i].buf.len; k++) {
                if (pkt.opts[i].buf.p[k] < 10) {
                    Serial.print("0");
                }
                Serial.print(pkt.opts[i].buf.p[k], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }

        Serial.println("\n----------------------------");
        Serial.println("Payload");
        for (i = 0; i < pkt.payload.len; i++) {
            if (pkt.payload.p[i] < 10) {
                Serial.print("0");
            }
            Serial.print(pkt.payload.p[i], HEX);
            Serial.print(" ");
        }
        Serial.println("---------------------");
        Serial.println();
    }
    delay(1000);
    
//    coap_rw_buffer_t rwBuffer;
    coap_packet_t cpt;
//    String uri = "abc/xyz";
    uint16_t msgId = 123;
//    uint8_t payload[];
//    coap_make(COAP_METHOD_GET, &cpt, uri, 0, 0, msgId, 0);
//    coap_make_response();
}

