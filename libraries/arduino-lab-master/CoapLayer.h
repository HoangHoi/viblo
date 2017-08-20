/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CoapLayer.h
 * Author: Hoang Hoi
 *
 * Created on Ngày 06 tháng 4 năm 2017, 12:25
 */

#ifndef COAPLAYER_H
#define COAPLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <Arduino.h>
#include "coap_conf.h"
#include "coap_enum.h"

class CoapLayer {
public:
    CoapLayer();
    CoapLayer(const CoapLayer& orig);
    virtual ~CoapLayer();
    static int parse(coap_packet_t *pkt, const uint8_t *buf, size_t buflen);
    static int make(coap_method_t method, coap_packet_t *pkt, String uri, const uint8_t *payload, size_t payloadlen, uint16_t msgId, const coap_buffer_t* tok);
    static int bufferToString(char *strbuf, size_t strbuflen, const coap_buffer_t *buf);
    static const coap_option_t *findOptions(const coap_packet_t *pkt, uint8_t num, uint8_t *count);
    static int build(uint8_t *buf, size_t *buflen, const coap_packet_t *pkt);
    static int makeResponse(coap_rw_buffer_t *scratch, coap_packet_t *pkt, const uint8_t *content, size_t content_len, uint8_t msgid_hi, uint8_t msgid_lo, const coap_buffer_t* tok, coap_responsecode_t rspcode, coap_content_type_t content_type);
    static void optionNibble(uint32_t value, uint8_t *nibble);
    static void setup(void);
private:
    static int parseHeader(coap_header_t *hdr, const uint8_t *buf, size_t buflen);
    static int parseToken(coap_buffer_t *tokbuf, const coap_header_t *hdr, const uint8_t *buf, size_t buflen);
    static int parseOption(coap_option_t *option, uint16_t *running_delta, const uint8_t **buf, size_t buflen);
    static int parseOptionsAndPayload(coap_option_t *options, uint8_t *numOptions, coap_buffer_t *payload, const coap_header_t *hdr, const uint8_t *buf, size_t buflen);

};

#endif /* COAPLAYER_H */

