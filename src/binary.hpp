#pragma once

#include "semaphore.hpp"

namespace freertos {
    namespace heap {
        class binary : public abstract::semaphore {
        public:
            binary(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
            using abstract::semaphore::get_handle;
        };
    }

    namespace stack {
        class binary : public abstract::semaphore {
        private:
            semaphore_struct buffer;
        public:
            binary(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
            using abstract::semaphore::get_handle;
        };
    }
}