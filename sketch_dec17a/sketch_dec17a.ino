#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // szerokość wyświetlacza OLED
#define SCREEN_HEIGHT 32 // wysokość wyświetlacza OLED
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Adres I2C 0x3C dla wyświetlacza 128x32
    for(;;); // Włącz pętlę nieskończoną, jeśli inicjalizacja wyświetlacza nie powiedzie się
  }
  display.clearDisplay();
  
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
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Odleglosc:");
    display.print(distance);
    display.println(" cm");
    display.display();
    
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
