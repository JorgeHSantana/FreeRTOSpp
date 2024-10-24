#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/FreeRTOSConfig.h"
#include "FreeRTOS/task.h"

namespace freertos {
    
    namespace constants {
        constexpr uint32_t max_delay_ms = portMAX_DELAY;
        constexpr uint8_t task_name_length = configMAX_TASK_NAME_LEN;
        constexpr uint8_t max_priority = configMAX_PRIORITIES - 1;
        constexpr uint8_t max_notifications = configTASK_NOTIFICATION_ARRAY_ENTRIES - 1;
    }
    
}