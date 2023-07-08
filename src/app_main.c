#include "FreeRTOS.h"
#include "task.h"

#include "app_log.h"

#include "app_task.h"

#include "util_data.h"

static const char* TAG = "app_main";

/**
 * @brief Application main
 * 
 */
void app_main(){
    LOGI(TAG, "on app_main");
    LOGV(TAG, "FreeRTOS Started, Starting Application...");

    // 拉起所有Application
    /* 这里修改函数名可以指定要运行的Task, GCC开了O3优化, 会自动剔除没有使用的函数*/
    app_task_main();

    // Application保活
    vTaskDelete(NULL);
}