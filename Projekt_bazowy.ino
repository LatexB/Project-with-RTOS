#include <Arduino_FreeRTOS.h>
#include <task.h>

#define TRIGGER_PIN  6
#define ECHO_PIN     7
#define LED_PIN_1    2
#define LED_PIN_2    3
#define LED_PIN_3    4
#define LED_PIN_4    5

void setup() {
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(LED_PIN_2, OUTPUT);
  pinMode(LED_PIN_3, OUTPUT);
  pinMode(LED_PIN_4, OUTPUT);
  xTaskCreate(
    TaskMeasureDistance
    ,  "Distance"
    ,  128
    ,  NULL
    ,  2
    ,  NULL );
}

void loop() {
  // Nie rób nic w pętli głównej.
}

void TaskMeasureDistance(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    long distance = duration / 29 / 2;
    if (distance <20) {
      digitalWrite(LED_PIN_1, HIGH);
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_3, LOW);
      digitalWrite(LED_PIN_4, LOW);
    } else if (distance < 50) {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, HIGH);
      digitalWrite(LED_PIN_3, LOW);
      digitalWrite(LED_PIN_4, LOW);
    } else if (distance < 70) {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_3, HIGH);
      digitalWrite(LED_PIN_4, LOW);
    } else if (distance < 100) {
      digitalWrite(LED_PIN_1, LOW);
      digitalWrite(LED_PIN_2, LOW);
      digitalWrite(LED_PIN_3, LOW);
      digitalWrite(LED_PIN_4, HIGH);
    }
    vTaskDelay(1 / portTICK_PERIOD_MS);
  }
}