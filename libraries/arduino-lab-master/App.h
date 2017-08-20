/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   App.h
 * Author: Hoang Hoi
 *
 * Created on Ngày 06 tháng 4 năm 2017, 13:42
 */

#ifndef APP_H
#define APP_H
#include <stdbool.h>
#include <string.h>
#include "CoapLayer.h"

#define RESPONSE_LENGTH 150
#define QUERY_LENGTH 15
#define DHT21_PIN 5

class App {
public:
    static int handleRequest(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt);
    static int handle_get_data(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt);
    static void endpoint_setup(void);
private:
    static char light;
    static int led;
    static char rsp[RESPONSE_LENGTH];
    static uint8_t compareBuffer(const coap_buffer_t &buffer, uint8_t * cpBuff, uint8_t cpBuffLength);
    static double getTemperature(char * response, uint8_t length);
    static uint8_t comparePaths(const coap_option_t * paths, const coap_endpoint_path_t * c_paths);
};

#endif /* APP_H */

