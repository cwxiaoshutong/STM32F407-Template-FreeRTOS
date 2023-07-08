#include "FreeRTOS.h"
#include "task.h"

#include "app_log.h"

#include "util_data.h"

static const char* TAG = "app_task";

void task1(){
    LOGI(TAG, "on task");

    while(1){
          // LOGW(TAG, "on task loop");
        // LOGE(TAG, "on task loop");
        // LOGV(TAG, "on task loop");      // LOGI(TAG, "on task loop");
        LOGD(TAG, "on task1 loop");
        // LOGW(TAG, "on task loop");
        // LOGE(TAG, "on task loop");
        // LOGV(TAG, "on task loop");
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

void task2(){
    LOGI(TAG, "on task");

    while(1){
          // LOGW(TAG, "on task loop");
        // LOGE(TAG, "on task loop");
        // LOGV(TAG, "on task loop");      // LOGI(TAG, "on task loop");
        LOGD(TAG, "on task2 loop");
        // LOGW(TAG, "on task loop");
        // LOGE(TAG, "on task loop");
        // LOGV(TAG, "on task loop");
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}

void app_task_main(){
    LOGI(TAG, "on app_task_main");

    xTaskCreate(task1, "task1", 1024, NULL, 5, NULL);
    // xTaskCreate(task1, "task2", 1024, NULL, 5, NULL);
    // xTaskCreate(task1, "task3", 1024, NULL, 5, NULL);
    // xTaskCreate(task2, "task4", 1024, NULL, 5, NULL);
    // xTaskCreate(task2, "task5", 1024, NULL, 5, NULL);
}
