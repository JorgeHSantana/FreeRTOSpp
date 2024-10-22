#include "./this_task.hpp"

using namespace freertos;
using namespace typedefs;

void this_task::sleep(uint32_t time_ms) {
    vTaskDelay(pdMS_TO_TICKS(time_ms));
}

void this_task::suspend(void) {
    vTaskSuspend(nullptr);
}

bool this_task::get_notification(uint32_t& notification_value, u_base_type index, uint32_t timeout_ms) {
    if (index > constants::max_notifications){
        return false;
    }
    
    return xTaskNotifyWaitIndexed(index, 0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(timeout_ms)) == pdPASS;
}

uint32_t this_task::get_free_stack_memory(void) {
    return uxTaskGetStackHighWaterMark(nullptr) * sizeof(task_stack);
}