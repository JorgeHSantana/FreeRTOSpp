#pragma once

namespace freertos {
    namespace abstract {
        template <typename DATA_TYPE>
        class safe_data {
            protected:
                DATA_TYPE data;
            public:
                safe_data(const DATA_TYPE& data) : data(data) {}

                void set(const DATA_TYPE& data){
                    this->data = data;
                }

                DATA_TYPE get(void){
                    return this->data;
                }

                template <typename ARGUMENT_TYPE>
                void use(ARGUMENT_TYPE arguments, void (*callback)(DATA_TYPE&, ARGUMENT_TYPE)){
                    callback(this->data, arguments);
                }

                operator DATA_TYPE(void){
                    return this->get();
                }

                safe_data& operator=(const DATA_TYPE& data){
                    this->set(data);
                    return *this;
                }
        };
    }
}