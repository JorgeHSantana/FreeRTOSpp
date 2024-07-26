#include "semaphore_base.h"

using namespace freertos;

semaphore_base::~semaphore_base(void) {
    vSemaphoreDelete(this->handle);
    this->handle = nullptr;
}

bool semaphore_base::is_valid(void) const {
    return this->handle != nullptr;
}

bool semaphore_base::give(void) {
    if (this->handle == nullptr){
        return false;
    }

    return xSemaphoreGive(this->handle) == pdPASS;
}

bool semaphore_base::take(uint32_t timeout_ms) {
    if (this->handle == nullptr){
        return false;
    }

    uint32_t ticks = 1;
    if (timeout_ms != max_delay_ms){
        ticks = pdMS_TO_TICKS(timeout_ms);
    }

    return xSemaphoreTake(this->handle, ticks) == pdTRUE;
}

bool semaphore_base::give_from_isr(void) {
    if (this->handle == nullptr){
        return false;
    }

    return xSemaphoreGiveFromISR(this->handle, nullptr) == pdTRUE;
}

bool semaphore_base::take_from_isr(void) {
    if (this->handle == nullptr){
        return false;
    }

    return xSemaphoreTakeFromISR(this->handle, nullptr) == pdTRUE;
}