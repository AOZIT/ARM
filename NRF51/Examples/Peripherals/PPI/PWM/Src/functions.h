/**
 * @brief       functions.h
 * @details     Functions's header.
 *
 * @return      NA
 *
 * @author      Manuel Caballero
 * @date        10/November/2017
 * @version     10/November/2017   The ORIGIN
 * @pre         NaN
 * @warning     NaN
 * @pre         This code belongs to AqueronteBlog ( http://unbarquero.blogspot.com ).
 */

#include "nrf.h"
#include "nrf51_bitfields.h"
#include "variables.h"

/* FUNCTION PROTOTYPES */
void conf_CLK       ( void );
void conf_GPIO      ( void );
void conf_GPIOTE    ( void );
void conf_TIMER0    ( void );
void conf_PPI       ( void );
