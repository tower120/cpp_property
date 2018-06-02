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
        return Property(this,
           [](const Data& self) -> In {
                return {
                    self.m_i.x * 10,
                    self.m_i.y * -10
                };
           },
           [](Data& self, const auto& i){
                self.m_i = i;
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
Property(this, getter, setter)
```

```
Getter(const Self&) -> T
Getter(const Self&) -> const T&
Getter(const Self&) -> const T*
```

```
Setter(Self&, Args&&...)
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
```


From const:
```c++
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
```

---


Requires C++17.