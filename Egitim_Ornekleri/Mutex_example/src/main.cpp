#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"


// Mutex tanimliyorum
SemaphoreHandle_t xMutex;

// Gorev prototiplerini belirtiyorum
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {

  // Serial portu baslatiyorum
  Serial.begin(115200);
  while (!Serial);

  // Mutex olusturuyorum
  xMutex = xSemaphoreCreateMutex();

  // Mutex olusturulamadiysa programi durduruyorum
  if(xMutex == NULL){
    Serial.println("Mutex olusturulamadi");
    while(1);
  }

  // Gorevleri olusturuyorum  
  xTaskCreate(vTask1 // Gorev1
              , "Task1" // Gorev adi
              , 1000 // Stack boyutu
              , NULL // Gorev parametresi
              , 1 // Gorev onceligi
              , NULL); // Gorev handle'i

  xTaskCreate(vTask2 // Gorev2
              , "Task2"// Gorev adi
              , 1000 // Stack boyutu
              , NULL // Gorev parametresi
              , 1 // Gorev onceligi
              , NULL); // Gorev handle'i
}

void loop() {
  // Gorevler FreeRTOS tarafindan yonetildigi icin loop fonksiyonunda bir sey yapmaya gerek duymuyorum
}


void vTask1(void *pvParameters){
  while(1){
   // Mutex al
   if(xSemaphoreTake(xMutex, (TickType_t)1000) == pdTRUE){
      // Semaphore'un ikinci parametre beklemesi gereken sureyi belirtir. 10ms bekleyecek 
      // portMaxDelay kullanilabilir oda mutex serbest birakilana kadar bekler
      Serial.println("Görev 1: Mutex alindi. Veriler işleniyor...");
      delay(2000);
      Serial.println("Görev 1: İşlem tamamlandi. Mutex birakiliyor.");
      xSemaphoreGive(xMutex); // Mutex birak
   }
   else{
      // Mutex alinamadiysa
      Serial.println("Görev 1: Mutex alinamadi. Başka bir işlem yapiliyor.");
   }
   vTaskDelay(1000 / portTICK_PERIOD_MS); // 0.5 saniye bekle
  }
}

void vTask2(void *pvParameters) {
  while (1) {
    // Mutex al
    if (xSemaphoreTake(xMutex, (TickType_t)1000) == pdTRUE) {
      // UART'a güvenli şekilde yaz
      Serial.println("Görev 2: Mutex alindi. Veriler işleniyor...");
      delay(2000);
      Serial.println("Görev 2: İşlem tamamlandi. Mutex birakiliyor.");
      xSemaphoreGive(xMutex); // Mutex birak
    } else {
      // Mutex alinamadi
      Serial.println("Görev 2: Mutex alinamadi.");
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}