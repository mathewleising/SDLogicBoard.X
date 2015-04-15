#include <stdint.h>
#include "w5200_config.h"
#include "w5200.h"
#include "w5200_regs.h"
#include "w5200_io.h"
#include "config.h"
#include "spi.h"
#include "delay.h"

#define QUEUESIZE 50


int count;
int sock;
int16_t qd[QUEUESIZE];
int16_t *fst;
int16_t *lst;

void hard_reset(void);

int w5200_init(void)
{
    count = 0;
    sock = 0;
    fst = &qd[0];
    lst = &qd[0];
    
    // Turn everything on
    W5200_PWR_ON;
    W5200_CS_STOP;
    W5200_RST_STOP;
    // Give it some time
    delay_millis(200);

    // Go hard in the paint
    hard_reset();

    uint8_t vsr = read_VRSN();
    while(vsr != 0x03)
    {
        vsr = read_VRSN();
        delay_for_1000_nops_x(8);
    }

    write_MR(MR_CONF);
    write_GAR(gateway_ip);
    write_SUBR(w52_const_subnet_classC);
    write_SHAR(w52_const_mac_default);
    write_SIPR(dest_ip);
    write_IMR(IMR_CONF);
    write_IMR(IMR2_CONF);
    write_IR2(IR2_CONF);
    write_PHYST(PHY_CONF);

    return init_sockets();
}

int w5200_buff(int16_t data)
{
    if (count >= QUEUESIZE)
        return -1;
    
    if (count == 0) // empty
    {
        count = 1;
        fst = &qd[0];
        lst = &qd[0];
    }
    else // 1 thing queued
    {
        count = 1;
        if (lst == &qd[QUEUESIZE-1])
        {
            lst = &qd[0];
        }
        else
        {
            lst++;
        }
    }

    *lst = data;

    count++;
    
    return 0;
}

void w5200_update(void)
{
    int16_t data;

    if (count > 0)
    {
        data = *fst;
        
        if (count != 1)
        {
            if (fst == &qd[QUEUESIZE-1])
            {
                fst = &qd[0];
            }
            else
            {
                fst++;
            }
        }

        count--;

        //Place data in socket
        put_socket(data);
    }
}

void hard_reset(void)
{
    W5200_RST_START;
    delay_millis(200); // Asks for 150m, 200 to be safe
    W5200_RST_STOP;
    delay_millis(200); // Asks for 150m, 200 to be safe
}