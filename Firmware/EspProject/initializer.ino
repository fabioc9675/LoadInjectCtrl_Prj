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
    pinMode(BOTON_1, INPUT_PULLUP);
    pinMode(BOTON_2, INPUT_PULLUP);
    pinMode(BOTON_3, INPUT_PULLUP);
    pinMode(BOTON_INGRESO, INPUT_PULLUP);
    pinMode(PIN_RELE1, OUTPUT);
    pinMode(PIN_RELE2, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);

    digitalWrite(PIN_RELE1, HIGH);
    digitalWrite(PIN_RELE2, HIGH);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/