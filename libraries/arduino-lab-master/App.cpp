/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   App.cpp
 * Author: Hoang Hoi
 * 
 * Created on Ngày 06 tháng 4 năm 2017, 13:42
 */

#include <stddef.h>
#include "App.h"
#include "coap_enum.h"
#include "coap_conf.h"
#include <dht.h>

char App::light = '0';
int App::led = 13;
char App::rsp[RESPONSE_LENGTH] = "";

const coap_endpoint_path_t get_data_path = {
    1,
    {"get-data"}
};

const coap_endpoint_t endpoints[] = {
    {COAP_METHOD_GET, App::handle_get_data, &get_data_path},
};
const uint8_t endpointsLength = 1;

int App::handle_get_data(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt) {
    uint8_t i;
    uint8_t count;
    uint16_t action;
    char query_buff[QUERY_LENGTH];
    const coap_option_t * queries = CoapLayer::findOptions(inpkt, COAP_OPTION_URI_QUERY, &count);
    for (i = 0; i < count; i++) {
        uint8_t j;
        CoapLayer::bufferToString(query_buff, QUERY_LENGTH, &queries[i].buf);
        String query_str(query_buff);
        j = query_str.indexOf('=');
        if (j != -1, query_str.substring(0, j) == "type") {
            Serial.println(query_str.substring(0, j));
            if (query_str.substring(j + 1) == "temp") {
                Serial.println(query_str.substring(j + 1));
                App::getTemperature(App::rsp, RESPONSE_LENGTH);
                Serial.println(App::rsp);
                return CoapLayer::makeResponse(
                        scratch,
                        outpkt,
                        (const uint8_t *) App::rsp,
                        strlen(App::rsp),
                        inpkt->hdr.id[0],
                        inpkt->hdr.id[1],
                        &inpkt->tok,
                        COAP_RSPCODE_CONTENT,
                        COAP_CONTENTTYPE_APPLICATION_JSON);
            }
        }
    }

    Serial.println("Not Found: ");
    CoapLayer::makeResponse(
            scratch,
            outpkt,
            NULL,
            0,
            inpkt->hdr.id[0],
            inpkt->hdr.id[1],
            &inpkt->tok,
            COAP_RSPCODE_NOT_FOUND,
            COAP_CONTENTTYPE_TEXT_PLAIN);
}

void App::endpoint_setup(void) {
    pinMode(led, OUTPUT);
}

uint8_t App::compareBuffer(const coap_buffer_t &buffer, uint8_t * cpBuff, uint8_t cpBuffLength) {
    uint8_t i;
    if (buffer.len != cpBuffLength) {
        return 0;
    }

    for (i = 0; i < cpBuffLength; i++) {
        if (buffer.p[i] != cpBuff[i]) {
            return 0;
        }
    }

    return 1;
}

uint8_t App::comparePaths(const coap_option_t * paths, const coap_endpoint_path_t * c_paths) {
    uint8_t i;
    for (i = 0; i < c_paths->count; i++) {
        if (!App::compareBuffer(paths[i].buf, (uint8_t *) c_paths->elems[i], strlen(c_paths->elems[i]))) {
            return 0;
        }
    }

    return 1;
}

int App::handleRequest(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt) {
    const coap_option_t *opt;
    uint8_t count;
    uint8_t i;
    uint8_t j;
    uint8_t k;
    Serial.print("Endpoint: ");
    Serial.println(endpoints[0].method);
    const coap_option_t * paths = CoapLayer::findOptions(inpkt, COAP_OPTION_URI_PATH, &count);

    for (k = 0; k < endpointsLength; k++) {
        Serial.print("Endpoint: ");
        Serial.println(endpoints[k].method);
        for (j = 0; j < endpoints[k].path->count; j++) {
            Serial.println(endpoints[k].path->elems[j]);
        }
        Serial.print("--------------------");
        if (endpoints[k].method != inpkt->hdr.code) {
            continue;
        }

        if (count != endpoints[k].path->count) {
            continue;
        }

        if (App::comparePaths(paths, endpoints[k].path)) {
            endpoints[k].handler(scratch, inpkt, outpkt);
        }

        return 1;
    }

    Serial.println("Not Found: ");
    CoapLayer::makeResponse(
            scratch,
            outpkt,
            NULL,
            0,
            inpkt->hdr.id[0],
            inpkt->hdr.id[1],
            &inpkt->tok,
            COAP_RSPCODE_NOT_FOUND,
            COAP_CONTENTTYPE_TEXT_PLAIN);

    return 0;
}

double App::getTemperature(char * response, uint8_t length) {
    dht DHT;
    int chk = DHT.read21(DHT21_PIN);
    double temperature = DHT.temperature;
    String temp(temperature);
    String S_response = "{'temp': 'value', 'unit': 'c'}";
    String value = "value";
    S_response.replace(value, temp);
    S_response.getBytes((uint8_t *) response, length);

    return temperature;
}
