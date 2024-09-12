#pragma once

#include "constants.hpp"
#include "typedefs.hpp"

namespace freertos {

    using namespace typedefs;
    using namespace constants;

    namespace abstract {
        class semaphore {
        protected:
            semaphore_handle handle {nullptr};

            enum class type {
                none = -1,
                binary = 0,
                counting = 1,
                mutex = 2,
                recursive = 3,
            };

            type semaphore_type {type::none};
            semaphore(type semaphore_type);
        public:
            ~semaphore(void);
            bool remove(void);
            bool is_valid(void) const;

            virtual bool give(void);
            virtual bool take(uint32_t timeout_ms = max_delay_ms);
            virtual bool give_from_isr(void);
            virtual bool take_from_isr(void);
        };
    }
}