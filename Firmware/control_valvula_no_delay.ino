#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definir los pines de los pulsadores y el relé
#define BOTON_1 4
#define BOTON_2 16
#define BOTON_3 17
#define BOTON_INGRESO 5
#define PIN_RELE1 18
#define PIN_RELE2 19

// Definir la dirección de la pantalla LCD
#define LCD_ADDR 0x27

portMUX_TYPE mux;

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

// Crear un objeto de la clase LiquidCrystal_I2C para la pantalla LCD
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);

void ARDUINO_ISR_ATTR  ISR_boton1() {
  boton1 = true;
}

void ARDUINO_ISR_ATTR  ISR_boton2() {
  boton2 = true;
}

void ARDUINO_ISR_ATTR  ISR_boton3() {
  boton3 = true;
}

void ARDUINO_ISR_ATTR  ISR_botonIngreso() {
  botonIngreso = true;
}

void setup() {
  // Inicializar la pantalla LCD
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // Configurar los pines de los pulsadores y el relé
  pinMode(BOTON_1, INPUT_PULLUP);
  pinMode(BOTON_2, INPUT_PULLUP);
  pinMode(BOTON_3, INPUT_PULLUP);
  pinMode(BOTON_INGRESO, INPUT_PULLUP);
  pinMode(PIN_RELE1, OUTPUT);
  pinMode(PIN_RELE2, OUTPUT);

  digitalWrite(PIN_RELE1, HIGH);
  digitalWrite(PIN_RELE2, HIGH);

  attachInterrupt(digitalPinToInterrupt(BOTON_1), ISR_boton1, FALLING);
  attachInterrupt(digitalPinToInterrupt(BOTON_2), ISR_boton2, FALLING);
  attachInterrupt(digitalPinToInterrupt(BOTON_3), ISR_boton3, FALLING);
  attachInterrupt(digitalPinToInterrupt(BOTON_INGRESO), ISR_botonIngreso, FALLING);

  // Mostrar el texto "Carga" en la pantalla LCD
  lcd.setCursor(0, 0);
  lcd.print("Carga");
  lcd.setCursor(6, 0);
  lcd.print("Inj");
  lcd.setCursor(11, 0);
  lcd.print("Ciclo");
}

void loop() {
  if (boton1) {


    if (cursorPosition == 0) {
      tiempo1++;
      lcd.setCursor(2, 1);
      if(tiempo1 <= 9){
        lcd.print(" ");
      }
      lcd.print(tiempo1);
    } else if (cursorPosition == 1) {
      tiempo2++;
      lcd.setCursor(7, 1);
      lcd.print(tiempo2);
    } else if (cursorPosition == 2) {
      tiempo_ciclo += 10;
      lcd.setCursor(13, 1);
      lcd.print(tiempo_ciclo);
    }
    delay(300);
    boton1 = false;
  }

  if (boton2) {
    //boton2 = false;

    if (cursorPosition == 0) {
      tiempo1--;
      if (tiempo1 < 0) {
        tiempo1 = 0;
      }
      lcd.setCursor(2, 1);
      if(tiempo1 <= 9){
        lcd.print(" ");
      }
      lcd.print(tiempo1);
    } else if (cursorPosition == 1) {
      tiempo2--;
      if (tiempo2 < 0) {
        tiempo2 = 0;
      }
      lcd.setCursor(7, 1);
      lcd.print(tiempo2);
    } else if (cursorPosition == 2) {
      tiempo_ciclo -= 10;
      if (tiempo_ciclo < 0) {
        tiempo_ciclo = 0;
      }
      lcd.setCursor(13, 1);
      lcd.print(tiempo_ciclo);
    }
    delay(300);
    boton2 = false;
  }

  if (boton3) {
    // boton3 = false;

    if (cursorPosition == 0) {
      lcd.setCursor(2, 1);
      cursorPosition = 1;
    } else if (cursorPosition == 1) {
      lcd.setCursor(7, 1);
      cursorPosition = 2;
    } else if (cursorPosition == 2) {
      lcd.setCursor(13, 1);
      cursorPosition = 0;
    }
    delay(300);
    boton3 = false;
    lcd.blink();  // Activar el parpadeo del cursor
    delay(1000);
    lcd.noBlink();  // Desactivar el parpadeo del cursor
  }

  if (botonIngreso) {
    botonIngreso = false;

    unsigned long startTime = millis();  // Tiempo de inicio del ciclo
    unsigned long elapsedTime = 0;       // Tiempo transcurrido

    while (elapsedTime < tiempo_ciclo * 1000 && relesEncendidos) {  // Ejecutar durante tiempo ciclo
      // Encender el primer LED y apagar el segundo LED
      digitalWrite(PIN_RELE1, LOW);
      digitalWrite(PIN_RELE2, HIGH);
      Serial.println("led1 on");
      unsigned long tiempo1Start = millis();
      delay(2000);
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, HIGH);

      while (millis() - tiempo1Start < tiempo1 * 1000) {
        // Esperar el tiempo correspondiente
      }

      // Encender el segundo LED y apagar el primer LED
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, LOW);
      Serial.println("led2 on");
      unsigned long tiempo2Start = millis();

      delay(2000);
      digitalWrite(PIN_RELE1, HIGH);
      digitalWrite(PIN_RELE2, HIGH);
      while (millis() - tiempo2Start < tiempo2 * 1000) {
        // Esperar el tiempo correspondiente
      }

      // Actualizar el tiempo transcurrido
      elapsedTime = millis() - startTime;
    }

    // Apagar ambos LEDs al finalizar el ciclo
    digitalWrite(PIN_RELE1, HIGH);
    digitalWrite(PIN_RELE2, HIGH);

    relesEncendidos = false;  // Desactivar el bucle para que no se reinicie
  }
}
