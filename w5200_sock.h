/* 
 * File:   w5200_sock.h
 * Author: mtleising
 *
 * Created on April 9, 2015, 10:23 AM
 */

#ifndef W5200_SOCK_H
#define	W5200_SOCK_H

#ifdef	__cplusplus
extern "C" {
#endif

/* Const IP and MAC address values */
extern const uint16_t w52_const_subnet_classA[2];
extern const uint16_t w52_const_subnet_classB[2];
extern const uint16_t w52_const_subnet_classC[2];
extern const uint16_t w52_const_ip_default[2];
extern const uint16_t w52_const_mac_default[3];

extern const char *wiznet_tcp_state[10];
extern const uint8_t wiznet_tcp_state_idx[10];

/* Functions */
//int wiznet_irq_getsocket();
//#define wiznet_w_command(sock, cmdval) wiznet_w_sockreg(sock, W52_SOCK_CR, cmdval)
//int wiznet_phystate();

int init_sockets(void);
//int wiznet_close(int);
int wiznet_txcommit(int);
int wiznet_send(int, void *, uint16_t, uint8_t);

// Ethernet MACRAW I/O
//int wiznet_mac_recvfrom(void *, uint16_t, uint16_t *, uint16_t *, uint16_t *, uint8_t, uint8_t);
//int wiznet_mac_sendto(void *, uint16_t, uint16_t *, uint16_t, uint16_t, uint8_t, uint8_t);

//int wiznet_init();

#ifdef	__cplusplus
}
#endif

#endif	/* W5200_SOCK_H */

