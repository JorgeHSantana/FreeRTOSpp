#pragma once

#include "constants.hpp"
#include "typedefs.hpp"
#include "this_task.hpp"
#include <functional>

namespace freertos {

    using namespace constants;
    using namespace typedefs;

    namespace abstract {
        class task {
            public:
                class notifier {
                    private:
                        task_handle handle {nullptr};
                        uint32_t last_value {0};
                        notifier(task_handle handle);

                        friend class task;
                    public:
                        bool signal(u_base_type index = 0, bool resume = true);
                        bool signal_from_isr(u_base_type index = 0, bool resume = true);

                        bool set_bits(uint32_t bits, u_base_type index = 0, bool resume = true);
                        bool set_bits_from_isr(uint32_t bits, u_base_type index = 0, bool resume = true);

                        bool increment(uint32_t value, u_base_type index = 0, bool resume = true);
                        bool increment_from_isr(uint32_t value, u_base_type index = 0, bool resume = true);

                        bool overwrite_value(uint32_t value, u_base_type index = 0, bool resume = true);
                        bool overwrite_value_from_isr(uint32_t value, u_base_type index = 0, bool resume = true);

                        bool send_value(uint32_t value, u_base_type index = 0, bool resume = true);
                        bool send_value_from_isr(uint32_t value, u_base_type index = 0, bool resume = true);

                        bool clear(u_base_type index = 0, bool resume = true);
                        bool clear_from_isr(u_base_type index = 0, bool resume = true);

                        uint32_t get_last_value(void) const;
                };
            protected:
                task_handle handle {nullptr};
                bool auto_start {false};
                uint32_t stack_size {0};

                task(bool auto_start = false, uint32_t stack_size = 0);
            public:
                ~task();
                bool resume(void);
                bool resume_from_isr(void);
                bool suspend(void);
                bool suspend_from_isr(void);
                bool is_running(void);
                bool join(uint32_t observer_delay_ms = 100, uint32_t timeout_ms = max_delay_ms);
                bool is_valid(void);
                notifier notify(void);
                uint32_t get_stack_memory_size(void);
                uint32_t get_free_stack_memory(void);
                uint32_t get_used_stack_memory(void);

        };
    }

    namespace stack {
        template <uint32_t STACK_SIZE, typename ARGUMENT_TYPE = void>
        class task;

        template <uint32_t STACK_SIZE, typename ARGUMENT_TYPE>
        class task : public abstract::task {
            private:
                task_stack stack[STACK_SIZE];
                task_struct buffer;
                ARGUMENT_TYPE arguments;
                void (*callback)(ARGUMENT_TYPE);

                static void wrapper(void* arguments){
                    if (arguments == nullptr) {
                        return;
                    }

                    task& self = *static_cast<task*>(arguments);

                    if (!self.auto_start){
                        this_task::suspend();
                    }

                    while (1) {
                        if (self.callback != nullptr) {
                            self.callback(self.arguments);
                        }

                        this_task::suspend();
                    }
                }

            public:
                task(const char* name, uint16_t priority, bool auto_start, ARGUMENT_TYPE param, void (*callback)(ARGUMENT_TYPE)) :
                abstract::task(auto_start, STACK_SIZE),
                arguments(param), 
                callback(callback) 
                {
                    this->handle = xTaskCreateStatic(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer);
                    if (this->handle != nullptr && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                #if defined(ESP32)
                    task(const char* name, uint16_t priority, bool auto_start, ARGUMENT_TYPE param, void (*callback)(ARGUMENT_TYPE), uint8_t core_id) :
                    abstract::task(auto_start, STACK_SIZE),
                    arguments(param),
                    callback(callback)
                    {
                        this->handle = xTaskCreateStaticPinnedToCore(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer, core_id);
                        if (this->handle != nullptr && !this->auto_start) {
                            this->join();
                            vTaskPrioritySet(this->handle, priority);
                        }
                    }
                #endif

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
                using abstract::task::notify;
                using abstract::task::get_stack_memory_size;
                using abstract::task::get_free_stack_memory;
                using abstract::task::get_used_stack_memory;
        };

        template <uint32_t STACK_SIZE>
        class task<STACK_SIZE, void> : public abstract::task {
            private:
                task_stack stack[STACK_SIZE];
                task_struct buffer;

                void (*callback)();

                static void wrapper(void* arguments) {
                    if (arguments == nullptr) {
                        return;
                    }

                    task& self = *static_cast<task*>(arguments);

                    if (!self.auto_start){
                        this_task::suspend();
                    }

                    while (1) {
                        if (self.callback != nullptr) {
                            self.callback();
                        }

                        this_task::suspend();
                    }
                }

            public:
                task(const char* name, uint16_t priority, bool auto_start, void (*callback)(void)) :
                abstract::task(auto_start, STACK_SIZE),
                callback(callback)
                {
                    this->handle = xTaskCreateStatic(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer);
                    if (this->handle != nullptr && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                #if defined(ESP32)
                    task(const char* name, uint16_t priority, bool auto_start, void (*callback)(void), uint8_t core_id) :
                    abstract::task(auto_start, STACK_SIZE),
                    callback(callback)
                    {
                        this->handle = xTaskCreateStaticPinnedToCore(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer, core_id);
                        if (this->handle != nullptr && !this->auto_start) {
                            this->join();
                            vTaskPrioritySet(this->handle, priority);
                        }
                    }
                #endif
                
                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
                using abstract::task::notify;
                using abstract::task::get_stack_memory_size;
                using abstract::task::get_free_stack_memory;
                using abstract::task::get_used_stack_memory;
        };
    }

    namespace heap {
        template <typename ARGUMENT_TYPE = void>
        class task;

        template <typename ARGUMENT_TYPE>
        class task : public abstract::task {
            private:
                ARGUMENT_TYPE arguments;
                void (*callback)(ARGUMENT_TYPE);

                static void wrapper(void* arguments) {
                    if (arguments == nullptr) {
                        return;
                    }

                    task& self = *static_cast<task*>(arguments);

                    if (!self.auto_start){
                        this_task::suspend();
                    }

                    while (1) {
                        if (self.callback != nullptr) {
                            self.callback(self.arguments);
                        }

                        this_task::suspend();
                    }
                }

            public:
                task(const char* name, uint16_t priority, bool auto_start, uint32_t stack_size, ARGUMENT_TYPE param, void (*callback)(ARGUMENT_TYPE)) :
                abstract::task(auto_start, stack_size),
                arguments(param), 
                callback(callback) 
                {
                    BaseType_t status = xTaskCreate(wrapper, name, stack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle) == pdPASS;
                    if (status == pdPASS && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                #if defined(ESP32)
                    task(const char* name, uint16_t priority, bool auto_start, uint32_t stack_size, ARGUMENT_TYPE param, void (*callback)(ARGUMENT_TYPE), uint8_t core_id) :
                    abstract::task(auto_start, stack_size),
                    arguments(param),
                    callback(callback)
                    {
                        BaseType_t status = xTaskCreatePinnedToCore(wrapper, name, stack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle, core_id) == pdPASS;
                        if (status == pdPASS && !this->auto_start) {
                            this->join();
                            vTaskPrioritySet(this->handle, priority);
                        }
                    }
                #endif

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
                using abstract::task::notify;
                using abstract::task::get_stack_memory_size;
                using abstract::task::get_free_stack_memory;
                using abstract::task::get_used_stack_memory;
        };

        template <>
        class task<void> : public abstract::task {
            private:
                std::function<void()> callback;

                static void wrapper(void* arguments) {
                    if (arguments == nullptr) {
                        return;
                    }

                    task& self = *static_cast<task*>(arguments);

                    if (!self.auto_start){
                        this_task::suspend();
                    }

                    while (1) {
                        if (self.callback != nullptr) {
                            self.callback();
                        }

                        this_task::suspend();
                    }
                }

            public:
                task(const char* name, uint16_t priority, bool auto_start, uint32_t stack_size, std::function <void()> callback) :
                abstract::task(auto_start, stack_size),
                callback(callback)
                {
                    BaseType_t status = xTaskCreate(wrapper, name, stack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle) == pdPASS;
                    if (status == pdPASS && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }
                
                #if defined(ESP32)
                    task(const char* name, uint16_t priority, bool auto_start, uint32_t stack_size, std::function <void()> callback, uint8_t core_id) :
                    abstract::task(auto_start, stack_size),
                    callback(callback)
                    {
                        BaseType_t status = xTaskCreatePinnedToCore(wrapper, name, stack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle, core_id) == pdPASS;
                        if (status == pdPASS && !this->auto_start) {
                            this->join();
                            vTaskPrioritySet(this->handle, priority);
                        }
                    }
                #endif

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
                using abstract::task::notify;
                using abstract::task::get_stack_memory_size;
                using abstract::task::get_free_stack_memory;
                using abstract::task::get_used_stack_memory;
        };
    }
}