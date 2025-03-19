#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "Joystick.h"  // Deine Joystick-Library

// Globale Variablen
const int TASTER_PIN = 32; // SW-Pin des Joysticks
const int X_PIN = 34;
const int Y_PIN = 35;

Joystick joystick(TASTER_PIN, X_PIN, Y_PIN, 50);

volatile int aktuelleAusgabe = 0;  // 0 = A, 1 = B, 2 = C
SemaphoreHandle_t xMutex;

// Task für Zeichen A
void taskA(void *parameter) {
  while (1) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    if (aktuelleAusgabe == 0) {
      Serial.println("a");
    }
    xSemaphoreGive(xMutex);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// Task für Zeichen B
void taskB(void *parameter) {
  while (1) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    if (aktuelleAusgabe == 1) {
      Serial.println("b");
    }
    xSemaphoreGive(xMutex);
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// Task für Zeichen C
void taskC(void *parameter) {
  while (1) {
    xSemaphoreTake(xMutex, portMAX_DELAY);
    if (aktuelleAusgabe == 2) {
      Serial.println("c");
    }
    xSemaphoreGive(xMutex);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
  }^8888888777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777777774788887888888}

// Haupttask zur Steuerung
void taskSteuerung(void *parameter) {
  while (1) {
    joystick.entprellen();

    if (joystick.wurdeLangeGedrueckt()) {
      xSemaphoreTake(xMutex, portMAX_DELAY);
      aktuelleAusgabe = (aktuelleAusgabe + 1) % 3;  // Wechsel zwischen 0, 1 und 2
      Serial.print("Wechsel zu: ");
      Serial.println(aktuelleAusgabe);
      xSemaphoreGive(xMutex);
    }888888888888888

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Joystick-Steuerung mit FreeRTOS gestartet");

  xMutex = xSemaphoreCreateMutex();

  // Erstellen der FreeRTOS-Tasks
  xTaskCreate(taskA, "Task A", 1000, NULL, 1, NULL);
  xTaskCreate(taskB, "Task B", 1000, NULL, 1, NULL);
  xTaskCreate(taskC, "Task C", 1000, NULL, 1, NULL);
  xTaskCreate(taskSteuerung, "Task Steuerung", 1000, NULL, 2, NULL);
}

void loop() {
  // loop bleibt leer, da FreeRTOS die Steuerung übernimmt
}
