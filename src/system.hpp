#pragma once

#include <stdint.h>
#include "./typedefs.hpp"

namespace freertos {
    using namespace typedefs;

    class system {
        public:
            static tick_type get_tick_count(void);
            static tick_type get_tick_count_from_isr(void);
            static uint64_t get_milliseconds(void);
            static uint64_t get_milliseconds_from_isr(void);
            static u_base_type get_amount_of_tasks(void);
    };
}