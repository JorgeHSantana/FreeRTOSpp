#include "./this_task.hpp"

using namespace freertos;

void this_task::sleep(uint32_t time_ms) {
    vTaskDelay(pdMS_TO_TICKS(time_ms));
}

void this_task::suspend(void) {
    vTaskSuspend(nullptr);
}
