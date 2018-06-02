#pragma once

#include "Property.h"


namespace const_mutable_helper{
    using namespace tower120::utils;

    namespace from_mutable{

        class Data{
            int m_j;
            auto& self_mut() const { return const_cast<Data&>(*this); }
        public:
            auto j() {
                return Property{
                    [&]()     { return m_j; },
                    [&](int j){ m_j = j; }
                };
            }
            auto j() const {
                return self_mut().j().make_const();
            }
        };

    }

    void main(){
       from_mutable::Data data;
       data.j() = 2;
       std::cout << data.j() << std::endl;
    }
}