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
    uint16_t dst_base_ptr;
    uint16_t dst_write_ptr;
    uint16_t dst_write;
} WIZNETSocketState;

WIZNETSocketState w52_sockets[W52_MAX_SOCKETS];
uint16_t current_sock; // 0-7


int init_sockets() {
    current_sock = 0;
    uint16_t i;
    uint16_t s;
    uint16_t off = 0;
    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        s = read_SnSR(i);
        if (s == W52_SOCK_SR_SOCK_CLOSED || s == W52_SOCK_SR_SOCK_FIN_WAIT) {
            write_SnIRM(i, SOCK_IMR_CONF);
            write_SnMR(i, W52_SOCK_MR_PROTO_UDP);//W52_SOCK_MR_PROTO_UDP);
            write_SnPORT(i, i);
            write_SnDHAR(i, dest_mac);
            write_SnDIPR(i, dest_ip);
            write_SnDPORT(i, DST_PORT);
            //write_SnMSSR(i,1472);
//            write_SnRX_MEM(i, 0x02); 2k on reset..
//            write_SnTX_MEM(i, 0x02); 2k on reset..

            exec_cmdSn(i, W52_SOCK_CMD_OPEN);
            // May need to implement this... write_SnIMR();

            //tx_base = (W52_TXMEM_BASE + off);

//            write_SnRX_MEM(i, 0x02);
//            write_SnTX_MEM(i, 0x02);
//            write_SnTX_WR(i, tx_base);
//            write_SnRX_RD(i, (W52_RXMEM_BASE + off));
//
//            w52_sockets[i].tx_size = (0x0800-2);
//            w52_sockets[i].dst_base_ptr = tx_base;
//            w52_sockets[i].dst_write_ptr = tx_base+1;
//
//            wiznet_w_reg16(w52_sockets[i].dst_write_ptr, i);
//
//            off = off + 0x0800;
//
//            exec_cmdSn(i, W52_SOCK_CMD_OPEN);
        } else {
            // Not all ports available...
            return -ENFILE;
        }
    }

    for (i = 0; i < W52_MAX_SOCKETS; i++) {
        uint8_t sr = read_SnSR(i);
        if (sr != W52_SOCK_SR_SOCK_UDP)
            return -ENFILE;

//        uint16_t fsr = read_SnTX_FSR(i);
//        while (fsr == 0)
//            fsr = read_SnTX_FSR(i);

        uint8_t dummy = 0;

        while (dummy==0)
        {
            w52_sockets[i].dst_base_ptr = 0x8000+(i*0x0800);
            w52_sockets[i].dst_write_ptr = read_SnTX_WR(i);
            w52_sockets[i].dst_write = w52_sockets[i].dst_write_ptr;

            if (w52_sockets[i].dst_write != 0)
                dummy = 1;

            if (w52_sockets[i].dst_base_ptr < w52_sockets[i].dst_write)
                dummy = 1;
        }
    }
    
    return 0;
}

void put_socket(uint16_t data) {
    // Send data to W5200 buffer
    wiznet_w_reg16(w52_sockets[current_sock].dst_write_ptr, data);
    
    w52_sockets[current_sock].dst_write_ptr -= 2;

    if (w52_sockets[current_sock].dst_base_ptr >= w52_sockets[current_sock].dst_write_ptr)
    {
        exec_cmdSn(current_sock, W52_SOCK_CMD_SEND);

        w52_sockets[current_sock].dst_write_ptr = w52_sockets[current_sock].dst_write;

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