#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

namespace freertos {
    
    namespace constants {
        constexpr uint32_t max_delay_ms = portMAX_DELAY;
        constexpr uint8_t task_name_length = configMAX_TASK_NAME_LEN;
        constexpr uint8_t max_priority = configMAX_PRIORITIES - 1;
        
        #if defined(configTASK_NOTIFICATION_ARRAY_ENTRIES)
            constexpr uint8_t max_notifications = configTASK_NOTIFICATION_ARRAY_ENTRIES - 1;
        #else
            constexpr uint8_t max_notifications = 0;
        #endif
        
        constexpr uint32_t min_stack_size = configMINIMAL_STACK_SIZE;
    }
    
}