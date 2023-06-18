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
extern volatile boolean boton1;
extern volatile boolean boton2;
extern volatile boolean boton3;
extern volatile boolean botonIngreso;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
// Interrupcion por presion del switch

// Interrupcion por encoder A
void IRAM_ATTR ISR_boton1(void)
{
    // da el semaforo para que quede libre para la tarea pulsador
    portENTER_CRITICAL_ISR(&mux);
    boton1 = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_boton2(void)
{
    portENTER_CRITICAL_ISR(&mux);
    boton2 = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_boton3(void)
{
    portENTER_CRITICAL_ISR(&mux);
    boton3 = true;
    portEXIT_CRITICAL_ISR(&mux);
}
// Interrupcion por encoder A
void IRAM_ATTR ISR_botonIngreso(void)
{
    portENTER_CRITICAL_ISR(&mux);
    botonIngreso = true;
    portEXIT_CRITICAL_ISR(&mux);
}

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
void init_IRQ(void)
{
    attachInterrupt(digitalPinToInterrupt(BOTON_1), ISR_boton1, FALLING);
    attachInterrupt(digitalPinToInterrupt(BOTON_2), ISR_boton2, FALLING);
    attachInterrupt(digitalPinToInterrupt(BOTON_3), ISR_boton3, FALLING);
    attachInterrupt(digitalPinToInterrupt(BOTON_INGRESO), ISR_botonIngreso, FALLING);
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/
