/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   coap_conf.h
 * Author: Hoang Hoi
 *
 * Created on Ngày 02 tháng 4 năm 2017, 01:10
 */

#ifndef COAP_CONF_H
#define COAP_CONF_H 1

#include "coap_enum.h"

#define MAXOPT 16

//http://tools.ietf.org/html/rfc7252#section-3
typedef struct
{
    uint8_t ver;                /* CoAP version number */
    uint8_t t;                  /* CoAP Message Type */
    uint8_t tkl;                /* Token length: indicates length of the Token field */
    uint8_t code;               /* CoAP status code. Can be request (0.xx), success reponse (2.xx), 
                                 * client error response (4.xx), or rever error response (5.xx) 
                                 * For possible values, see http://tools.ietf.org/html/rfc7252#section-12.1 */
    uint8_t id[2];
} coap_header_t;

typedef struct
{
    const uint8_t *p;
    size_t len;
} coap_buffer_t;

typedef struct
{
    uint8_t *p;
    size_t len;
} coap_rw_buffer_t;

typedef struct
{
    uint8_t num;                /* Option number. See http://tools.ietf.org/html/rfc7252#section-5.10 */
    coap_buffer_t buf;          /* Option value */
} coap_option_t;

typedef struct
{
    coap_header_t hdr;          /* Header of the packet */
    coap_buffer_t tok;          /* Token value, size as specified by hdr.tkl */
    uint8_t numopts;            /* Number of options */
    coap_option_t opts[MAXOPT]; /* Options of the packet. For possible entries see
                                 * http://tools.ietf.org/html/rfc7252#section-5.10 */
    coap_buffer_t payload;      /* Payload carried by the packet */
} coap_packet_t;
typedef int (*coap_endpoint_func)(coap_rw_buffer_t *scratch, const coap_packet_t *inpkt, coap_packet_t *outpkt);
#define MAX_SEGMENTS 4  // 2 = /foo/bar, 3 = /foo/bar/baz
typedef struct
{
    uint8_t count;
    const char *elems[MAX_SEGMENTS];
} coap_endpoint_path_t;

typedef struct
{
    coap_method_t method;               /* (i.e. POST, PUT or GET) */
    coap_endpoint_func handler;         /* callback function which handles this 
                                         * type of endpoint (and calls 
                                         * coap_make_response() at some point) */
    const coap_endpoint_path_t *path;   /* path towards a resource (i.e. foo/bar/) */
} coap_endpoint_t;

#endif
