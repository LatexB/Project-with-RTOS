#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
void Task1( void *pvParameters );
void Task2( void *pvParameters );

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

long distance = 0;

TaskHandle_t TaskHandle_1; // handler for Task1
TaskHandle_t TaskHandle_2; // handler for Task2


void setup() 
{
  Serial.begin(9600); // Enable serial communication library.
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

   xTaskCreate(Task1, "LED1", 100, NULL, 3, &TaskHandle_1);
   xTaskCreate(Task2, "LED2", 100, NULL, 2, &TaskHandle_2);
   vTaskStartScheduler();

   
}

void loop() 
{
  // put your main code here, to run repeatedly:

}

//definition of Task1
void Task1(void* pvParameters)
{
     UBaseType_t uxPriority = uxTaskPriorityGet( NULL );
    while(1)
    {
    Serial.println("Task1 is running and about to raise Task2 Priority");
    vTaskPrioritySet( TaskHandle_2, ( uxPriority + 1 ) );
       digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    long duration = pulseIn(ECHO_PIN, HIGH);
    distance = duration / 29 / 2;
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Odleglosc:");
    display.print(distance);
    display.println(" cm");
    display.display();
    
    Serial.print("Odleglosc: "); // Wyświetl odległość w porcie szeregowym
    Serial.print(distance);
    Serial.println(" cm");
    }
}
void Task2(void* pvParameters)
{
 UBaseType_t   uxPriority = uxTaskPriorityGet( NULL );
     while(1)
    {
          //if (xSemaphoreTake(xEventSemaphore, portMAX_DELAY) == pdTRUE) { // Czekaj na zdarzenie
      if (distance < 20) {
        digitalWrite(LED_PIN_1, HIGH);
      } else {
        digitalWrite(LED_PIN_1, LOW);
      }
    Serial.println("Task2 is running and about to lower Task2 Priority");
    vTaskPrioritySet( TaskHandle_2, ( uxPriority - 2 ) );
    
    }
  
}