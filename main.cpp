#include <iostream>
#include "Property.h"
#include "const_mutable_helper.h"

using namespace tower120::utils;

struct In{
    int x;
    int y;
};

class Data{
    In m_i;
public:
    auto i(){
        return Property(this
           , [](const Data& self) -> In {
                return {
                    self.m_i.x * 10,
                    self.m_i.y * -10
                };
           }
           , [](Data& self, const auto& i){
                self.m_i = i;
           }
        );
    }
};

int main() {
    Data data;
    data.i() = In{10,20};
    std::cout << data.i()->x << " : " << data.i()->y << std::endl;

    const_mutable_helper::main();
    return 0;
}