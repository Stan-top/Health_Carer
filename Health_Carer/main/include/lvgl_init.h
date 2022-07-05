#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "lvgl.h"
#include "freertos/semphr.h"
#include "esp_system.h"


/* Littlevgl specific */
#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif


#include "lvgl_helpers.h"
#include "lvgl/src/hal/lv_hal_disp.h"


#define LV_TICK_PERIOD_MS 1
#define BUF_W 20
#define BUF_H 10


void lv_tick_task(void *arg);
void guiTask1(void *pvParameter);
void Show_State();