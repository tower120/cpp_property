#include <iostream>
#include "Property.h"
#include "const_mutable_helper.h"
#include "test.h"

using namespace tower120::utils;

struct In{
    int x;
    int y;
};

class Data{
    In m_i;
public:
    auto i(){
        return Property(
           [&]() -> In {
                return {
                    m_i.x * 10,
                    m_i.y * -10
                };
           },
           [&](const auto& i){
                m_i = i;
           }
        );
    }
};

int main() {
    Data data;
    data.i() = In{10,20};
    std::cout << data.i()->x << " : " << data.i()->y << std::endl;

    const_mutable_helper::main();
    test::main();
    return 0;
}