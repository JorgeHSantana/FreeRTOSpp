#pragma once

#include "constants.hpp"
#include "typedefs.hpp"

namespace freertos {

    using namespace typedefs;
    using namespace constants;

    class semaphore_base {
    protected:
        semaphore_handle handle {nullptr};
        semaphore_struct buffer;
        
    public:
        ~semaphore_base(void);
        bool is_valid(void) const;

        virtual bool give(void);
        virtual bool take(uint32_t timeout_ms = max_delay_ms);
        virtual bool give_from_isr(void);
        virtual bool take_from_isr(void);
    };
}