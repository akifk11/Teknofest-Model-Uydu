#include <Arduino.h>

// LED Pinlerini const olarak tanımlayın
const int LED1_PIN = 2;
const int LED2_PIN = 4;
const int LED3_PIN = 5;

SemaphoreHandle_t xSemaphore;

// LED görevi 
void vLEDToggleTask(void *pvParameters) {
  int ledPin = *(int *)pvParameters;

  while (1) {
    // Semaphore almayi dene

    if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      digitalWrite(ledPin, HIGH); // LED'i yak
      Serial.printf("LED %d yandi \n", ledPin);
      vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 saniye bekle
      digitalWrite(ledPin, LOW); // LED'i sondur
      Serial.printf("LED %d sonduruldu \n", ledPin);

      // Semaphore'u geri ver
      xSemaphoreGive(xSemaphore);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 saniye bekle Diger goverler icin bekle
    }
  }
}


void setup() {
  // Seri haberlesme
  Serial.begin(115200);

  // LED pinlerini cikis yap
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  // Counting Semaphore'u olustur(2 kaynak aynı anda kullanabilir)
  // Kaynak yonetimi burada yapilir
  xSemaphore = xSemaphoreCreateCounting(2, 2); // kaynak sayisi 2, baslangic degeri 2 


  // Gorevleri olusturuyorum
  xTaskCreate(vLEDToggleTask, "LED1 Task", 1000, (void *)&LED1_PIN, 1, NULL);
  xTaskCreate(vLEDToggleTask, "LED2 Task", 1000, (void *)&LED2_PIN, 1, NULL);
  xTaskCreate(vLEDToggleTask, "LED3 Task", 1000, (void *)&LED3_PIN, 1, NULL);
}

void loop() {
  // loop fonksiyonu bos
}