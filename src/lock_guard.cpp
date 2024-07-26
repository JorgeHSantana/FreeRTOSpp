#include "lock_guard.hpp"

using namespace freertos;

lock_guard::lock_guard(abstract::semaphore& semaphore, uint32_t timeout_ms) :
semaphore(semaphore)
{
    this->owned = this->semaphore.take(timeout_ms);
}

bool lock_guard::owns_lock(void) const {
    return this->owned;
}

lock_guard::~lock_guard(void) {
    if (this->owned){
        this->owned = !this->semaphore.give();
    }
}
