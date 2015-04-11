#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "w5200_config.h"
#include "w5200_buf.h"
#include "w5200_io.h"
#include "w5200_sock.h"

/* Default IPs and utility subnets */
const uint16_t w52_const_subnet_classA[2] = {0xFF00, 0x0000};
const uint16_t w52_const_subnet_classB[2] = {0xFFFF, 0x0000};
const uint16_t w52_const_subnet_classC[2] = {0xFFFF, 0xFF00};
const uint16_t w52_const_ip_default[2] = {0xA980, 0x8082};  // 169.128.128.130
const uint16_t w52_const_mac_default[3] = {0x5452, 0x0000, 0xF801};  // 54:52:00:00:F8:01 ... sounds random enough

uint8_t ip_addr[4] = {0x1F,0x91,0x1F,0x91};
//Use IANA recommended ephemeral port range 49152-65535
#define DST_PORT 0xC350
/* IRQ handler flag */
volatile uint8_t w5200_irq;
uint16_t w52_portoffset;

/* TCP state descriptions */
const char * wiznet_tcp_state[] = {
        "ESTABLISHED",
        "SYN_SENT",
        "SYN_RECV",
        "FIN_WAIT",
        "TIME_WAIT",
        "CLOSE",
        "CLOSE_WAIT",
        "LAST_ACK",
        "LISTEN",
        "CLOSING" };

const uint8_t wiznet_tcp_state_idx[] = {
        W52_SOCK_SR_SOCK_ESTABLISHED,
        W52_SOCK_SR_SOCK_SYNSENT,
        W52_SOCK_SR_SOCK_SYNRECV,
        W52_SOCK_SR_SOCK_FIN_WAIT,
        W52_SOCK_SR_SOCK_TIME_WAIT,
        W52_SOCK_SR_SOCK_CLOSED,
        W52_SOCK_SR_SOCK_CLOSE_WAIT,
        W52_SOCK_SR_SOCK_LAST_ACK,
        W52_SOCK_SR_SOCK_LISTEN,
        W52_SOCK_SR_SOCK_CLOSING };

/* Open socket information */
typedef struct {
    uint16_t dst_mask;
    uint16_t dst_ptr;
} WIZNETSocketState;

WIZNETSocketState w52_sockets[W52_MAX_SOCKETS];

int wiznet_sockets() {
    uint16_t i;
    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        uint16_t s = read_SnSR(i);
        if (s == W52_SOCK_SR_SOCK_CLOSED || s == W52_SOCK_SR_SOCK_FIN_WAIT) {
            write_SnMR(i, W52_SOCK_MR_PROTO_UDP);
            write_SnPORT(i, i);
            write_SnDIPR(i,ip_addr);
            write_SnDPORT(i, DST_PORT);
            
//            // dst_mask is offset address
//            w52_sockets[i].dst_mask = (read_SnTX_WR(i) & gSn_TX_MASK);
//            // dst_ptr is physical start address
//            w52_sockets[i].dst_ptr = gSn_TX_BASE + w52_sockets[i].dst_mask;

            exec_cmdSn(i, W52_SOCK_CMD_OPEN);
        } else {
            // Not all ports available...
            return -ENFILE;
        }
    }
}


void exec_cmdSn(uint16_t s, uint16_t _cmd) {
    // Send command to socket
    write_SnCR(s, _cmd);
    // Wait for command to complete
    while (read_SnCR(s))
        ;
}