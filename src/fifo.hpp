#pragma once

#include "collection.hpp"

namespace freertos {
    namespace heap {
        template <typename DATA_TYPE>
        class fifo : public abstract::collection<DATA_TYPE> {
        private:
        public:
            fifo(uint32_t storage_size, bool overwrite_if_full = false) :
            abstract::collection<DATA_TYPE>(abstract::collection<DATA_TYPE>::send_mode::to_back, overwrite_if_full, storage_size)
            {
                if (this->handle != nullptr) {
                    return;
                }

                this->handle = xQueueCreate(storage_size, sizeof(DATA_TYPE));
            }

            using abstract::collection<DATA_TYPE>::pop;
            using abstract::collection<DATA_TYPE>::push;
            using abstract::collection<DATA_TYPE>::pop_from_isr;
            using abstract::collection<DATA_TYPE>::push_from_isr;
            using abstract::collection<DATA_TYPE>::available;
            using abstract::collection<DATA_TYPE>::get_free_space;
            using abstract::collection<DATA_TYPE>::clear;
            using abstract::collection<DATA_TYPE>::is_valid;
            using abstract::collection<DATA_TYPE>::is_empty;
            using abstract::collection<DATA_TYPE>::is_full;
            using abstract::collection<DATA_TYPE>::is_empty_from_isr;
            using abstract::collection<DATA_TYPE>::is_full_from_isr;
            using abstract::collection<DATA_TYPE>::get_item_size;
            using abstract::collection<DATA_TYPE>::get_storage_size;
            using abstract::collection<DATA_TYPE>::get_handle;
        };
    }

    namespace stack {
        template <uint32_t STORAGE_SIZE, typename DATA_TYPE>
        class fifo : public abstract::collection<DATA_TYPE> {
        private:
            queue_struct buffer;
            uint8_t storage_area[STORAGE_SIZE * sizeof(DATA_TYPE)];
        public:
            fifo(bool overwrite_if_full = false) :
            abstract::collection<DATA_TYPE>(abstract::collection<DATA_TYPE>::send_mode::to_back, overwrite_if_full, STORAGE_SIZE) 
            {
                if (this->handle != nullptr) {
                    return;
                }

                this->handle = xQueueCreateStatic(STORAGE_SIZE, sizeof(DATA_TYPE), this->storage_area, &this->buffer);
            }

            using abstract::collection<DATA_TYPE>::pop;
            using abstract::collection<DATA_TYPE>::push;
            using abstract::collection<DATA_TYPE>::pop_from_isr;
            using abstract::collection<DATA_TYPE>::push_from_isr;
            using abstract::collection<DATA_TYPE>::available;
            using abstract::collection<DATA_TYPE>::get_free_space;
            using abstract::collection<DATA_TYPE>::clear;
            using abstract::collection<DATA_TYPE>::is_valid;
            using abstract::collection<DATA_TYPE>::is_empty;
            using abstract::collection<DATA_TYPE>::is_full;
            using abstract::collection<DATA_TYPE>::is_empty_from_isr;
            using abstract::collection<DATA_TYPE>::is_full_from_isr;
            using abstract::collection<DATA_TYPE>::get_item_size;
            using abstract::collection<DATA_TYPE>::get_storage_size;
            using abstract::collection<DATA_TYPE>::get_handle;
        };
    }
}