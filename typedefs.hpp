#pragma once

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/task.h"
#include "FreeRTOS/queue.h"
#include "FreeRTOS/semphr.h"
#include "FreeRTOS/timers.h"

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
    }
}