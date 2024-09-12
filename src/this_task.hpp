#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "constants.hpp"
#include "typedefs.hpp"

namespace freertos {
    using namespace constants;

    class this_task {
        public:
            static void sleep(uint32_t time_ms);
            static void suspend(void);
            static void yield(void);
            static void remove(void);

            template <typename ARGUMENT_TYPE>
            static bool sleep_for(ARGUMENT_TYPE arguments, bool (*condition)(ARGUMENT_TYPE), uint32_t observer_delay_ms = 100, uint32_t timeout_ms = max_delay_ms){
                if (condition == nullptr) {
                    return false;
                }
                
                if (observer_delay_ms > timeout_ms || observer_delay_ms == 0) {
                    observer_delay_ms = 100;
                }

                uint32_t start_time = xTaskGetTickCount();
                uint32_t timeout_ticks = pdMS_TO_TICKS(timeout_ms);

                while (!condition(arguments)) {
                    if (timeout_ms != max_delay_ms && (xTaskGetTickCount() - start_time >= timeout_ticks)) {
                        return false;
                    }
                    this_task::sleep(observer_delay_ms);
                }

                return true;
            }

            template <typename ARGUMENT_TYPE>
            static void suspend_for(ARGUMENT_TYPE arguments, bool (*condition)(ARGUMENT_TYPE)){
                while (condition != nullptr && condition(arguments)) {
                    this_task::suspend();
                }
            }

            template <typename ARGUMENT_TYPE>
            static void yield_for(ARGUMENT_TYPE arguments, bool (*condition)(ARGUMENT_TYPE)){
                while (condition != nullptr && condition(arguments)) {
                    this_task::yield();
                }
            }

            template <typename ARGUMENT_TYPE>
            static void remove_if(ARGUMENT_TYPE arguments, bool (*condition)(ARGUMENT_TYPE)){
                if (condition != nullptr && condition(arguments)) {
                    this_task::remove();
                }
            }
    };
}