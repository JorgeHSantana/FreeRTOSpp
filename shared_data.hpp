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
        public:
            void set(const DATA_TYPE& data){
                lock_guard guard(*this->semaphore);
                this->data = data;
            }
            
            DATA_TYPE get(void){
                lock_guard guard(*this->semaphore);
                return this->data;
            }

            template <typename ARGUMENT_TYPE>
            void use(ARGUMENT_TYPE arguments, void (*callback)(DATA_TYPE&, ARGUMENT_TYPE)){
                lock_guard guard(*this->semaphore);
                callback(this->data, arguments);
            }

            operator DATA_TYPE(void){
                return this->get();
            }

            shared_data& operator=(const DATA_TYPE& data){
                this->set(data);
                return *this;
            }

        };
    }

    namespace heap {
        template <typename DATA_TYPE>
        class shared_data : public abstract::shared_data<DATA_TYPE> {
            private:
                heap::recursive locker;
            public:
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
                shared_data(const DATA_TYPE& data) : abstract::shared_data<DATA_TYPE>(data), locker() {
                    this->semaphore = &this->locker;
                }

                ~shared_data(void){
                    this->semaphore = nullptr;
                }
        };
    }
}