/* 
 * File:   w5200.h
 * Author: mtleising
 *
 * Created on April 7, 2015, 5:45 PM
 */

#ifndef W5200_H
#define	W5200_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "w5200_io.h"

int w5200_init(void);
int w5200_buff(int16_t data);
void w5200_update(void);


#ifdef	__cplusplus
}
#endif

#endif	/* W5200_H */

