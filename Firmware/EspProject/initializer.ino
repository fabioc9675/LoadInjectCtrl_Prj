/*
 * File:   initializer.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "initializer.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// **********************************************************

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// inicializacion de los pines del microcontrolador
void init_GPIO(void)
{
    // Configurar los pines de los pulsadores y el relé
    pinMode(BUTTON_INCREMENT, INPUT_PULLUP);
    pinMode(BUTTON_DECREMENT, INPUT_PULLUP);
    pinMode(BUTTON_SELECT, INPUT_PULLUP);
    pinMode(BUTTON_OK, INPUT_PULLUP);

    pinMode(RELAY_LOAD, OUTPUT);
    pinMode(RELAY_INJE, OUTPUT);
    pinMode(LED_LOAD, OUTPUT);
    pinMode(LED_INJE, OUTPUT);

    digitalWrite(RELAY_LOAD, HIGH);
    digitalWrite(RELAY_INJE, HIGH);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/