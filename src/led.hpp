#pragma once

#include "config.hpp"
#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include <mutex> // std::mutex

std::mutex ledMtx; // mutex for critical section

ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM];
volatile bool rgbInUse = false;
struct RGB
{
    int R;
    int G;
    int B;
};

struct RGBTaskParams
{
    RGB rgb1;
    RGB rgb2;
    int times;

    RGBTaskParams() = default;
    RGBTaskParams(const RGBTaskParams &other)
    {
        rgb1 = other.rgb1;
        rgb2 = other.rgb2;
        times = other.times;
    }
};

void rgbInit()
{
    int ch;

    /*
     * Prepare and set configuration of timers
     * that will be used by LED Controller
     */
    ledc_timer_config_t ledc_timer;

    ledc_timer.duty_resolution = LEDC_TIMER_8_BIT; // resolution of PWM duty
    ledc_timer.freq_hz = 5000;                     // frequency of PWM signal
    ledc_timer.speed_mode = LEDC_LS_MODE;          // timer mode
    ledc_timer.timer_num = LEDC_LS_TIMER;          // timer index

    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    // Prepare and set configuration of timer1 for low speed channels
    ledc_timer.speed_mode = LEDC_HS_MODE;
    ledc_timer.timer_num = LEDC_HS_TIMER;
    ledc_timer_config(&ledc_timer);

    /*
     * Prepare individual configuration
     * for each channel of LED Controller
     * by selecting:
     * - controller's channel number
     * - output duty cycle, set initially to 0
     * - GPIO number where LED is connected to
     * - speed mode, either high or low
     * - timer servicing selected channel
     *   Note: if different channels use one timer,
     *         then frequency and bit_num of these channels
     *         will be the same
     */

    ledc_channel[0].channel = LEDC_0_CHANNEL,
    ledc_channel[0].duty = 0;
    ledc_channel[0].gpio_num = LEDC_0;
    ledc_channel[0].speed_mode = LEDC_HS_MODE;
    ledc_channel[0].hpoint = 0;
    ledc_channel[0].timer_sel = LEDC_HS_TIMER;

    ledc_channel[1].channel = LEDC_1_CHANNEL;
    ledc_channel[1].duty = 0;
    ledc_channel[1].gpio_num = LEDC_1;
    ledc_channel[1].speed_mode = LEDC_HS_MODE;
    ledc_channel[1].hpoint = 0;
    ledc_channel[1].timer_sel = LEDC_HS_TIMER;

    ledc_channel[2].channel = LEDC_2_CHANNEL;
    ledc_channel[2].duty = 0;
    ledc_channel[2].gpio_num = LEDC_2;
    ledc_channel[2].speed_mode = LEDC_HS_MODE;
    ledc_channel[2].hpoint = 0;
    ledc_channel[2].timer_sel = LEDC_HS_TIMER;

    // ledc_channel[2].channel = LEDC_LS_CH2_CHANNEL;
    // ledc_channel[2].duty = 0;
    // ledc_channel[2].gpio_num = LEDC_LS_CH2_GPIO;
    // ledc_channel[2].speed_mode = LEDC_LS_MODE;
    // ledc_channel[2].hpoint = 0;
    // ledc_channel[2].timer_sel = LEDC_LS_TIMER;

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < 3; ch++)
    {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);
}

void setRGB(int r, int g, int b)
{

    // printf("setting r[%d] g[%d] b[%d] \n", r, g, b);
    // ledc_set_fade_with_time(ledc_channel[LEDC_R_CHANNEL].speed_mode,
    //                         ledc_channel[LEDC_R_CHANNEL].channel, r, LEDC_TEST_FADE_TIME);

    // ledc_set_fade_with_time(ledc_channel[LEDC_G_CHANNEL].speed_mode,
    //                         ledc_channel[LEDC_G_CHANNEL].channel, g, LEDC_TEST_FADE_TIME);

    // ledc_set_fade_with_time(ledc_channel[LEDC_B_CHANNEL].speed_mode,
    //                         ledc_channel[LEDC_B_CHANNEL].channel, b, LEDC_TEST_FADE_TIME);

    // for (int ch = 0; ch < 3; ch++)
    // {
    //     ledc_fade_start(ledc_channel[ch].speed_mode,
    //                     ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    // }

    ledc_set_duty(ledc_channel[LEDC_0_CHANNEL].speed_mode, ledc_channel[LEDC_0_CHANNEL].channel, r);
    ledc_update_duty(ledc_channel[LEDC_0_CHANNEL].speed_mode, ledc_channel[LEDC_0_CHANNEL].channel);

    ledc_set_duty(ledc_channel[LEDC_1_CHANNEL].speed_mode, ledc_channel[LEDC_1_CHANNEL].channel, g);
    ledc_update_duty(ledc_channel[LEDC_1_CHANNEL].speed_mode, ledc_channel[LEDC_1_CHANNEL].channel);

    ledc_set_duty(ledc_channel[LEDC_2_CHANNEL].speed_mode, ledc_channel[LEDC_2_CHANNEL].channel, b);
    ledc_update_duty(ledc_channel[LEDC_2_CHANNEL].speed_mode, ledc_channel[LEDC_2_CHANNEL].channel);
    // printf("2. LEDC fade down to duty = 0\n");
    // for (ch = 0; ch < 2; ch
    // ++)
    // {
    //      printf("1. LEDC fade up to ch = %d\n", ch);
    //     ledc_set_fade_with_time(ledc_channel[ch].speed_mode,
    //                             ledc_channel[ch].channel, 0, LEDC_TEST_FADE_TIME);
    //     ledc_fade_start(ledc_channel[ch].speed_mode,
    //                     ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    //                     vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
    // }

    // printf("3. LEDC set duty = %d without fade\n", LEDC_TEST_DUTY);
    // for (ch = 0; ch < 2; ch++)
    // {
    //      printf("1. LEDC fade up to ch = %d\n", ch);
    //     ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_TEST_DUTY);
    //     ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
    //     vTaskDelay(LEDC_TEST_FADE_TIME / portTICK_PERIOD_MS);
    // }

    // printf("4. LEDC set duty = 0 without fade\n");
    // for (int ch = 0; ch < 3; ch++)
    // {
    //     printf("1. LEDC turn off ch = %d\n", ch);
    //     ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
    //     ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);

    // }
}

volatile int flashCmd=0;
void flashRGB(RGB rgb1, RGB rgb2, int times)
{
    flashCmd++;

    int thisCmd = flashCmd;
    vTaskDelay(250 /portTICK_RATE_MS); //give others time to exit

    // if (rgbInUse == true)
    // {
    //     return;
    // }
    // rgbInUse = true;
    Serial.println("RGB in use:" + String(rgbInUse));
    Serial.println("Flashing RGB");

    for (int i = 0; i < times; i++)
    {
        // Serial.println("Flash:" + String(i));
        //Serial.printf("r[%d] g[%d] b[%d] \n", rgb1.R, rgb1.G, rgb1.B);
        setRGB(rgb1.R, rgb1.G, rgb1.B);
        //delay(100);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        //Serial.printf("r[%d] g[%d] b[%d] \n", rgb2.R, rgb2.G, rgb2.B);
        setRGB(rgb2.R, rgb2.G, rgb2.B);
        //delay(100);
        vTaskDelay(100 / portTICK_PERIOD_MS);

        if(thisCmd != flashCmd)
        {
           break;
        }
    }
    setRGB(0, 0, 0);
    // rgbInUse = false;
}

void flashRGBTask(void *pvParameters)
{
    RGBTaskParams *params = (RGBTaskParams *)pvParameters;

    // Serial.println("flashRGBTask1");
    flashRGB(params->rgb1, params->rgb2, params->times);
    // Serial.println("flashRGBTask2");
    delete params;
    vTaskDelete(NULL);
}

void flashRGBDontWait(RGB rgb1, RGB rgb2, int times)
{
    TaskHandle_t xHandle = NULL;
    RGBTaskParams *params = new RGBTaskParams();
    params->rgb1 = rgb1;
    params->rgb2 = rgb2;
    params->times = times;

    xTaskCreate(flashRGBTask, "flashRGBTask", 5000, params, 1, &xHandle);
}