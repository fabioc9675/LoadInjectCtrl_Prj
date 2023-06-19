/*
 * File:   initializer.h
 * Author: GIBIC UdeA
 *
 * Created on July 4, 2020, 13:41 PM
 */

#ifndef INITIALIZER_H
#define INITIALIZER_H

#ifdef __cplusplus
extern "C"
{
#endif

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>

  /** ****************************************************************************
   ** ************ DEFINES *******************************************************
   ** ****************************************************************************/
  //********DEFINICIONES DE HARDWARE******
  // Definir los pines de los pulsadores y el relé
#define BUTTON_INCREMENT 4
#define BUTTON_DECREMENT 16
#define BUTTON_SELECT 17
#define BUTTON_OK 5

#define RELAY_LOAD 18
#define RELAY_INJE 19

#define LED_LOAD 12
#define LED_INJE 13

//*********DEFINICIONES DE SOFTWARE*******
#define TRUE 1
#define FALSE 0

#define POS_LOAD 0
#define POS_INJE 7

#define TIME_RELAY 2

#define POS_LOAD_MIN 1
#define POS_LOAD_SEC 4
#define POS_INJE_MIN 8
#define POS_INJE_SEC 11

// Definir la dirección de la pantalla LCD
#define LCD_ADDR 0x27

// Definicion de casos para la configuracion de los tiempos
#define LOAD_MIN 1
#define LOAD_SEC 2
#define INJE_MIN 3
#define INJE_SEC 4

#define CASE_MIN 0
#define CASE_SEC 1

#define STOP_PROCESS 0
#define INIT_PROCESS 1

#define STOP_COUNTER 0
#define INIT_COUNTER 1
#define LOAD_CHANGE 2
#define LOAD_WAIT 3
#define INJE_CHANGE 4
#define INJE_WAIT 5

  /** ****************************************************************************
   ** ************ VARIABLES *****************************************************
   ** ****************************************************************************/

  /** ****************************************************************************
   ** ************ FUNCTIONS *****************************************************
   ** ****************************************************************************/
  void init_GPIO(void); // inicializacion de los pines del microcontrolador
  void loadTurn_On(void);
  void injeTurn_On(void);
  void relayTurn_Off(void);

  /* *****************************************************************************
   * *****************************************************************************
   * ******************** USO DE MODULO ADS **************************************
   * *****************************************************************************
   * *****************************************************************************/

  /* *****************************************************************************
   * *****************************************************************************
   * ***************** PROTOTYPE DEFINITION **************************************
   * *****************************************************************************
   * *****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* INITIALIZER_H */
