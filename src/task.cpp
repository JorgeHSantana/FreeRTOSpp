#include "task.hpp"

using namespace freertos;
using namespace constants;
using namespace typedefs;
using namespace abstract;

task::task(task_handle handle) : handle(handle) {}

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
    return this->is_valid() && eTaskGetState(this->handle) == eRunning;
}

bool task::join(uint32_t observer_delay_ms, uint32_t timeout_ms) {
    if (this->handle == nullptr) {
        return false;
    }

    return this_task::delay_for<task&>(*this, [](task& self) -> bool {
        return !self.is_running();
    }, observer_delay_ms, timeout_ms);
}

bool task::is_valid(void) {
    return this->handle != nullptr;
}

task::notifier task::get_notifier(void) {
    return task::notifier(this->handle);
}

task::info task::get_info(void) {
    return task::info(this->handle);
}

task_handle& task::get_handle(void) {
    return this->handle;
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

task::info::info(task_handle handle) : handle(handle) {
    vTaskGetInfo(this->handle, &this->status, pdTRUE, eInvalid);
}

uint16_t task::info::get_priority(void) const {
    return this->status.uxCurrentPriority;
}

uint16_t task::info::get_base_priority(void) const {
    return this->status.uxBasePriority;
}

uint32_t task::info::get_free_stack_memory(void) const {
    return this->status.usStackHighWaterMark * sizeof(task_stack);
}

task::info::state task::info::get_state(void) const {
    switch (this->status.eCurrentState) {
        case eReady:
            return state::ready;
        case eRunning:
            return state::running;
        case eBlocked:
            return state::blocked;
        case eSuspended:
            return state::suspended;
        case eDeleted:
            return state::deleted;
        default:
            return state::invalid;
    }
}

uint32_t task::info::get_core_id(void) const {
    return this->status.xCoreID;
}

const char* task::info::get_name(void) const {
    return pcTaskGetName(this->handle);
}

bool task::info::is_ready(void) const {
    return this->status.eCurrentState == eReady;
}

bool task::info::is_running(void) const {
    return this->status.eCurrentState == eRunning;
}

bool task::info::is_blocked(void) const {
    return this->status.eCurrentState == eBlocked;
}

bool task::info::is_suspended(void) const {
    return this->status.eCurrentState == eSuspended;
}

bool task::info::is_deleted(void) const {
    return this->status.eCurrentState == eDeleted;
}

bool task::info::is_invalid(void) const {
    return this->status.eCurrentState == eInvalid;
}

task_handle task::self::get_handle(void) {
    return xTaskGetCurrentTaskHandle();
}

void task::self::suspend(void) {
    vTaskSuspend(nullptr);
}

void task::self::yield(void) {
    taskYIELD();
}

void task::self::delay(uint32_t time_ms) {
    vTaskDelay(pdMS_TO_TICKS(time_ms));
}

bool task::self::get_notification(uint32_t& notification_value, u_base_type index, uint32_t timeout_ms) {
    if (index > constants::max_notifications){
        return false;
    }
    
    return xTaskNotifyWaitIndexed(index, 0x00, 0xFFFFFFFF, &notification_value, pdMS_TO_TICKS(timeout_ms)) == pdPASS;
}

task::info task::self::get_info(void) {
    return task::info(xTaskGetCurrentTaskHandle());
}