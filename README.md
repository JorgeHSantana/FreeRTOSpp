# FreeRTOS++

FreeRTOS++ is an extension of FreeRTOS encapsulated in modern design patterns using C++. It enhances the usability and safety of FreeRTOS in embedded systems by leveraging object-oriented programming and modern C++ features.

## Features

- **Modern Design Patterns**: Utilizes modern C++ design patterns to encapsulate FreeRTOS functionality.
- **Safety**: Designed with embedded systems in mind, ensuring reliability and security.
- **Abstraction Layer**: Provides a higher-level abstraction over FreeRTOS APIs for easier integration and maintenance.
- **Compatibility**: Works seamlessly with existing FreeRTOS applications and configurations.
  
## Usage

To use FreeRTOS++, simply include the necessary headers and link the library with your project. Ensure compatibility with your version of FreeRTOS and configure according to your embedded system's requirements.

### Example

```cpp
#include <Arduino.h>
#include "etl/delegate.h"
#include "etl/string.h"
#include "etl/type_traits.h"
#include "freertos.hpp"

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
            
            if (param > 20) {
                return;
            }

            Serial.printf("Task running: %d\n", param.get());

            param = param + 1;
            
            freertos::this_task::sleep(250);
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

    static freertos::stack::task<2048, decltype(task_arg_struct)&> task("task", 1, true, task_arg_struct, [](decltype(task_arg_struct)& arg) {
        while (true) {
            if (!arg.task.is_running()) {
                Serial.printf("Restarting Task!\n");
                arg.data = 0;
                arg.task.resume();
            }
            Serial.printf("Hello World!\n");
            freertos::this_task::sleep(1000);
        }
    });

    static int8_t heap_data = 0;
    static freertos::heap::task<> heap_task("heap_task", 1, true, 2048, [&]() {
        while(true){
            Serial.printf("Heap Task: %d\n", heap_data);
            freertos::this_task::sleep(1000);
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
}

void loop() {
    freertos::this_task::sleep(1000);
}
