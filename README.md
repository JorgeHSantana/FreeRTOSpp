# FreeRTOS++

FreeRTOS++ is an extension of FreeRTOS encapsulated in modern design patterns using C++. It enhances the usability and safety of FreeRTOS in embedded systems by leveraging object-oriented programming and modern C++ features.

## Features

- **Modern Design Patterns**: Utilizes modern C++ design patterns to encapsulate FreeRTOS functionality.
- **Safety**: Designed with embedded systems in mind, ensuring reliability and security.
- **Abstraction Layer**: Provides a higher-level abstraction over FreeRTOS APIs for easier integration and maintenance.
- **Compatibility**: Works seamlessly with existing FreeRTOS applications and configurations.
  
## Usage

To use FreeRTOS++, simply include the necessary headers and link the library with your project. Ensure compatibility with your version of FreeRTOS and configure according to your embedded system's requirements.

### Example 01

```cpp
#include <Arduino.h>
#include "etl/delegate.h"
#include "etl/string.h"
#include "etl/type_traits.h"
#include "freertos.hpp"

class my_app : public freertos::abstract::app {
    private:
        static constexpr const char* name = "Test";
        static constexpr uint32_t stack_size = 1024;
        static constexpr uint16_t priority = 1;

        freertos::stack::task<stack_size, freertos::abstract::app&> task;

        static void main(freertos::abstract::app& app);
        my_app() : freertos::abstract::app(task), task(name, priority, *this, my_app::main) {}
    public:
        static my_app& instance(void){
            static my_app instance;
            return instance;
        }
};

void my_app::main(freertos::abstract::app& app){
    while(1){
        Serial.println("Hello World");
        freertos::this_app::delay(200);
    }
}

void setup() {
    Serial.begin(115200);

    freertos::stack::lifo<2, etl::string<20>> data_struct(true);
    
    data_struct.push("Hello World!");
    data_struct.push("How are you?");
    data_struct.push("I'm fine!");
    while(data_struct.available() > 0) {
        etl::string<20> data;
        data_struct.pop(data);
        Serial.printf("Data: %s\n", data.c_str());
    }

    static freertos::stack::shared_data<int> test = 10;

    static freertos::stack::task<2048, freertos::abstract::shared_data<int>&> task_arg("task_arg", 1, false, test, [](freertos::abstract::shared_data<int>& param) {

        freertos::stack::mutex mutex;
        while (true) {
            int param_buf = param.get();

            if (param_buf > 20) {
                return;
            }

            Serial.printf("Task running: %d\n", param.get());

            param.set(param_buf + 1);
            
            freertos::this_task::delay(250);
        }
    });

    task_arg.resume();

    static struct {
        freertos::stack::shared_data<int>& data;
        freertos::stack::task<2048, freertos::abstract::shared_data<int>&>& task;
    } task_arg_struct = {
        test, 
        task_arg
    };

    static freertos::stack::task<2048, decltype(task_arg_struct)&> task("task", 1, task_arg_struct, [](decltype(task_arg_struct)& arg) {
        while (true) {
            if (!arg.task.is_running()) {
                Serial.printf("Restarting Task!\n");
                arg.data = 0;
                arg.task.resume();
            }
            Serial.printf("Hello World!\n");
            freertos::this_task::delay(1000);
        }
    });

    static int8_t heap_data = 0;
    static freertos::heap::task<> heap_task("heap_task", 1, 2048, [&]() {
        while(true){
            Serial.printf("Heap Task: %d\n", heap_data);
            freertos::this_task::delay(1000);
            heap_data++;
        }
    });

    static int data = 10;
    static freertos::stack::timer<int&> printer("timer", data, [](int& param) {
        param++;
        Serial.printf("timer: %d\n", param);
    }, 1000, true);
    printer.start();

    static freertos::heap::timer<void> printer2("timer2", [](void) {
        Serial.printf("timer2!\n");
    }, 200, true, true);

    my_app::instance().start();
}

void loop() {
    freertos::this_task::delay(1000);
}
```

### Example 02

```cpp
#include <Arduino.h>
#include "etl/delegate.h"
#include "etl/string.h"
#include "etl/type_traits.h"
#include "freertos.hpp"
#include "esp_timer.h"

class my_app : public freertos::abstract::app {
    private:
        static constexpr const char* name = "Test";
        static constexpr uint32_t stack_size = 4 * 1024;
        static constexpr uint16_t priority = 1;

        freertos::stack::task<stack_size, my_app&> task;

        static void main(my_app& app);
        my_app() : freertos::abstract::app(task), task(my_app::name, my_app::priority, *this, my_app::main) {}
    public:
        static my_app& instance(void){
            static my_app instance;
            return instance;
        }

        freertos::stack::shared_data_with_isr<int>* count {nullptr};
};

void my_app::main(my_app& app){
    while(1){
        app.count->use([](int& count){
            Serial.printf("Count 1: %d\n", count);
            freertos::this_app::delay(10000);
        });
        freertos::this_app::delay(1000);
    }
}

class my_app_2 : public freertos::abstract::app {
    private:
        static constexpr const char* name = "Test_2";
        static constexpr uint32_t stack_size = 4 * 1024;
        static constexpr uint16_t priority = 1;

        freertos::stack::task<stack_size, my_app_2&> task;

        static void main(my_app_2& app);
        my_app_2() : freertos::abstract::app(task), task(my_app_2::name, my_app_2::priority, *this, my_app_2::main) {}
    public:
        static my_app_2& instance(void){
            static my_app_2 instance;
            return instance;
        }

        freertos::stack::shared_data_with_isr<int>* count {nullptr};
};

void my_app_2::main(my_app_2& app){
    while(1){
        app.count->use([](int& count){
            Serial.printf("Count 2: %d\n", count);
        });
        freertos::this_app::delay(100);
    }
}

void IRAM_ATTR on_timer(void* param) {
    static auto* count_ptr = static_cast<freertos::stack::shared_data_with_isr<int>*>(param);
    if (count_ptr == nullptr) {
        return;
    }
    
    count_ptr->use_from_isr([](int& count){    
        ESP_DRAM_LOGE("TIMER", "Timer triggered");
        count++;
    });
}

void setup_timer(freertos::stack::shared_data_with_isr<int>* count_ptr) {
    esp_timer_create_args_t global_time_configs = {
        .callback = &on_timer,
        .arg = count_ptr,
        .dispatch_method = ESP_TIMER_ISR,
        .name = "HardTimer",
        .skip_unhandled_events = false
    };

    esp_timer_handle_t global_timer_handle;
    ESP_ERROR_CHECK(esp_timer_create(&global_time_configs, &global_timer_handle));
    ESP_ERROR_CHECK(esp_timer_start_periodic(global_timer_handle, 100000));
}

void setup() {
    static freertos::stack::shared_data_with_isr<int> count;

    my_app::instance().count = &count;
    my_app::instance().start();

    my_app_2::instance().count = &count;
    my_app_2::instance().start();

    Serial.begin(115200);

    setup_timer(&count);
}

void loop() {
    freertos::this_task::suspend();
}
```

### Example 03

```cpp
#include <Arduino.h>
#include "etl/delegate.h"
#include "etl/string.h"
#include "etl/type_traits.h"
#include "freertos.hpp"
#include "esp_timer.h"
#include "esp_log.h"

using namespace freertos;

void setup() {
    static stack::task<2048> task_1("task_1", 0, true, []() {
        while(1){
            ESP_LOGI("task_1", "free stack memory: %d", this_task::get_free_stack_memory());

            uint32_t notification = 0;
            if (this_task::get_notification(notification, 0, 100)) {
                ESP_LOGE("task_1", "received: %d", notification);
            }

            this_task::suspend();
        }
    });

    static stack::task<2048, abstract::task&> task_2("task_2", 0, true, task_1, [](abstract::task& task) {
        static uint32_t notification = 0;
        while(1){
            ESP_LOGI("task_2", "free stack memory: %d", this_task::get_free_stack_memory());

            if (task.notify().send_value(notification, 0, true)){
                ESP_LOGE("task_2", "send: %d", notification);
                notification++;
            }

            this_task::delay(1000);
        }
    });
}

void loop() {
    this_task::suspend();
}
```
