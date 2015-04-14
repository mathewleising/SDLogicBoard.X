/* w5200_config.h
 * WizNet W5200 Ethernet Controller Driver
 * TI MSP430 Edition
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

#ifndef W5200_CONFIG_H
#define W5200_CONFIG_H

#include <stdint.h>
#include "w5200_regs.h"

/* Custom statements used to temporarily set SPI speed (and restore afterwards)
 * The W5200 supports up to 33MHz SPI guaranteed (80MHz if you can manage the crosstalk).
 * It is advantageous to set SMCLK to the full speed of the DCO if possible.
 * Use this to minimize the SPI bitrate divider and restore it as needed (in case other
 * attached devices require lower SPI speeds)
 */

/* Method used to wait between issuing an operation and waiting for IRQ */
#define WIZNET_CPU_WAIT LPM0

/* Use pedantic checking (i.e. verify socket file descriptor is within allowed range,
 * verify PHYSTATUS link is up when performing operations, etc)
 * Comment this out to disable.
 */
#define W52_PEDANTIC_CHECKING 1

/* Socket memory buffer sizes
 * Defaults to 2KB, should be kept here unless you lower the # of max sockets.
 */
#define W52_SOCK_MEM_SIZE 2048
#define W52_SOCK_MEM_MASK 2047

/* End user configuration */

// Structure for holding socket information
#define W52_MAX_SOCKETS 8

//typedef struct {
//	uint8_t mode;
//	uint8_t srcport_idx;
//	uint8_t is_bind;
//	uint16_t tx_wr;
//	uint16_t rx_rd;
//} WIZNETSocketState;
//
//extern WIZNETSocketState w52_sockets[W52_MAX_SOCKETS];


/* Default IPs and utility subnets */
#define MR_CONF 0x00
#define IMR_CONF 0x00
static uint8_t w52_const_subnet_classA[4] = {0xFF, 0x00, 0x00, 0x00};
static uint8_t w52_const_subnet_classB[4] = {0xFF, 0xFF, 0x00, 0x00};
static uint8_t w52_const_subnet_classC[4] = {0xFF, 0xFF, 0xFF, 0x00};
static uint8_t w52_const_ip_default[4] = {0xA9, 0x80, 0x80, 0x82};  // 169.128.128.130
static uint8_t w52_const_mac_default[6] = {0x54, 0x52, 0x00, 0x00, 0xF8, 0x01};  // 54:52:00:00:F8:01 ... sounds random enough
static uint8_t dest_ip[4] = {0x1F,0x91,0x1F,0x91};
//Use IANA recommended ephemeral port range 49152-65535
#define DST_PORT 0xC350




/* Relevant ERRNO values */
#define ENETDOWN 100
#define EBADF 9
#define EFAULT 14
#define EPROTONOSUPPORT 93
#define ENFILE 23

#define EADDRINUSE 98

#define ESHUTDOWN 108
#define ENOTCONN 107
#define ECONNREFUSED 111
#define ECONNABORTED 103
#define ECONNRESET 104
#define EHOSTDOWN 112
#define EHOSTUNREACH 113
#define ETIMEDOUT 110

#define EINPROGRESS 115
#define EISCONN 106
#define EAGAIN 11

#endif
