#include "recursive.hpp"

using namespace freertos;

stack::recursive::recursive(void) : abstract::semaphore(semaphore::type::recursive){
    if (this->handle != nullptr) {
        return;
    }

    this->handle = xSemaphoreCreateRecursiveMutexStatic(&this->buffer);
}

heap::recursive::recursive(void) : abstract::semaphore(semaphore::type::recursive){
    if (this->handle != nullptr) {
        return;
    }

    this->handle = xSemaphoreCreateRecursiveMutex();
}