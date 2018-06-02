#pragma once

#include "Property.h"

namespace experimental_Property_test{
    using namespace tower120::utils::experimental;

    struct In{
        int x;
        int y;
    };

    class Data{
        In m_i;
    public:
        auto j() {
            return Property(
                [&]() -> const auto& { return m_i; },
                [&](const auto& i)   { m_i = i; }
            );
        }
        auto j() const {
            return const_cast<Data&>(*this).j().make_const();
        }
    };

    void main(){
        {
            // read
            const Data data{};
            std::cout << data.j()->x;
        }

        {
            // write
            Data data{};
            data.j() = In{1,2};
        }

    }
}