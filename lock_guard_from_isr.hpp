#pragma once

#include "semaphore.hpp"

namespace freertos {

    class lock_guard_from_isr {
    public:
        explicit lock_guard_from_isr(abstract::semaphore& semaphore);
        ~lock_guard_from_isr(void);
        bool owns_lock(void) const;
    private:
        abstract::semaphore& semaphore;
        bool owned {false};
    };

}
