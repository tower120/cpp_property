#pragma once

#include <utility>

namespace tower120::utils {

    template<class Self, class Getter, class Setter>
    class Property {
        Self *self;
        /* [[no_unique_address]] */ Getter getter;
        /* [[no_unique_address]] */ Setter setter;

        using GetT = decltype(getter(*self));

        // from Boost
        template<typename T>
        class arrow_proxy {
            T t;
        public:
            arrow_proxy(T &&t) : t(std::move(t)) {}

            const T *operator->() const { return &t; }
        };

    public:
        // require c++17 guaranteed rvo without move ctr
        Property(const Property &) = delete;

        Property(Property &&) = delete;

        Property(Self *self, const Getter &getter, const Setter &setter)
                : self(self), getter(getter), setter(setter) {}


        // getters
        decltype(auto) get() const {
            return getter(static_cast<const Self&>(*self));
        }

        operator GetT() const {
            return get();
        }

        decltype(auto) operator*() const {
            return get();
        }

        decltype(auto) operator->() const {
            if constexpr (std::is_reference_v<GetT>) {
                return &get();
            } else if constexpr (std::is_pointer_v<GetT>) {
                return get();
            } else {
                // is value
                return arrow_proxy<GetT>(get());
            }
        }


        // setters
        // args... here, so you can use them with lambda overload
        template<class ...Args>
        void set(Args &&... args) {
            setter(*self, std::forward<Args>(args)...);
        }

        template<class Value>
        void operator=(Value &&value) {
            set(std::forward<Value>(value));
        }


        // forward comparison operators (do we need them?)
        template<class OtherSelf, class OtherGetter, class OtherSetter>
        bool operator==(const Property<OtherSelf, OtherGetter, OtherSetter> &other) const {
            return get() == other.get();
        }

        template<class OtherSelf, class OtherGetter, class OtherSetter>
        bool operator!=(const Property<OtherSelf, OtherGetter, OtherSetter> &other) const {
            return get() != other.get();
        }


        // helper
        class Acess {
            friend Self;

            Acess() {}

            Acess(const Acess &) = delete;

            Acess(Acess &&) = delete;
        };

        decltype(auto) make_mut(Acess) const {
            using MutSelf = std::remove_const_t<Self>;
            return Property<MutSelf, Getter, Setter>(const_cast<MutSelf *>(self), getter, setter);
        }

        decltype(auto) make_const(Acess) const {
            using ConstSelf = std::add_const_t<Self>;
            return Property<ConstSelf, Getter, Setter>(static_cast<ConstSelf *>(self), getter, setter);
        }

    };

}