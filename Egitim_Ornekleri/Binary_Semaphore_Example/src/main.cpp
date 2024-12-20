#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// pin tanimlamalari
#define LED_PIN 2
#define BUTTON_PIN 0

// binary semaphore tanimlamasi
SemaphoreHandle_t xBinarySemaphore;

// Rutin kesme fonksiyonu

void IRAM_ATTR buttonISR(){
  // Semaphore'u tetikle
  xSemaphoreGiveFromISR(xBinarySemaphore, NULL);
}

// LED Kontrol Görevi
void LEDTask(void *pvParameters){
  while(1){
    // Semaphore'u al
    if(xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdTRUE){
      // LED'i yak
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED Yandi!!!!!");
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 saniye bekletiyorum
      // LED'i sondur
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED Sondu!!!!!");
    }
    }
  }

void setup() {
  
    // Seri portu baslat
  Serial.begin(115200);
  
  // LED ve Buton Pinlerini Ayarla
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Binary Semaphore'u olustur
  xBinarySemaphore = xSemaphoreCreateBinary();
  if(xBinarySemaphore == NULL){
    Serial.println("Binary Semaphore Olusturulamadi");
  while(1);
  }

  // Kesme fonksiyonunu tetikle
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);

  // LED Task'ini olustur
  xTaskCreate(
    LEDTask,
    "LED Task", 
    1000, 
    NULL,
    1,
    NULL
  );

}


void loop() {
  // Bos void loop fonksiyonu ;
}