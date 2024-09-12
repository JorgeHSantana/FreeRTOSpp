#pragma once

#include "typedefs.hpp"
#include "constants.hpp"

namespace freertos {

    using namespace typedefs;
    using namespace constants;

    namespace abstract {
        template <typename DATA_TYPE>
        class collection {
        protected:
            enum class send_mode : int8_t {
                none = -1,
                to_back = 0,
                to_front = 1,
                single = 2
            };
            send_mode mode {send_mode::none};
            queue_handle handle {nullptr};
            bool overwrite_if_full {false};
            const uint32_t storage_size {0};

            explicit collection(send_mode mode, bool overwrite_if_full, uint32_t storage_size): 
            mode(mode),
            overwrite_if_full(overwrite_if_full),
            storage_size(storage_size)
            {}
        public:
            ~collection(void){
                this->remove();
            }

            bool remove(void) {
                if (this->handle == nullptr) {
                    return false;
                }

                vQueueDelete(this->handle);
                this->handle = nullptr;
                return true;
            }

            bool pop(DATA_TYPE& data, uint32_t timeout_ms = max_delay_ms) {
                if (this->handle == nullptr || this->mode == send_mode::none) {
                    return false;
                }

                uint32_t ticks = 1;
                if (timeout_ms != max_delay_ms){
                    ticks = pdMS_TO_TICKS(timeout_ms);
                }

                return xQueueReceive(this->handle, (void*) &data, ticks) == pdPASS;
            }

            bool push(DATA_TYPE data, uint32_t timeout_ms = max_delay_ms) {
                if (this->handle == nullptr || this->mode == send_mode::none) {
                    return false;
                }

                uint32_t ticks = 1;
                if (this->mode == send_mode::single){
                    ticks = 0;
                }
                else if (timeout_ms != max_delay_ms){
                    ticks = pdMS_TO_TICKS(timeout_ms);
                }

                if (this->overwrite_if_full && this->mode != send_mode::single && this->is_queue_full()){
                    DATA_TYPE buffer;
                    if (this->pop(buffer, timeout_ms) == false){
                        return false;
                    }
                }

                return xQueueGenericSend(this->handle, static_cast<void*>(&data), ticks, static_cast<UBaseType_t>(this->mode)) == pdPASS;
            }

            bool pop_from_isr(DATA_TYPE& data) {
                if (this->handle == nullptr || this->mode == send_mode::none) {
                    return false;
                }

                return xQueueReceiveFromISR(this->handle, (void*) &data, nullptr) == pdPASS;
            }

            bool push_from_isr(DATA_TYPE data) {
                if (this->handle == nullptr || this->mode == send_mode::none) {
                    return false;
                }

                if (this->overwrite_if_full && this->mode != send_mode::single && this->is_queue_full_from_isr()){
                    DATA_TYPE buffer;
                    if (this->pop_from_isr(buffer) == false){
                        return false;
                    }
                }

                return xQueueGenericSendFromISR(this->handle, static_cast<const void*>(&data), nullptr, static_cast<UBaseType_t>(this->mode)) == pdPASS;
            }

            uint32_t available(void) const {
                return uxQueueMessagesWaiting(this->handle);
            }

            uint32_t get_free_space(void) const {
                return uxQueueSpacesAvailable(this->handle);
            }

            bool is_queue_empty(void) const {
                return (this->get_free_space() == this->storage_size);
            }

            bool is_queue_full(void) const {
                return (this->get_free_space() == 0);
            }

            bool is_queue_empty_from_isr(void) const {
                return xQueueIsQueueEmptyFromISR(this->handle);
            }

            bool is_queue_full_from_isr(void) const {
                return xQueueIsQueueFullFromISR(this->handle);
            }

            bool clear(void) {
                return xQueueReset(this->handle) == pdPASS;
            }

            bool is_valid(void) const {
                return this->handle != nullptr;
            }

            uint16_t get_item_size(void) const{
                return sizeof(DATA_TYPE);
            }

            uint32_t get_storage_size(void){
                return this->storage_size;
            }
        };
    }
}