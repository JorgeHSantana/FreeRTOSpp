#pragma once

#include "semaphore.hpp"

namespace freertos {

    class lock_guard {
        private:
            abstract::semaphore& semaphore;
            bool locked {false};
        public:
            explicit lock_guard(abstract::semaphore& semaphore, uint32_t timeout_ms = constants::max_delay_ms);
            ~lock_guard(void);
            bool is_locked() const;
        };

}
