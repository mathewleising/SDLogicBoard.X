#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "w5200_config.h"
#include "w5200_buf.h"
#include "w5200_io.h"
#include "w5200_sock.h"


void exec_cmdSn(uint16_t s, uint8_t _cmd);

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
    uint16_t s;
    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        s = read_SnSR(i);
        if (s == W52_SOCK_SR_SOCK_CLOSED || s == W52_SOCK_SR_SOCK_FIN_WAIT) {
            write_SnMR(i, W52_SOCK_MR_PROTO_UDP);
            write_SnPORT(i, i);
            write_SnDIPR(i, dest_ip);
            write_SnDPORT(i, DST_PORT);
            // May need to implement this... write_SnIMR();

            
//            gS0_RX_BASE = 0x0000(Chip base address) + 0xC000(Internal RX buffer address); // Set base address of RX memory for SOCKET 0
//            Sn_RXMEM_SIZE(ch) = (uint8 *) 2; // Assign 2K rx memory per SOCKET
//            gS0_RX_MASK = 2K ? 1; // 0x07FF, for getting offset address within assigned SOCKET 0 RX memory
//            gS1_RX_BASE = gS0_RX_BASE + (gS0_RX_MASK + 1);
//            gS1_RX_MASK = 2K ? 1;
            exec_cmdSn(i, W52_SOCK_CMD_OPEN);
        } else {
            // Not all ports available...
            return -ENFILE;
        }
    }

    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        w52_sockets[i].tx_size = read_SnTX_FSR(i);
        w52_sockets[i].dst_base_ptr = read_SnTX_WR(i);
        w52_sockets[i].dst_write_ptr = read_SnTX_RD(i);
    }

    return 0;
}

void put_socket(uint16_t data) {
    // Send data to W5200 buffer
    wiznet_w_reg16(w52_sockets[current_sock].dst_write_ptr, data);
    
    w52_sockets[current_sock].dst_write_ptr++;

    if (w52_sockets[current_sock].tx_size == (w52_sockets[current_sock].dst_base_ptr - w52_sockets[current_sock].dst_write_ptr))
    {
        exec_cmdSn(current_sock, W52_SOCK_CMD_SEND);

        w52_sockets[current_sock].dst_write_ptr = w52_sockets[current_sock].dst_base_ptr;

        if (current_sock<7)
        {
            current_sock++;
        }
        else
        {
            current_sock = 0;
        }
    }
}


void exec_cmdSn(uint16_t s, uint8_t _cmd) {
    // Send command to socket
    write_SnCR(s, _cmd);

    // Wait for command to complete
    while (read_SnCR(s))
        ;
}