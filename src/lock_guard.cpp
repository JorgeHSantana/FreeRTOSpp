#include "lock_guard.hpp"

using namespace freertos;

#include "esp_log.h"

lock_guard::lock_guard(abstract::semaphore& semaphore, uint32_t timeout_ms) :
semaphore(semaphore)
{
    this->semaphore.take(timeout_ms);
}

lock_guard::~lock_guard(void) {
    this->semaphore.give();
}
