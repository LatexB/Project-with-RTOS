#include <Arduino_FreeRTOS.h>
#define TRIGGER_PIN 6 // Trigger pin of Ultrasonic sensor
#define ECHO_PIN 7 // Echo pin of Ultrasonic sensor

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

  xTaskCreate(MyTask1, "Task1", 100, NULL, 1, NULL);
  xTaskCreate(MyTask2, "Task2", 100, NULL, 2, NULL);
  xTaskCreate(MyTask3, "Task3", 100, NULL, 3, NULL);
  xTaskCreate(MyIdleTask, "IdleTask", 100, NULL, 0, NULL);
}

void loop()
{
}

long measureDistance() {
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = (duration/2) / 29.1;
  return distance;
}

static void MyTask1(void* pvParameters)
{
  while(1)
  { 
    long distance = measureDistance();
    if (distance < 10) {
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW); 
      digitalWrite(4,LOW);
      digitalWrite(5,LOW); 
      Serial.println(F("Task1"));
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

static void MyTask2(void* pvParameters)
{  
  while(1)
  { 
    long distance = measureDistance();
    if (distance >= 10 && distance < 20) {
      digitalWrite(2,LOW);
      digitalWrite(3,HIGH); 
      digitalWrite(4,LOW);
      digitalWrite(5,LOW);   
      Serial.println(F("Task2"));
    }
    vTaskDelay(110/portTICK_PERIOD_MS);
  }
}

static void MyTask3(void* pvParameters)
{ 
  while(1)
  { 
    long distance = measureDistance();
    if (distance >= 20 && distance < 30) {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW); 
      digitalWrite(4,HIGH);
      digitalWrite(5,LOW);
      Serial.println(F("Task3"));
    }
    vTaskDelay(120/portTICK_PERIOD_MS);
  }
}

static void MyIdleTask(void* pvParameters)
{
  while(1)
  { 
    long distance = measureDistance();
    if (distance >= 30) {
      digitalWrite(2,LOW);
      digitalWrite(3,LOW); 
      digitalWrite(4,LOW);
      digitalWrite(5,HIGH);
      Serial.println(F("Idle state"));
    }
    delay(50);
  }  
}
