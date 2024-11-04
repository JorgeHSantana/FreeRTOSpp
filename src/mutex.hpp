#pragma once

#include "semaphore.hpp"

namespace freertos {
    namespace heap {
        class mutex : public abstract::semaphore {
        public:
            mutex(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::is_valid;
            using abstract::semaphore::get_handle;
        };
    }

    namespace stack {
        class mutex : public abstract::semaphore {
        private:
            semaphore_struct buffer;
        public:
            mutex(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
            using abstract::semaphore::is_valid;
            using abstract::semaphore::get_handle;
        };
    }
}