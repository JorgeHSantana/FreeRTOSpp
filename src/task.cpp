#include "task.hpp"

using namespace freertos;
using namespace constants;
using namespace typedefs;
using namespace abstract;

task::task(bool auto_start, uint32_t stack_size) : auto_start(auto_start), stack_size(stack_size) {}

task::~task(void) {
    if (!this->is_valid()) {
        return;
    }

    vTaskDelete(this->handle);
    this->handle = nullptr;
    this->stack_size = 0;
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

uint32_t task::get_stack_memory_size(void) {
    return this->stack_size;
}

uint32_t task::get_free_stack_memory(void) {
    if (!this->is_valid()) {
        return 0;
    }

    return uxTaskGetStackHighWaterMark(this->handle) * sizeof(task_stack);
}

uint32_t task::get_used_stack_memory(void) {
    if (!this->is_valid()) {
        return 0;
    }

    return this->get_stack_memory_size() - this->get_free_stack_memory();
}

task::notifier task::notify(void) {
    static task::notifier buffer(this->handle);
    return buffer;
}

task::notifier::notifier(task_handle handle) : handle(handle) {}

bool task::notifier::signal(u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    uint32_t dummy = 0;

    if (xTaskNotifyAndQueryIndexed(this->handle, index, dummy, eNoAction, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::signal_from_isr(u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    uint32_t dummy = 0;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, dummy, eNoAction, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::set_bits(uint32_t bits, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    if (xTaskNotifyAndQueryIndexed(this->handle, index, bits, eSetBits, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::set_bits_from_isr(uint32_t bits, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, bits, eSetBits, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::increment(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    if (xTaskNotifyAndQueryIndexed(this->handle, index, value, eIncrement, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::increment_from_isr(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, value, eIncrement, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::overwrite_value(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    if (xTaskNotifyAndQueryIndexed(this->handle, index, value, eSetValueWithOverwrite, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::overwrite_value_from_isr(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, value, eSetValueWithOverwrite, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::send_value(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    if (xTaskNotifyAndQueryIndexed(this->handle, index, value, eSetValueWithoutOverwrite, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::send_value_from_isr(uint32_t value, u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, value, eSetValueWithoutOverwrite, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::clear(u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    if (xTaskNotifyAndQueryIndexed(this->handle, index, 0, eSetValueWithoutOverwrite, &this->last_value) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

bool task::notifier::clear_from_isr(u_base_type index, bool resume) {
    if (index > constants::max_notifications){
        return false;
    }
    
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (xTaskNotifyAndQueryIndexedFromISR(this->handle, index, 0, eSetValueWithoutOverwrite, &this->last_value, &xHigherPriorityTaskWoken) != pdPASS){
        return false;
    }

    if (resume == true){
        vTaskResume(this->handle);
    }

    return true;
}

uint32_t task::notifier::get_last_value(void) const {
    return this->last_value;
}