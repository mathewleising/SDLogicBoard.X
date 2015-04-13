#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "w5200_config.h"
#include "w5200_buf.h"
#include "w5200_io.h"
#include "w5200_sock.h"


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
    uint16_t tx_size;
    uint16_t dst_base_ptr;
    uint16_t dst_write_ptr;
} WIZNETSocketState;

WIZNETSocketState w52_sockets[W52_MAX_SOCKETS];
uint16_t current_sock; // 0-7

int init_sockets() {
    current_sock = 0;
    uint16_t i;
    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        uint16_t s = read_SnSR(i);
        if (s == W52_SOCK_SR_SOCK_CLOSED || s == W52_SOCK_SR_SOCK_FIN_WAIT) {
            write_SnMR(i, W52_SOCK_MR_PROTO_UDP);
            write_SnPORT(i, i);
            write_SnDIPR(i, dest_ip);
            write_SnDPORT(i, DST_PORT);
            // May need to implement this... write_SnIMR();

            w52_sockets[i].tx_size = read_SnTX_FSR(i);
            w52_sockets[i].dst_base_ptr = read_SnTX_WR(i);
            w52_sockets[i].dst_write_ptr = read_SnTX_RD(i);

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