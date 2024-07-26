#include "task.hpp"

using namespace freertos;
using namespace constants;
using namespace typedefs;
using namespace abstract;

task::task(bool auto_start){
    this->auto_start = auto_start;
}

task::~task(void) {
    if (!this->is_valid()) {
        return;
    }

    vTaskDelete(this->handle);
    this->handle = nullptr;
}

bool task::resume(void) {
    if (!this->is_valid()) {
        return false;
    }
    
    vTaskResume(this->handle);
    return true;
}

bool task::resume_from_isr(void) {
    if (!this->is_valid()) {
        return false;
    }
    
    xTaskResumeFromISR(this->handle);
    return true;
}

bool task::suspend(void) {
    if (!this->is_valid()) {
        return false;
    }

    vTaskSuspend(this->handle);
    return true;
}

bool task::suspend_from_isr(void) {
    return this->suspend();
}

bool task::is_running(void) {
    return this->is_valid() && eTaskGetState(this->handle) != eSuspended && eTaskGetState(this->handle) != eDeleted;
}

bool task::join(uint32_t observer_delay_ms, uint32_t timeout_ms) {
    if (this->handle == nullptr) {
        return false;
    }

    return this_task::sleep_for<task&>(*this, [](task& self) -> bool {
        return !self.is_running();
    }, observer_delay_ms, timeout_ms);
}

bool task::is_valid(void) {
    return this->handle != nullptr;
}