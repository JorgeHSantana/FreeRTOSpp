#pragma once

#include "collection.hpp"

namespace freertos {
    namespace stack {
        template <typename DATA_TYPE>
        class singleton : public abstract::collection<DATA_TYPE> {
        private:
            DATA_TYPE data;
        public:
            singleton(void) :
            abstract::collection<DATA_TYPE>(abstract::collection<DATA_TYPE>::send_mode::single, true, 1) 
            {
                if (this->handle != nullptr) {
                    return;
                }

                this->handle = xQueueCreateStatic(1, sizeof(DATA_TYPE), (uint8_t*)&this->data, &this->buffer);
            }

            using abstract::collection<DATA_TYPE>::pop;
            using abstract::collection<DATA_TYPE>::push;
            using abstract::collection<DATA_TYPE>::pop_from_isr;
            using abstract::collection<DATA_TYPE>::push_from_isr;
            using abstract::collection<DATA_TYPE>::available;
            using abstract::collection<DATA_TYPE>::get_free_space;
            using abstract::collection<DATA_TYPE>::clear;
            using abstract::collection<DATA_TYPE>::is_valid;
            using abstract::collection<DATA_TYPE>::is_queue_empty;
            using abstract::collection<DATA_TYPE>::is_queue_full;
            using abstract::collection<DATA_TYPE>::is_queue_empty_from_isr;
            using abstract::collection<DATA_TYPE>::is_queue_full_from_isr;
            using abstract::collection<DATA_TYPE>::get_item_size;
            using abstract::collection<DATA_TYPE>::get_storage_size;
        };
    }
}
