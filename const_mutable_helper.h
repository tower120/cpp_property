#pragma once

#include "Property.h"


namespace const_mutable_helper{

    namespace from_const{

        class Data{
            int m_j;
            const auto& self_const() const { return *this; }
        public:
            auto j() const {
                return Property{this
                    , [](const Data& self) { return self.m_j; }
                    , [](Data& self, int j){ self.m_j = j; }
                };
            }
            auto j() {
                return self_const().j().make_mut({});
            }
        };

    }

    namespace from_mutable{

        class Data{
            int m_j;
            auto& self_mut() const { return *const_cast<Data*>(this); }
        public:
            auto j() {
                return Property{this
                    , [](const Data& self) { return self.m_j; }
                    , [](Data& self, int j){ self.m_j = j; }
                };
            }
            auto j() const {
                return self_mut().j().make_const({});
            }
        };

    }

    void main(){
        {
            from_const::Data data;
            data.j() = 1;
            std::cout << data.j() << std::endl;
        }

        {
            from_mutable::Data data;
            data.j() = 2;
            std::cout << data.j() << std::endl;
        }
    }
}