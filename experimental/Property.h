#pragma once

#include <utility>
#include <type_traits>


namespace tower120::utils::experimental {

    template<class Getter, class Setter>
    class Property {
        Getter getter;
        Setter setter;

        using GetT = decltype(getter());

        static_assert(std::is_object_v<GetT> || std::is_const_v<std::remove_reference_t<GetT>>
                      ,"Property Getter must return const ref/pointer or object.");

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
        Property(const Getter &getter, const Setter &setter)
                : getter(getter), setter(setter) {}

        // getters
        decltype(auto) get() const {
            return getter();
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
            setter(std::forward<Args>(args)...);
        }

        template<class Value>
        void operator=(Value &&value) {
            set(std::forward<Value>(value));
        }


        // forward comparison operators (do we need them?)
        template<class OtherGetter, class OtherSetter>
        bool operator==(const Property<OtherGetter, OtherSetter> &other) const {
            return get() == other.get();
        }

        template<class OtherGetter, class OtherSetter>
        bool operator!=(const Property<OtherGetter, OtherSetter> &other) const {
            return get() != other.get();
        }


        auto make_const() const {
            struct empty{};
            return Property<Getter, empty>(getter, empty{});
        }

    };

}