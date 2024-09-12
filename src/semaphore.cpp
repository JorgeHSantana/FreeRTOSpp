#include "semaphore.hpp"

using namespace freertos;
using namespace abstract;

semaphore::semaphore(type semaphore_type) :
semaphore_type(semaphore_type)
{}

semaphore::~semaphore(void) {
    this->remove();
}

bool semaphore::remove(void) {
    if (this->handle == nullptr) {
        return false;
    }

    vSemaphoreDelete(this->handle);
    this->handle = nullptr;
    this->semaphore_type = type::none;
    return true;
}

bool semaphore::is_valid(void) const {
    return (this->handle != nullptr && this->semaphore_type != type::none);
}

bool semaphore::give(void) {
    if (!this->is_valid()){
        return false;
    }

    if (this->semaphore_type == type::recursive){
        return xSemaphoreGiveRecursive(handle) == pdPASS;
    }
    return xSemaphoreGive(this->handle) == pdPASS;
}

bool semaphore::take(uint32_t timeout_ms) {
    if (!this->is_valid()){
        return false;
    }

    uint32_t ticks = 1;
    if (timeout_ms != max_delay_ms){
        ticks = pdMS_TO_TICKS(timeout_ms);
    }

    if (this->semaphore_type == type::recursive){
        return xSemaphoreTakeRecursive(handle, ticks) == pdPASS;
    }
    return xSemaphoreTake(this->handle, ticks) == pdTRUE;
}

bool semaphore::give_from_isr(void) {
    if (!this->is_valid() || this->semaphore_type == type::recursive){
        return false;
    }

    return xSemaphoreGiveFromISR(this->handle, nullptr) == pdTRUE;
}

bool semaphore::take_from_isr(void) {
    if (!this->is_valid() || this->semaphore_type == type::recursive){
        return false;
    }

    return xSemaphoreTakeFromISR(this->handle, nullptr) == pdTRUE;
}