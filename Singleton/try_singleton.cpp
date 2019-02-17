#include <iostream>
#include "Singleton.hpp"

template<typename T>
class MayersSingleton {
public:
    static T& instance() {
        static T instance_;
        return instance_;
    }

    MayersSingleton(MayersSingleton const&) = delete;
    MayersSingleton& operator=(MayersSingleton const&) = delete;

protected:
    ~MayersSingleton() noexcept = default;
private:
    MayersSingleton() noexcept = default;
};


struct SomeSingleton {
    SomeSingleton() { std::cerr << __PRETTY_FUNCTION__ << "\n"; }
    ~SomeSingleton() noexcept { std::cerr << __PRETTY_FUNCTION__ << "\n"; }

    friend std::ostream& operator<<(std::ostream& os, SomeSingleton const& i)
    {
        return os << "SomeSingleton instance: " << reinterpret_cast<void const*>(&i);
    }
};

using Singleton = SingletonHolder<SomeSingleton, CreateUsingNew, DefaultLifetime, ClassLevelLock>;
// using Singleton = MayersSingleton<SomeSingleton>;

struct SomeClass {
    SomeClass() { std::cerr << __PRETTY_FUNCTION__ << "\n"; }
    ~SomeClass() noexcept
    {
        std::cerr << __PRETTY_FUNCTION__ << "\n";
        try {
            auto& i{Singleton::instance()};
            std::cerr << i << "\n";
        }
        catch(std::exception const& e) {
            std::cerr << "Exception caught while trying to reference the singleton\n";
        }
    }
};


int main()
{
    std::cerr << "Program start\n";
    SomeClass{};
    auto const& i{Singleton::instance()};
    std::cerr << i << "\n";
    std::cerr << "Leaving main()\n";
}
