#include "IPv6Stack.h"
#include "XBeeMACLayer.h"
#include "CoapLayer.h"
#include "App.h"

#define IS_INTERMEDIATE_ROUTER  (UIP_CONF_ROUTER && 1)
#define IS_BORDER_ROUTER (UIP_CONF_ROUTER && !IS_INTERMEDIATE_ROUTER)
#define UDP_PORT 5683
#define SEND_INTERVAL  (10 * 1000)// 10 seconds

int mem() {
    uint8_t * heapptr, * stackptr;
    stackptr = (uint8_t *) malloc(4);
    heapptr = stackptr;
    free(stackptr);
    stackptr = (uint8_t *) (SP);
    return stackptr - heapptr;
}

static uint8_t scratch_raw[32];
static coap_rw_buffer_t scratch_buf = {scratch_raw, sizeof (scratch_raw)};
XBeeMACLayer macLayer;

#define UDP_MAX_DATA_LEN 200
uint8_t udp_send[UDP_MAX_DATA_LEN];

IPv6Address addr_dest(0xaa, 0xaa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1);
IPv6Timer send_timer;

uint8_t coap_data[UDP_MAX_DATA_LEN];
uint16_t coap_data_length = 0;
IPv6Address sender_address;
uint16_t dest_port = UDP_PORT;

uint16_t len;
uint8_t packetbuf[UDP_MAX_DATA_LEN];

void coap_callback(uint8_t *data, uint16_t datalen, uint16_t sender_port, IPv6Address &sender_addr) {

    Serial.println();
    Serial.print("MEMORY LEFT:");
    Serial.println(mem());

    Serial.println();
    Serial.println();
    Serial.print("Data received from ip: ");
    sender_addr.print();
    Serial.print(" port: ");
    Serial.print(sender_port);
    Serial.println();
    uint16_t rc;
    uint16_t i;
    len = 0;
    coap_packet_t pkt;

    Serial.println();

    Serial.println();
    if (0 != (rc = CoapLayer::parse(&pkt, data, datalen))) {
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

        size_t rsplen = sizeof (packetbuf);
        coap_packet_t rsppkt;
        App::handleRequest(&scratch_buf, &pkt, &rsppkt);

        memset(packetbuf, 0, UDP_MAX_DATA_LEN);
        if (0 != (rc = CoapLayer::build(packetbuf, &rsplen, &rsppkt))) {
            Serial.print("coap_build failed rc=");
            Serial.println(rc, DEC);
        } else {
            IPv6Stack::udpSend(sender_addr, sender_port, packetbuf, rsplen);
        }
    }
}

void setup() {
    Serial.begin(38400);
    delay(1000);
    Serial.println();
    delay(100);
    Serial.println();
    Serial.print("MEMORY LEFT:");
    Serial.println(mem());

    // init network-device
    if (!IPv6Stack::initMacLayer(&macLayer)) {
        Serial.println("CANNOT INITIALIZE XBEE MODULE.. CANNOT CONTINUE");
        while (true) {
        };
    }
    Serial.println("MAC INITIALIZED");

    //init IP Stack
    IPv6Stack::initIpStack();
    Serial.println("IPV6 INITIALIZED");
    delay(100);

    //init UDP
    IPv6Stack::initUdp(UDP_PORT);
    Serial.println("UDP INITIALIZED");
    delay(100);

    send_timer.set(SEND_INTERVAL);
    Serial.println("SEND TIMER SET");
    delay(50);

    Serial.println("SETUP FINISHED!");
    delay(100);
    //    coap_setup();
    //    App::endpoint_setup();
}

void loop() {
    IPv6Stack::pollTimers();

    if (IPv6Stack::receivePacket()) {
        Serial.print("MEMORY LEFT:");
        Serial.println(mem());
        Serial.println("receiver data");
        if (IPv6Stack::udpDataAvailable()) {
            Serial.println("udpDataAvailable");
            coap_data_length = IPv6Stack::getCoapDataLength();
            IPv6Stack::getCoapData(coap_data);
            IPv6Stack::getUdpSenderIpAddress(sender_address);
            coap_callback(coap_data, coap_data_length, IPv6Stack::getUdpSenderPort(), sender_address);
        }
    }
    delay(100);
}
/*---------------------------------------------------------------------------*/
