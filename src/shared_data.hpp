#pragma once

#include "recursive.hpp"

namespace freertos {
    namespace abstract{
        template <typename DATA_TYPE>
        class shared_data {
        private:
            DATA_TYPE data;
        protected:
            abstract::semaphore* semaphore {nullptr};

            shared_data(const DATA_TYPE& data) : data(data), semaphore(nullptr) {}
            shared_data(void) : data(), semaphore(nullptr) {}
        public:
            void set(const DATA_TYPE& data){
                lock_guard guard(*this->semaphore);
                this->data = data;
            }
            
            void set_from_isr(const DATA_TYPE& data){
                lock_guard_from_isr guard(*this->semaphore);
                this->data = data;
            }

            DATA_TYPE get(void){
                lock_guard guard(*this->semaphore);
                return this->data;
            }

            DATA_TYPE get_from_isr(void){
                lock_guard_from_isr guard(*this->semaphore);
                return this->data;
            }

            void use(void (*callback)(DATA_TYPE&)){
                lock_guard guard(*this->semaphore);
                callback(this->data);
            }

            template <typename ARGUMENT_TYPE>
            void use(ARGUMENT_TYPE arguments, void (*callback)(DATA_TYPE&, ARGUMENT_TYPE)){
                lock_guard guard(*this->semaphore);
                callback(this->data, arguments);
            }

            void use_from_isr(void (*callback)(DATA_TYPE&)){
                lock_guard_from_isr guard(*this->semaphore);
                callback(this->data);
            }

            template <typename ARGUMENT_TYPE>
            void use_from_isr(ARGUMENT_TYPE arguments, void (*callback)(DATA_TYPE&, ARGUMENT_TYPE)){
                lock_guard_from_isr guard(*this->semaphore);
                callback(this->data, arguments);
            }
        };
    }

    namespace heap {
        template <typename DATA_TYPE>
        class shared_data : public abstract::shared_data<DATA_TYPE> {
            private:
                heap::recursive locker;
            public:
                shared_data(void) : abstract::shared_data<DATA_TYPE>(), locker() {
                    this->semaphore = &this->locker;
                }

                shared_data(const DATA_TYPE& data) : abstract::shared_data<DATA_TYPE>(data), locker() {
                    this->semaphore = &this->locker;
                }

                ~shared_data(void){
                    this->semaphore = nullptr;
                }
        };
    }

    namespace stack {
        template <typename DATA_TYPE>
        class shared_data : public abstract::shared_data<DATA_TYPE> {
            private:
                stack::recursive locker;
            public:
                shared_data(void) : abstract::shared_data<DATA_TYPE>(), locker() {
                    this->semaphore = &this->locker;
                }

                shared_data(const DATA_TYPE& data) : abstract::shared_data<DATA_TYPE>(data), locker() {
                    this->semaphore = &this->locker;
                }

                ~shared_data(void){
                    this->semaphore = nullptr;
                }
        };
    }
}
