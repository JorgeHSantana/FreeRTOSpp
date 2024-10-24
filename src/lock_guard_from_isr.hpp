#pragma once

#include "semaphore.hpp"

namespace freertos {
    template <typename DATA_TYPE = void>
    class lock_guard_from_isr;

    template <typename DATA_TYPE>
    class lock_guard_from_isr {
    public:
        explicit lock_guard_from_isr(abstract::semaphore& semaphore, DATA_TYPE& data, void (*callback)(DATA_TYPE&)):
        semaphore(semaphore)
        {
            if (semaphore.take_from_isr()){
                callback(data);
            }
        }

        ~lock_guard_from_isr(void){
            this->semaphore.give_from_isr();
        }
    private:
        abstract::semaphore& semaphore;
    };

    template <>
    class lock_guard_from_isr<void> {
    public:
        explicit lock_guard_from_isr(abstract::semaphore& semaphore, void (*callback)(void)) : 
        semaphore(semaphore)
        {
            if (semaphore.take_from_isr()){
                callback();
            }
        }

        ~lock_guard_from_isr(void){
            this->semaphore.give_from_isr();
        }
    private:
        abstract::semaphore& semaphore;
    };
}
