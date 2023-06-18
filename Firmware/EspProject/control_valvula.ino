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

/** ****************************************************************************
 ** ************ DEFINES *******************************************************
 ** ****************************************************************************/

//********DEFINICIONES CONDICIONES******
#define TRUE 1
#define FALSE 0

#define POS_LOAD 0
#define POS_INJE 7

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

/** ****************************************************************************
 ** ************ STRUCTURES ****************************************************
 ** ****************************************************************************/

typedef struct
{
  int second;
  int minute;
  String secondStr;
  String minuteStr;
  String timeToPrint;
} Control;

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

// Definir variables globales
int tiempo1 = 0;
int tiempo2 = 0;
int tiempo_ciclo = 0;
unsigned long tiempoActual = 0;
int cursorPosition = 0;

volatile boolean boton1 = false;
volatile boolean boton2 = false;
volatile boolean boton3 = false;
volatile boolean botonIngreso = false;
bool relesEncendidos = true;

int contador = 0;

// Crear un objeto de la clase LiquidCrystal_I2C para la pantalla LCD
LiquidCrystal_I2C lcd(LCD_ADDR, 20, 4);

/** ****************************************************************************
 ** ************ FUNCTIONS *****************************************************
 ** ****************************************************************************/

void AssignTimeValue(Control *control);
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
  lcd.print("Load   Inject  Ciclo");

  AssignTimeValue(&loadTime);
  AssignTimeValue(&injeTime);
  AssignTimeValue(&ciclTime);

  lcd.setCursor(POS_LOAD, 2);
  lcd.print(loadTime.timeToPrint);
  lcd.setCursor(POS_INJE, 2);
  lcd.print(injeTime.timeToPrint);

  configState = LOAD_MIN;        // pasa al estado de configuracion de minutos para LOAD
  positionCursor = POS_LOAD_MIN; // Coloca el cursoe en la posicion de minutos para carga

  lcd.setCursor(POS_LOAD_MIN, 2);
  lcd.blink(); // Activar el parpadeo del cursor
}

/* ***************************************************************************
 * **** LOOP MAIN_MENU *******************************************************
 * ***************************************************************************/

void loop()
{

  if (flagTimerInterrupt)
  {
    flagTimerInterrupt = FALSE;

    contador++;
    if (contador >= 1000)
    {
      contador = 0;
      Serial.println("interrupcion");
    }
  }

  if (boton1)
  {

    if (cursorPosition == 0)
    {
      tiempo1++;
      lcd.setCursor(2, 1);
      if (tiempo1 <= 9)
      {
        lcd.print(" ");
      }
      lcd.print(tiempo1);
    }
    else if (cursorPosition == 1)
    {
      tiempo2++;
      lcd.setCursor(7, 1);
      lcd.print(tiempo2);
    }
    else if (cursorPosition == 2)
    {
      tiempo_ciclo += 10;
      lcd.setCursor(13, 1);
      lcd.print(tiempo_ciclo);
    }
    delay(300);
    boton1 = false;
  }

  if (boton2)
  {
    // boton2 = false;

    if (cursorPosition == 0)
    {
      tiempo1--;
      if (tiempo1 < 0)
      {
        tiempo1 = 0;
      }
      lcd.setCursor(2, 1);
      if (tiempo1 <= 9)
      {
        lcd.print(" ");
      }
      lcd.print(tiempo1);
    }
    else if (cursorPosition == 1)
    {
      tiempo2--;
      if (tiempo2 < 0)
      {
        tiempo2 = 0;
      }
      lcd.setCursor(7, 1);
      lcd.print(tiempo2);
    }
    else if (cursorPosition == 2)
    {
      tiempo_ciclo -= 10;
      if (tiempo_ciclo < 0)
      {
        tiempo_ciclo = 0;
      }
      lcd.setCursor(13, 1);
      lcd.print(tiempo_ciclo);
    }
    delay(300);
    boton2 = false;
  }

  if (boton3)
  {
    // Cambia de estado
    nextStateControl();
    lcd.setCursor(positionCursor, 2);

    delay(300);     // debouncing time
    boton3 = false; // habilita la bandera para atender una nueva interrupcion
  }

  if (botonIngreso)
  {
    botonIngreso = false;

    unsigned long startTime = millis(); // Tiempo de inicio del ciclo
    unsigned long elapsedTime = 0;      // Tiempo transcurrido

    while (elapsedTime < tiempo_ciclo * 1000 && relesEncendidos)
    { // Ejecutar durante tiempo ciclo

      digitalWrite(LED_1, HIGH);
      digitalWrite(LED_2, LOW);

      // Encender el primer LED y apagar el segundo LED
      digitalWrite(PIN_RELE1, LOW);
      digitalWrite(PIN_RELE2, HIGH);
      Serial.println("led1 on");
      unsigned long tiempo1Start = millis();
      delay(2000);
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, HIGH);

      while (millis() - tiempo1Start < tiempo1 * 1000)
      {
        // Esperar el tiempo correspondiente
      }

      digitalWrite(LED_1, LOW);
      digitalWrite(LED_2, HIGH);

      // Encender el segundo LED y apagar el primer LED
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, LOW);
      Serial.println("led2 on");
      unsigned long tiempo2Start = millis();

      delay(2000);
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, HIGH);
      while (millis() - tiempo2Start < tiempo2 * 1000)
      {
        // Esperar el tiempo correspondiente
      }

      // Actualizar el tiempo transcurrido
      elapsedTime = millis() - startTime;
    }

    // Apagar ambos LEDs al finalizar el ciclo
    digitalWrite(PIN_RELE1, HIGH);
    digitalWrite(PIN_RELE2, HIGH);

    relesEncendidos = false; // Desactivar el bucle para que no se reinicie
  }
}

/* ***************************************************************************
 * **** Functions definition *************************************************
 * ***************************************************************************/
// Funcion para asignar el valor de tiempo a la estructura
void AssignTimeValue(Control *control)
{

  // Evalua si el valor de minuto es menor que 10
  if (control->minute < 10)
  {
    control->minuteStr = "0" + String(control->minute);
  }
  else
  {
    control->minuteStr = String(control->minute);
  }
  // Evalua si el valor de segundo es menor que 10
  if (control->second < 10)
  {
    control->secondStr = "0" + String(control->second);
  }
  else
  {
    control->secondStr = String(control->second);
  }

  control->timeToPrint = control->minuteStr + ":" + control->secondStr;
}

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
