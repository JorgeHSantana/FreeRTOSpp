#pragma once

#include "constants.hpp"
#include "typedefs.hpp"

namespace freertos {
    using namespace typedefs;
    using namespace constants;

    namespace abstract {
        class timer {
            protected:
                timer_handle handle;
            public:
                ~timer();
                bool remove(void);
                bool start(uint32_t delay_ms = 0);
                bool start_from_isr(void);
                bool stop(uint32_t delay_ms = 0);
                bool stop_from_isr(void);
                bool reset(uint32_t delay_ms = 0);
                bool reset_from_isr(void);
                bool set_period(uint32_t period_ms);
                bool set_period_from_isr(uint32_t period_ms);
                bool get_auto_reload(void);
                bool set_auto_reload(bool auto_reload);
                uint32_t get_period_ms(void);
                uint32_t get_expiration_time_ms(void);
                bool is_running(void);
                bool is_valid(void);
        };
    }

    namespace heap {
        template <typename ARGUMENT_TYPE = void>
        class timer;

        template <typename ARGUMENT_TYPE>
        class timer : public abstract::timer {
            private:
                void (*callback)(ARGUMENT_TYPE);
                ARGUMENT_TYPE argument;

                static void callback_wrapper(TimerHandle_t handle){
                    timer& self = *static_cast<timer*>(pvTimerGetTimerID(handle));
                    if (self.callback != nullptr) {
                        self.callback(self->argument);
                    }
                }

            public:
                timer(const char* name, ARGUMENT_TYPE argument, void (*callback)(ARGUMENT_TYPE), uint32_t period_ms = 0, bool auto_reload = false, bool auto_start = false):
                callback(callback),
                argument(argument)
                {
                    this->handle = xTimerCreate("", pdMS_TO_TICKS(period_ms), auto_reload, this, &timer::callback_wrapper);
                    if (auto_start) {
                        this->start();
                    }
                }

                using abstract::timer::start;
                using abstract::timer::start_from_isr;
                using abstract::timer::stop;
                using abstract::timer::stop_from_isr;
                using abstract::timer::reset;
                using abstract::timer::reset_from_isr;
                using abstract::timer::set_period;
                using abstract::timer::set_period_from_isr;
                using abstract::timer::get_auto_reload;
                using abstract::timer::set_auto_reload;
                using abstract::timer::get_period_ms;
                using abstract::timer::get_expiration_time_ms;
                using abstract::timer::is_running;
                using abstract::timer::is_valid;
        };

        template <>
        class timer<void> : public abstract::timer {
            private:
                void (*callback)(void);

                static void callback_wrapper(TimerHandle_t handle){
                    timer& self = *static_cast<timer*>(pvTimerGetTimerID(handle));
                    if (self.callback != nullptr) {
                        self.callback();
                    }
                }
            public:
                timer(const char* name, void (*callback)(), uint32_t period_ms = 0, bool auto_reload = false, bool auto_start = false):
                callback(callback)
                {
                    this->handle = xTimerCreate(name, pdMS_TO_TICKS(period_ms), auto_reload, this, &timer::callback_wrapper);
                    if (auto_start) {
                        this->start();
                    }
                }

                using abstract::timer::start;
                using abstract::timer::start_from_isr;
                using abstract::timer::stop;
                using abstract::timer::stop_from_isr;
                using abstract::timer::reset;
                using abstract::timer::reset_from_isr;
                using abstract::timer::set_period;
                using abstract::timer::set_period_from_isr;
                using abstract::timer::get_auto_reload;
                using abstract::timer::set_auto_reload;
                using abstract::timer::get_period_ms;
                using abstract::timer::get_expiration_time_ms;
                using abstract::timer::is_running;
                using abstract::timer::is_valid;
        };
    }

    namespace stack {
        template <typename ARGUMENT_TYPE = void>
        class timer;

        template <typename ARGUMENT_TYPE>
        class timer : public abstract::timer {
            private:
                void (*callback)(ARGUMENT_TYPE);
                ARGUMENT_TYPE argument;
                timer_struct buffer;

                static void callback_wrapper(TimerHandle_t handle){
                    timer& self = *static_cast<timer*>(pvTimerGetTimerID(handle));
                    if (self.callback != nullptr) {
                        self.callback(self.argument);
                    }
                }

            public:
                timer(const char* name, ARGUMENT_TYPE argument, void (*callback)(ARGUMENT_TYPE), uint32_t period_ms = 0, bool auto_reload = false, bool auto_start = false):
                callback(callback),
                argument(argument)
                {
                    this->handle = xTimerCreateStatic(name, pdMS_TO_TICKS(period_ms), auto_reload, this, &timer::callback_wrapper, &this->buffer);
                    if (auto_start) {
                        this->start();
                    }
                }

                using abstract::timer::start;
                using abstract::timer::start_from_isr;
                using abstract::timer::stop;
                using abstract::timer::stop_from_isr;
                using abstract::timer::reset;
                using abstract::timer::reset_from_isr;
                using abstract::timer::set_period;
                using abstract::timer::set_period_from_isr;
                using abstract::timer::get_auto_reload;
                using abstract::timer::set_auto_reload;
                using abstract::timer::get_period_ms;
                using abstract::timer::get_expiration_time_ms;
                using abstract::timer::is_running;
                using abstract::timer::is_valid;
        };

        template <>
        class timer<void> : public abstract::timer {
            private:
                void (*callback)(void);
                timer_struct buffer;

                static void callback_wrapper(TimerHandle_t handle){
                    timer& self = *static_cast<timer*>(pvTimerGetTimerID(handle));
                    if (self.callback != nullptr) {
                        self.callback();
                    }
                }

            public:
                timer(const char* name, void (*callback)(), uint32_t period_ms = 0, bool auto_reload = false, bool auto_start = false):
                callback(callback)
                {
                    this->handle = xTimerCreateStatic(name, pdMS_TO_TICKS(period_ms), auto_reload, this, &timer::callback_wrapper, &this->buffer);
                    if (auto_start) {
                        this->start();
                    }
                }

                using abstract::timer::start;
                using abstract::timer::start_from_isr;
                using abstract::timer::stop;
                using abstract::timer::stop_from_isr;
                using abstract::timer::reset;
                using abstract::timer::reset_from_isr;
                using abstract::timer::set_period;
                using abstract::timer::set_period_from_isr;
                using abstract::timer::get_auto_reload;
                using abstract::timer::set_auto_reload;
                using abstract::timer::get_period_ms;
                using abstract::timer::get_expiration_time_ms;
                using abstract::timer::is_running;
                using abstract::timer::is_valid;
        };
    }
}