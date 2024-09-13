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
            protected:
                task_handle handle {nullptr};
                bool auto_start {false};
                task(bool auto_start = false);
            public:
                ~task();
                bool resume(void);
                bool resume_from_isr(void);
                bool suspend(void);
                bool suspend_from_isr(void);
                bool is_running(void);
                bool join(uint32_t observer_delay_ms = 100, uint32_t timeout_ms = max_delay_ms);
                bool is_valid(void);
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
                abstract::task(auto_start),
                arguments(param), 
                callback(callback) 
                {
                    this->handle = xTaskCreateStatic(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer);
                    if (this->handle != nullptr && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
        };

        template <uint32_t STACK_SIZE>
        class task<STACK_SIZE, void> : public abstract::task {
            private:
                task_stack stack[STACK_SIZE];
                timer_struct buffer;

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
                abstract::task(auto_start),
                callback(callback)
                {
                    this->handle = xTaskCreateStatic(wrapper, name, STACK_SIZE, this, this->auto_start ? priority : constants::max_priority, stack, &buffer);
                    if (this->handle != nullptr && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }
                
                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
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
                task(const char* name, uint16_t priority, bool auto_start, uint32_t sack_size, ARGUMENT_TYPE param, void (*callback)(ARGUMENT_TYPE)) :
                abstract::task(auto_start),
                arguments(param), 
                callback(callback) 
                {
                    BaseType_t status = xTaskCreate(wrapper, name, sack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle) == pdPASS;
                    if (status == pdPASS && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
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
                task(const char* name, uint16_t priority, bool auto_start, uint32_t sack_size, std::function <void()> callback) :
                abstract::task(auto_start),
                callback(callback)
                {
                    BaseType_t status = xTaskCreate(wrapper, name, sack_size, this, this->auto_start ? priority : constants::max_priority, &this->handle) == pdPASS;
                    if (status == pdPASS && !this->auto_start) {
                        this->join();
                        vTaskPrioritySet(this->handle, priority);
                    }
                }

                using abstract::task::resume;
                using abstract::task::suspend;
                using abstract::task::is_running;
                using abstract::task::join;
                using abstract::task::is_valid;
        };
    }
}