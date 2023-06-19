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

// Funciones para pender y apagar estado de rele
void loadTurn_On(void)
{
    digitalWrite(RELAY_LOAD, LOW);  // prende carga
    digitalWrite(RELAY_INJE, HIGH); // apaga injeccion
    digitalWrite(LED_LOAD, HIGH);   // Prende led indicador
    digitalWrite(LED_INJE, LOW);    // apaga led indicador
}

void injeTurn_On(void)
{
    digitalWrite(RELAY_LOAD, HIGH); // apaga carga
    digitalWrite(RELAY_INJE, LOW);  // prende injeccion
    digitalWrite(LED_LOAD, LOW);    // apaga led indicador
    digitalWrite(LED_INJE, HIGH);   // Prende led indicador
}

void relayTurn_Off(void)
{
    digitalWrite(RELAY_LOAD, HIGH); // apaga carga
    digitalWrite(RELAY_INJE, HIGH); // apaga injeccion
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/