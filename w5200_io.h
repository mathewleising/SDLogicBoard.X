/* Wiznet W5200 Sys I/O driver for TI MSP430
 *
 *
 * Copyright (c) 2013, Eric Brundick <spirilis@linux.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright notice
 * and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT,
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef W5200_IO_H
#define W5200_IO_H

#include <stdint.h>
#include "w5200_regs.h"

/* Register I/O primitives */
void wiznet_w_reg(uint16_t, uint8_t data);
uint8_t wiznet_r_reg(uint16_t addr);
void wiznet_w_reg16(uint16_t, uint16_t data);
uint16_t wiznet_r_reg16(uint16_t addr);

void wiznet_w_buff(uint16_t addr, uint8_t *data, uint16_t len);
void wiznet_r_buff(uint16_t addr, uint8_t *data, uint16_t len);

#define wiznet_w_sockreg(sock, addr, val) wiznet_w_reg(W52_SOCK_REG_RESOLVE(sock, addr), val)
#define wiznet_r_sockreg(sock, addr) wiznet_r_reg(W52_SOCK_REG_RESOLVE(sock, addr))
#define wiznet_w_sockreg16(sock, addr, val) wiznet_w_reg16(W52_SOCK_REG_RESOLVE(sock, addr), val)
#define wiznet_r_sockreg16(sock, addr) wiznet_r_reg16(W52_SOCK_REG_RESOLVE(sock, addr))
#define wiznet_w_sockreg_buff(sock, addr, data, len) wiznet_w_buff(W52_SOCK_REG_RESOLVE(sock, addr), data, len)
#define wiznet_r_sockreg_buff(sock, addr, data, len) wiznet_r_buff(W52_SOCK_REG_RESOLVE(sock, addr), data, len)

#define __GP_REGISTER8(name, address)\
  static inline void write_##name(uint8_t _data) {\
    wiznet_w_reg(address, _data);\
  }\
  static inline uint8_t read_##name() {\
    return wiznet_r_reg(address);\
  }

#define __GP_REGISTER16(name, address)\
  static void write_##name(uint16_t _data) {\
    wiznet_w_reg16(address,   _data);\
  }\
  static uint16_t read_##name() {\
    uint16_t res = wiznet_r_reg16(address);\
    return res;\
  }

#define __GP_REGISTER_N(name, address, size)\
  static uint16_t write_##name(uint8_t *_buff) {\
    wiznet_w_buff(address, _buff, size);\
  }\
  static uint16_t read_##name(uint8_t *_buff) {\
    wiznet_r_buff(address, _buff, size);\
  }

#define __SOCKET_REGISTER8(name, address)\
  static inline void write_##name(uint16_t _s, uint8_t _data) {\
    wiznet_w_sockreg(_s, address, _data);\
  }\
  static inline uint8_t read_##name(uint16_t _s) {\
    return wiznet_r_sockreg(_s, address);\
  }

#define __SOCKET_REGISTER16(name, address)\
  static void write_##name(uint16_t _s, uint16_t _data) {\
    wiznet_w_sockreg16(_s, address,   _data);\
  }\
  static uint16_t read_##name(uint16_t _s) {\
    return wiznet_r_sockreg16(_s, address);\
  }

#define __SOCKET_REGISTER_N(name, address, size)\
  static uint16_t write_##name(uint16_t _s, uint8_t *_buff) {\
    wiznet_w_sockreg_buff(_s, address, _buff, size);\
  }\
  static uint16_t read_##name(uint16_t _s, uint8_t *_buff) {\
    wiznet_r_sockreg_buff(_s, address, _buff, size);\
  }

  __GP_REGISTER8 (MR,     W52_MR);    // Mode
  __GP_REGISTER_N(GAR,    W52_GATEWAY, 4); // Gateway IP address
  __GP_REGISTER_N(SUBR,   W52_SUBNETMASK, 4); // Subnet mask address
  __GP_REGISTER_N(SHAR,   W52_SOURCEMAC, 6); // Source MAC address
  __GP_REGISTER_N(SIPR,   W52_SOURCEIP, 4); // Source IP address
  __GP_REGISTER8 (IR,     W52_IR);    // Interrupt
  __GP_REGISTER8 (IMR,    W52_IMR);    // Interrupt Mask
  __GP_REGISTER16(RTR,    W52_RTR0);    // Timeout address
  __GP_REGISTER8 (RCR,    W52_RCR);    // Retry count
  __GP_REGISTER8 (RMSR,   W52_RMSR);    // Receive memory size
  __GP_REGISTER8 (TMSR,   W52_TMSR);    // Transmit memory size
  __GP_REGISTER8 (PATR,   W52_PPPOE_AUTHTYPE);    // Authentication type address in PPPoE mode
  __GP_REGISTER8 (PTIMER, W52_PTIMER);    // PPP LCP Request Timer
  __GP_REGISTER8 (PMAGIC, W52_PMAGIC);    // PPP LCP Magic Number
  __GP_REGISTER_N(UIPR,   W52_UIPR, 4); // Unreachable IP address in UDP mode
  __GP_REGISTER16(UPORT,  W52_UPORT);    // Unreachable Port address in UDP mode
  __SOCKET_REGISTER8(SnMR,        W52_SOCK_MR)        // Mode
  __SOCKET_REGISTER8(SnCR,        W52_SOCK_CR)        // Command
  __SOCKET_REGISTER8(SnIR,        W52_SOCK_IR)        // Interrupt
  __SOCKET_REGISTER8(SnSR,        W52_SOCK_SR)        // Status
  __SOCKET_REGISTER16(SnPORT,     W52_SOCK_SRCPORT)        // Source Port
  __SOCKET_REGISTER_N(SnDHAR,     W52_SOCK_DESTMAC, 6)     // Destination Hardw Addr
  __SOCKET_REGISTER_N(SnDIPR,     W52_SOCK_DESTIP, 4)     // Destination IP Addr
  __SOCKET_REGISTER16(SnDPORT,    W52_SOCK_DESTPORT)        // Destination Port
  __SOCKET_REGISTER16(SnMSSR,     W52_SOCK_MSS)        // Max Segment Size
  __SOCKET_REGISTER8(SnPROTO,     W52_SOCK_PROTO)        // Protocol in IP RAW Mode
  __SOCKET_REGISTER8(SnTOS,       W52_SOCK_TOS)        // IP TOS
  __SOCKET_REGISTER8(SnTTL,       W52_SOCK_TTL)        // IP TTL
  __SOCKET_REGISTER16(SnTX_FSR,   W52_SOCK_TXFREE_SIZE)        // TX Free Size
  __SOCKET_REGISTER16(SnTX_RD,    W52_SOCK_TX_READPTR)        // TX Read Pointer
  __SOCKET_REGISTER16(SnTX_WR,    W52_SOCK_TX_WRITEPTR)        // TX Write Pointer
  __SOCKET_REGISTER16(SnRX_RSR,   W52_SOCK_RX_RECVSIZE)        // RX Free Size
  __SOCKET_REGISTER16(SnRX_RD,    W52_SOCK_RX_READPTR)        // RX Read Pointer
  __SOCKET_REGISTER16(SnRX_WR,    W52_SOCK_RX_WRITEPTR)        // RX Write Pointer (supported?)

#endif
