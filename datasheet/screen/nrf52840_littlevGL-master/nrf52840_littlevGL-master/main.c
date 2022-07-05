/**
 * Copyright (c) 2017 - 2018, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "nrfx_spim.h"
#include "app_util_platform.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include <string.h>
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "FT6336.h"
#include "st7789v.h"
#include "lvgl.h"

#include "app_timer.h"

APP_TIMER_DEF(id_lvgl_tick); 
static void lvgl_tick_handler(void * p_context)
{
 lv_tick_inc(10);
}
lv_obj_t * label1 ;
 
int main(void)
{


  APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
  NRF_LOG_DEFAULT_BACKENDS_INIT();

  lv_init();

//  ret_code_t err_code = app_timer_init();
//  APP_ERROR_CHECK(err_code);
//  err_code = app_timer_create (&id_lvgl_tick, APP_TIMER_MODE_REPEATED, lvgl_tick_handler);
//  APP_ERROR_CHECK(err_code);
//  err_code = app_timer_start(id_lvgl_tick, APP_TIMER_TICKS(10), NULL);
//  APP_ERROR_CHECK(err_code);

  lv_theme_t *th = lv_theme_alien_init(30, NULL);
  lv_theme_set_current(th);

  
  st7789_init();
  lv_disp_drv_t disp;
  lv_disp_drv_init(&disp);
  disp.disp_flush = st7789_flush;
  lv_disp_drv_register(&disp);

  FT6336_Init();
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.read = FT6336_read;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  lv_indev_drv_register(&indev_drv);


//  lv_obj_t *calendar = lv_calendar_create(lv_scr_act(),NULL);
//  lv_obj_set_size(calendar,240,240);


  lv_obj_t *scr = lv_obj_create(NULL, NULL);	
  lv_scr_load(scr);  
  label1 =  lv_label_create(scr , NULL);

  lv_obj_t * btn =  lv_btn_create(scr , NULL);
  lv_obj_set_size(btn,100,100);
  lv_btn_set_ink_in_time(btn,100);
  lv_btn_set_ink_out_time(btn,100);
  lv_obj_align(btn,scr,LV_ALIGN_CENTER,0,0);
  char buffer[20];
 

  while(1) 
  {
  
          nrf_delay_ms(10);
          lv_tick_inc(10);
          lv_task_handler();
          sprintf(buffer,"x:%d  y:%d",240-TPR_Structure.x[0],240-TPR_Structure.y[0]);
          lv_label_set_text(label1,buffer);
		
         
          
  }
}
