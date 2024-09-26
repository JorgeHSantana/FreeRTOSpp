#pragma once

#include "semaphore.hpp"

namespace freertos {
    namespace heap {
        class recursive : public abstract::semaphore {
        public:
            recursive(void);

            using abstract::semaphore::give;
            using abstract::semaphore::take;
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
            using abstract::semaphore::is_valid;
        };
    }
}