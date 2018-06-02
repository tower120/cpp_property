# cpp_property
Header only, zero overhead C# - like properties for C++!

```c++
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
           [](const auto& i){
               m_i = i;
           }
        );
    }
};

int main(){
   Data data;
   data.i() = In{1,2};
   std::cout
        << data.i()->x
        << data.i()->y;
}
```

Simple interface:
```
Property(getter, setter)
```

```
Getter() -> T
Getter() -> const T&
Getter() -> const T*
```

```
Setter(Args&&...)
```


**Accessors**:

`operator*() const`

`operator->() const`

`get() const`

`operator T() const`


**Mutators**:

`void set(Args&&...)`

`void operator=(Value&&)`


---

#### const/non const redundancy

From mutable:
```c++
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
```


---


Requires C++17.