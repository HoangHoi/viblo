#ifndef IPV6STACK_H
#define IPV6STACK_H

extern "C" {
#include <inttypes.h>    
#include "uip.h"
#include "simple_udp.h"
}

#include <Arduino.h>
#include "MACLayer.h"
#include "IPv6Address.h"
#include "IPv6llAddress.h"
#include "IPv6Timer.h"

// define coap
#define COAP_PORT 5683
#define UIP_IPH_LEN    40   /* Size of IPv6 header */
#define UIP_UDPH_LEN    8    /* Size of UDP header */
#define UIP_IPUDPH_LEN (UIP_UDPH_LEN + UIP_IPH_LEN)    /* Size of IP + UDP header */
#define UIP_BUFSIZE  160 // The size of the uIP packet buffer.
#define UIP_UDP_PAYLOAD_MAX_BUFSIZE (UIP_BUFSIZE - UIP_IPUDPH_LEN) // The max size of the UDP datagram payload buffer. ( 160 - 48 )
#define COAP_PACKET_SIZE UIP_UDP_PAYLOAD_MAX_BUFSIZE

class IPv6Stack {
private:

    static uint8_t mac_output(uip_lladdr_t *lladdr);
    static MACLayer* mac; //this pointer is for the mac_output function to be able to access the mac layer
    static struct simple_udp_connection broadcast_connection;
    static void receiver(struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr,
            uint16_t sender_port, const uip_ipaddr_t *receiver_addr, uint16_t receiver_port, const uint8_t *data, uint16_t datalen);
    //Processes ipv6 received packets if any
    static void processIpStack();

    static const uint8_t* udp_data;
    static uint16_t udp_data_length;
    static uint16_t udp_data_length_left;
    static const uip_ipaddr_t* sender_addr;
    static uint16_t sender_port;
    static IPv6llAddress ll_destination_address;
    static IPv6llAddress ll_source_address;
    static uint8_t count;

    static const u16_t coap_port = 5683; // CoAP packets are sent 5683 by default.
    static const u16_t source_port = 61616; // UDP Source Port Number. Chosed 61616 (0xF0B0) for 6lowpan compression from 16 to 4 bits http://tools.ietf.org/html/rfc6282#section-4.3.1
    static char packetBuffer[ COAP_PACKET_SIZE ]; // Buffer to hold outgoing packets
    static unsigned int messageID;

public:

    //Initializes the mac layer (First function to call in setup). If it returns false, the MAC layer could not be initialized, so should stop working
    static bool initMacLayer(MACLayer* macLayer);
    //Initializes the variables of the IPv6 Stack (Second function to call in setup)
    static void initIpStack();
    //Initializes the UDP listening in the local port
    static void initUdp(uint16_t local_port);
    //Sends a UDP message. to: the destination IPv6 address, remote_port: the destination port, data: pointer to data, datalen: the length of the data. 
    static void udpSend(const IPv6Address &to, uint16_t remote_port, const void *data, uint16_t datalen);
    //Returns true if a packet has been received at MAC layer and has been successfully decompressed (6lowPAN)
    static bool receivePacket();
    //Refreshes timers and if expired, runs the corresponding methods (sending RS, RA, NS, etc)
    static void pollTimers(); //Note: polling timers put received udp data to 0
    //Adds manually an address to our list or addresses
    static void addAddress(IPv6Address &address);
    //When configured as router, this method allows to set a prefix to the router so it can communicate it with the RA
    static void setPrefix(IPv6Address &prefix, uint8_t prefix_length); //For ROUTERS only
    //In case we have a global address which has passed DAD, this returns true and copies the address to the parameter
    static bool getGlobalPreferredAddress(IPv6Address& address);
    //Returns the amount of bytes available from the last received UDP message (if any). Note: If timers are polled, this becomes 0 and that data is lost
    static uint16_t udpDataAvailable();
    //Reads one byte of the last UDP message received and gets ready to return the next byte of the data received (if any).
    static char readUdpData();
    //Returns the complete length of the data received by the last UDP message (if any). Note: If timers are polled, this becomes 0 and that data is lost
    static uint16_t getUdpDataLength();
    //Returns the data of the last UDP message received (if any), copied in the buffer passed by parameter (this buffer should have enough space for the data)
    static void getUdpData(uint8_t* buffer);
    //Returns the port used by the sender of the last received UDP message
    static uint16_t getUdpSenderPort();
    //Copies the IP address of the sender of the last received UDP message
    static void getUdpSenderIpAddress(IPv6Address &address);
    //Sends a PING (ICMPv6 Echo request) message to the specified address
    static void ping(IPv6Address &destination, uint8_t datalength);
    //Is Coap packet
    static uint8_t isCoapPacket();
    //Returns the data of the last COAP message received 
    static void getCoapData(uint8_t* buffer);
    //Return coap data length
    static uint16_t getCoapDataLength();

    /**
     * Coap layer
     */

    //Sends a sendCoAPpacket
    //    static void sendCoAPpacket(
    //        IPv6Address &address,
    //        String &uri,
    //        String &query,
    //        String &secondQuery,
    //        String &payload
    //    );
    //    static void sendCoap(IPv6Address &address, CoApPacket &packet);
    static void get(IPv6Address &address, String &uri);
    //    static void post(IPv6Address &address, String &uri, String &data);
    //    static void put(IPv6Address &address, String &uri, String &data);
    //    static void del(IPv6Address &address, String &uri);
    //    static void get(IPv6Address &address, const char uri[]);
    //    static void post(IPv6Address &address, const char uri[], const char data[]);
    //    static void put(IPv6Address &address, const char uri[], const char data[]);
    //    static void del(IPv6Address &address, const char uri[]);
};


#endif

