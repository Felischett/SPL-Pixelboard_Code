// FreeRTOS-Tasks für ESP32: Serieller Monitor Test

void taskA(void *parameter) {
  while (1) {
    Serial.println("a");
    delay(1000);  // 1 Sekunde warten
  }
}

void taskB(void *parameter) {
  while (1) {
    Serial.println("b");
    delay(2000);  // 2 Sekunden warten
  }
}

void taskC(void *parameter) {
  while (1) {
    Serial.println("c");
    delay(3000);  // 3 Sekunden warten
  }
}

void setup() {
  Serial.begin(115200);  // Serielle Kommunikation starten

  // Erstellen der FreeRTOS-Tasks
  xTaskCreate(taskA, "Task A", 1000, NULL, 1, NULL);
  xTaskCreate(taskB, "Task B", 1000, NULL, 1, NULL);
  xTaskCreate(taskC, "Task C", 1000, NULL, 1, NULL);
}

void loop() {
  // Der loop bleibt leer, da FreeRTOS die Tasks verwaltet
}
