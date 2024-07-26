#pragma once

#include "semaphore.hpp"

namespace freertos {
    namespace heap {
        class recursive : public abstract::semaphore {
        public:
            recursive(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
        };
    }

    namespace stack {
        class recursive : public abstract::semaphore {
        private:
            semaphore_struct buffer;
        public:
            recursive(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::give_from_isr;
            using abstract::semaphore::take_from_isr;
            using abstract::semaphore::is_valid;
        };
    }
}