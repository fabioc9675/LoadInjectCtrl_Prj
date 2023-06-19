/*
 * File:   timefunctions.ino
 * Author: Fabian Castaño
 *
 * Created on June 19, 2023, 13:41 PM
 */

/** ****************************************************************************
 ** ************ INCLUDES ******************************************************
 ** ****************************************************************************/
#include "timefunctions.h"

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

// Funcion para decrementar el tiempo de control
void BackCounterTime(Control *control)
{
    control->totalTime--;
    // verifica si debe apagar el rele
    if ((control->setTimeTotal - control->totalTime) == TIME_RELAY)
    {
        control->relayOver = TRUE;
    }

    control->second--;
    if (control->second < 0)
    {
        if (control->minute == 0)
        {
            control->timeOver = TRUE;
            control->minute = control->setTimeMin;
            control->second = control->setTimeSec;
            control->totalTime = control->setTimeTotal;
            AssignTimeValue(control);
            return;
        }
        else
        {
            control->second = 59;
            control->minute--;
        }
    }
    AssignTimeValue(control);
}

// Funcion para incrementar valor de las variables
void IncrementValue(Control *control, int caseLabel)
{

    if (caseLabel == CASE_MIN)
    {
        control->minute++;
        if (control->minute > 59)
        {
            control->minute = 0;
        }
    }

    if (caseLabel == CASE_SEC)
    {
        control->second++;
        if (control->second > 59)
        {
            control->second = 0;
        }
    }

    // Asigna el valor a imprimir
    AssignTimeValue(control);
}

// Funcion para decrementar valor de las variables
void DecrementValue(Control *control, int caseLabel)
{

    if (caseLabel == CASE_MIN)
    {
        if (control->minute == 0)
        {
            control->minute = 60;
        }
        control->minute--;
    }

    if (caseLabel == CASE_SEC)
    {
        if (control->second == 0)
        {
            control->second = 60;
        }
        control->second--;
    }

    // Asigna el valor a imprimir
    AssignTimeValue(control);
}

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

    // Configuracion de tiempo seteado
    control->totalTime = control->minute * 60 + control->second;
    control->timeToPrint = control->minuteStr + ":" + control->secondStr;
}

// Funcion para asignar el valor de tiempo a la estructura
void SetControlTime(Control *control)
{
    control->setTimeMin = control->minute;
    control->setTimeSec = control->second;
    control->setTimeTotal = control->setTimeMin * 60 + control->setTimeSec;
}

/** ****************************************************************************
 ** ************ END OF THE CODE ***********************************************
 ** ****************************************************************************/