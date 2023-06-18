/*
 * File:   interruption.ino
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "interruption.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// **********************************************************
extern portMUX_TYPE mux;

// bandera de activacion de botones
extern volatile boolean buttonIncrement;
extern volatile boolean buttonDecrement;
extern volatile boolean buttonSelect;
extern volatile boolean buttonOk;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// Interrupcion por presion del switch

// Interrupcion por encoder A
void IRAM_ATTR ISR_ButtonIncrement(void)
{
    // da el semaforo para que quede libre para la tarea pulsador
    portENTER_CRITICAL_ISR(&mux);
    buttonIncrement = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_ButtonDecrement(void)
{
    portENTER_CRITICAL_ISR(&mux);
    buttonDecrement = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_ButtonSelect(void)
{
    portENTER_CRITICAL_ISR(&mux);
    buttonSelect = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_ButtonOk(void)
{
    portENTER_CRITICAL_ISR(&mux);
    buttonOk = true;
    portEXIT_CRITICAL_ISR(&mux);
}

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
void init_IRQ(void)
{
    attachInterrupt(digitalPinToInterrupt(BUTTON_INCREMENT), ISR_ButtonIncrement, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_DECREMENT), ISR_ButtonDecrement, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_SELECT), ISR_ButtonSelect, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_OK), ISR_ButtonOk, FALLING);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
