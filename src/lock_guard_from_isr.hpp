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
                this->locked = true;
                callback(data);
            }
        }

        ~lock_guard_from_isr(void){
            this->semaphore.give_from_isr();
            this->locked = false;
        }

        bool is_locked() const {
            return this->locked;
        }
    private:
        abstract::semaphore& semaphore;
        bool locked {false};
    };

    template <>
    class lock_guard_from_isr<void> {
    public:
        explicit lock_guard_from_isr(abstract::semaphore& semaphore, void (*callback)(void)) : 
        semaphore(semaphore)
        {
            if (semaphore.take_from_isr()){
                this->locked = true;
                callback();
            }
        }

        ~lock_guard_from_isr(void){
            this->semaphore.give_from_isr();
            this->locked = false;
        }

        bool is_locked() const {
            return this->locked;
        }
    private:
        abstract::semaphore& semaphore;
        bool locked {false};
    };
}
