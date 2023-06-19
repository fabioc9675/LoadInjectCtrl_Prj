/*
 * File:   control_valvula.ino
 * Author: Sebastian Hernandez - Fabian Castaño
 *
 * Created on June 17, 2023, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include <Arduino.h>
#include <Esp.h>

#include <stdio.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "initializer.h"
#include "timer.h"
#include "interruption.h"
#include "timefunctions.h"

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

/** ****************************************************************************
 ** ************ EXTERN VARIABLES **********************************************
 ** ****************************************************************************/
// definiciones para el timer
extern hw_timer_t *timer;
extern portMUX_TYPE timerMux;
extern portMUX_TYPE mux;

/** ****************************************************************************
 ** ************ VARIABLES *****************************************************
 ** ****************************************************************************/

// Estructura de datos
Control loadTime;
Control injeTime;
Control ciclTime;

// Banderas del sistema

volatile uint8_t flagTimerInterrupt;

int configState = 0;
int positionCursor = 0;
int processState = 0;
int counterState = 0;

// boleanos de interrupcion
volatile boolean buttonIncrement = false;
volatile boolean buttonDecrement = false;
volatile boolean buttonSelect = false;
volatile boolean buttonOk = false;

int contador = 0;

// Crear un objeto de la clase LiquidCrystal_I2C para la pantalla LCD
LiquidCrystal_I2C lcd(LCD_ADDR, 20, 4);

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/
void nextStateControl(void);

/* ***************************************************************************
 * **** CONFIGURACION ********************************************************
 * ***************************************************************************/

void setup()
{
  // Inicializar la pantalla LCD
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  init_GPIO();
  init_Timer();
  init_IRQ();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sistema LOAD-INJECT");
  lcd.setCursor(15, 3);
  lcd.print("GICM");

  delay(4000);

  // Mostrar el texto "Carga" en la pantalla LCD
  lcd.setCursor(0, 1);
  lcd.print("Load   Inject  ");

  loadTime.minute = 2;
  injeTime.minute = 2;

  AssignTimeValue(&loadTime);
  AssignTimeValue(&injeTime);
  AssignTimeValue(&ciclTime);

  lcd.setCursor(POS_LOAD, 2);
  lcd.print(loadTime.timeToPrint);
  lcd.setCursor(POS_INJE, 2);
  lcd.print(injeTime.timeToPrint);

  configState = LOAD_MIN; // pasa al estado de configuracion de minutos para LOAD
  counterState = STOP_COUNTER;
  positionCursor = POS_LOAD_MIN; // Coloca el cursoe en la posicion de minutos para carga

  lcd.setCursor(POS_LOAD_MIN, 2);
  lcd.blink(); // Activar el parpadeo del cursor
}

/* ***************************************************************************
 * **** LOOP MAIN_MENU *******************************************************
 * ***************************************************************************/

void loop()
{

  // Modo configuracion
  if (processState == STOP_PROCESS)
  {

    // Evaluacion del boton de incremento de valor
    if (buttonIncrement)
    {
      switch (configState)
      {
      case LOAD_MIN:
        IncrementValue(&loadTime, CASE_MIN);
        lcd.setCursor(POS_LOAD, 2);
        lcd.print(loadTime.timeToPrint);
        break;
      case LOAD_SEC:
        IncrementValue(&loadTime, CASE_SEC);
        lcd.setCursor(POS_LOAD, 2);
        lcd.print(loadTime.timeToPrint);
        break;
      case INJE_MIN:
        IncrementValue(&injeTime, CASE_MIN);
        lcd.setCursor(POS_INJE, 2);
        lcd.print(injeTime.timeToPrint);
        break;
      case INJE_SEC:
        IncrementValue(&injeTime, CASE_SEC);
        lcd.setCursor(POS_INJE, 2);
        lcd.print(injeTime.timeToPrint);
        break;

      default:
        break;
      }

      lcd.setCursor(positionCursor, 2);

      delay(300);
      buttonIncrement = false;
    }

    // Evaluacion del boton de decremento del valor
    if (buttonDecrement)
    {
      switch (configState)
      {
      case LOAD_MIN:
        DecrementValue(&loadTime, CASE_MIN);
        lcd.setCursor(POS_LOAD, 2);
        lcd.print(loadTime.timeToPrint);
        break;
      case LOAD_SEC:
        DecrementValue(&loadTime, CASE_SEC);
        lcd.setCursor(POS_LOAD, 2);
        lcd.print(loadTime.timeToPrint);
        break;
      case INJE_MIN:
        DecrementValue(&injeTime, CASE_MIN);
        lcd.setCursor(POS_INJE, 2);
        lcd.print(injeTime.timeToPrint);
        break;
      case INJE_SEC:
        DecrementValue(&injeTime, CASE_SEC);
        lcd.setCursor(POS_INJE, 2);
        lcd.print(injeTime.timeToPrint);
        break;

      default:
        break;
      }

      lcd.setCursor(positionCursor, 2);

      delay(300);
      buttonDecrement = false;
    }

    // Boton seleccion de dato
    if (buttonSelect)
    {
      // Cambia de estado
      nextStateControl();
      lcd.setCursor(positionCursor, 2);

      delay(300);           // debouncing time
      buttonSelect = false; // habilita la bandera para atender una nueva interrupcion
    }
  }

  else if (processState == INIT_PROCESS)
  {

    // Interrupcion por timer
    if (flagTimerInterrupt)
    {
      flagTimerInterrupt = FALSE;

      // Inicia el conteo
      if (counterState == INIT_COUNTER)
      {
        contador = 1;
        counterState = LOAD_CHANGE;
        loadTurn_On();
      }

      if (contador == 0)
      {
        // Maquina de estado para los cambios de estado
        switch (counterState)
        {
        case LOAD_CHANGE:
          // Prende carga
          BackCounterTime(&loadTime);

          lcd.setCursor(POS_LOAD, 2);
          lcd.print(loadTime.timeToPrint);

          if (loadTime.relayOver == TRUE)
          {
            loadTime.relayOver = FALSE;
            relayTurn_Off();
          }

          if (loadTime.timeOver == TRUE)
          {
            loadTime.timeOver = FALSE;
            counterState = INJE_CHANGE;
            injeTurn_On();
          }

          break;

        case INJE_CHANGE:
          // Prende injeccion
          BackCounterTime(&injeTime);

          lcd.setCursor(POS_INJE, 2);
          lcd.print(injeTime.timeToPrint);

          if (injeTime.relayOver == TRUE)
          {
            injeTime.relayOver = FALSE;
            relayTurn_Off();
          }

          if (injeTime.timeOver == TRUE)
          {
            injeTime.timeOver = FALSE;
            counterState = LOAD_CHANGE;
            loadTurn_On();
          }

          break;

        default:
          break;
        }

        Serial.print("**Proceso Iniciado** - LOAD: ");
        Serial.print(loadTime.timeToPrint);
        Serial.print(" - INJECT: ");
        Serial.println(injeTime.timeToPrint);
      }

      // Manejo de interrupcion por timer
      contador++;
      if (contador >= 1000)
      {
        contador = 0;
      }
    }
  }

  // ---------------------------------------------------------
  // -- Boton de iniciar proceso -----------------------------
  // ---------------------------------------------------------
  if (buttonOk)
  {

    switch (processState)
    {
    case STOP_PROCESS:
      processState = INIT_PROCESS;
      counterState = INIT_COUNTER;
      // Configura los setpoints de tiempo
      SetControlTime(&loadTime);
      SetControlTime(&injeTime);
      loadTime.timeOver = FALSE;
      loadTime.relayOver = FALSE;
      injeTime.timeOver = FALSE;
      injeTime.relayOver = FALSE;

      loadTime.minute = loadTime.setTimeMin;
      loadTime.second = loadTime.setTimeSec;
      injeTime.minute = injeTime.setTimeMin;
      injeTime.second = injeTime.setTimeSec;

      AssignTimeValue(&loadTime);
      AssignTimeValue(&injeTime);

      // Mostrar el texto "Carga" en la pantalla LCD
      lcd.setCursor(0, 1);
      lcd.print("Load   Inject  Ciclo");

      lcd.noBlink(); // Activar el parpadeo del cursor
      break;

    case INIT_PROCESS:
      processState = STOP_PROCESS;

      // Compone los contadores de load e injection
      relayTurn_Off(); // apaga ambos relays
      digitalWrite(LED_INJE, LOW);
      digitalWrite(LED_LOAD, LOW);

      loadTime.minute = loadTime.setTimeMin;
      loadTime.second = loadTime.setTimeSec;
      loadTime.totalTime = loadTime.setTimeTotal;
      injeTime.minute = injeTime.setTimeMin;
      injeTime.second = injeTime.setTimeSec;
      injeTime.totalTime = injeTime.setTimeTotal;

      configState = LOAD_MIN;        // pasa al estado de configuracion de minutos para LOAD
      positionCursor = POS_LOAD_MIN; // Coloca el cursoe en la posicion de minutos para carga

      // Mostrar el texto "Carga" en la pantalla LCD
      lcd.setCursor(0, 1);
      lcd.print("Load   Inject       ");
      lcd.setCursor(POS_LOAD_MIN, 2);
      lcd.blink(); // Activar el parpadeo del cursor
      break;

    default:
      break;
    }

    delay(300);       // debouncing time
    buttonOk = false; // habilita la bandera para atender una nueva interrupcion
  }
}

/* ***************************************************************************
 * **** Functions definition *************************************************
 * ***************************************************************************/
// Funcion para cambiar el estado en la maquina de estados
void nextStateControl(void)
{
  configState++;

  switch (configState)
  {
  case LOAD_MIN:
    positionCursor = POS_LOAD_MIN;
    break;
  case LOAD_SEC:
    positionCursor = POS_LOAD_SEC;
    break;
  case INJE_MIN:
    positionCursor = POS_INJE_MIN;
    break;
  case INJE_SEC:
    positionCursor = POS_INJE_SEC;
    break;

  default:
    configState = LOAD_MIN;
    positionCursor = POS_LOAD_MIN;
    break;
  }
}

/* ***************************************************************************
 * **** FIN DEL PROGRAMA *****************************************************
 * ***************************************************************************/
