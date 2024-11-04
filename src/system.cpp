#include "system.hpp"

#include "freertos/task.h"

using namespace freertos;
using namespace typedefs;

tick_type system::get_tick_count(void) {
    return xTaskGetTickCount();
}

tick_type system::get_tick_count_from_isr(void){
    return xTaskGetTickCountFromISR();
}

uint64_t system::get_milliseconds(void){
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

uint64_t system::get_milliseconds_from_isr(void){
    return pdTICKS_TO_MS(xTaskGetTickCountFromISR());
}

u_base_type system::get_amount_of_tasks(void){
    return uxTaskGetNumberOfTasks();
}