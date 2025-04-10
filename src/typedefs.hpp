#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"

namespace freertos {
    namespace typedefs {
        using task_handle = TaskHandle_t;
        using queue_handle = QueueHandle_t;
        using semaphore_handle = SemaphoreHandle_t;
        using task_struct = StaticTask_t;
        using queue_struct = StaticQueue_t;
        using semaphore_struct = StaticSemaphore_t;
        using task_stack = StackType_t;
        using timer_handle = TimerHandle_t;
        using timer_struct = StaticTimer_t;
        using u_base_type = UBaseType_t;
        using base_type = BaseType_t;
        using tick_type = TickType_t;
    }
}