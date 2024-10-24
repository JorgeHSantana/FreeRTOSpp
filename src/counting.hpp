#pragma once

#include "semaphore.hpp"

namespace freertos {
    namespace heap {
        class counting : public abstract::semaphore {
        public:
            counting(uint32_t max_count, uint32_t initial_count = 0);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
        };
    }

    namespace stack {
        class counting : public abstract::semaphore {
        private:
            semaphore_struct buffer;
        public:
            counting(uint32_t max_count, uint32_t initial_count = 0);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
        };
    }
}