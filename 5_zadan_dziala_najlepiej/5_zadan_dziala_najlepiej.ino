#include <Arduino_FreeRTOS.h>
#include <LiquidCrystal.h>

#define TRIGGER_PIN 6 // Trigger pin of Ultrasonic sensor
#define ECHO_PIN 7 // Echo pin of Ultrasonic sensor

// Initialize the LCD connected to pins 8, 9, 10, 11, 12, 13
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

long distance = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("In Setup function"));
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  xTaskCreate(MeasureDistanceTask, "MeasureDistance", 100, NULL, 1, NULL);
  xTaskCreate(MyTask1, "Task1", 100, NULL, 0, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 0, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 0, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
}

void loop()
{
}

long measureDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  vTaskDelay(5/portTICK_PERIOD_MS);
  digitalWrite(TRIGGER_PIN, HIGH);
  vTaskDelay(5/portTICK_PERIOD_MS);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = (duration/58);
  return distance;
}

static void MeasureDistanceTask(void* pvParameters)
{
  while(1)
  { 
    distance = measureDistance();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.print(distance);
    lcd.print(" cm");
    vTaskDelay(400/portTICK_PERIOD_MS);
  }
}

static void MyTask1(void* pvParameters)
{
  while(1)
  { 
    if (distance < 10) {
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW); 
      digitalWrite(4,LOW);
      digitalWrite(5,LOW); 
      Serial.println(F("Task1"));
    }
    vTaskDelay(400/portTICK_PERIOD_MS);
  }
}

static void MyTask2(void* pvParameters)
{  
  while(1)
  { 
    if (distance >= 10 && distance < 20) {
      digitalWrite(2,LOW);
      digitalWrite(3,HIGH); 
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);   
      Serial.println(F("Task2"));
    }
    vTaskDelay(400/portTICK_PERIOD_MS);
  }
}

static void MyTask3(void* pvParameters)
{ 
  while(1)
  { 
    if (distance >= 20 && distance < 30) {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW); 
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      Serial.println(F("Task3"));
    }
    vTaskDelay(400/portTICK_PERIOD_MS);
  }
}

static void MyIdleTask(void* pvParameters)
{
  while(1)
  { 
    if (distance >= 30) {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW); 
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      Serial.println(F("Idle state"));
    }
    vTaskDelay(400/portTICK_PERIOD_MS);
  }  
}
