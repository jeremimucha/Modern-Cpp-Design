#include <iostream>
#include <memory>
#include <utility>


// C++ language rules allow for declaration of local classes - inside functions or methods,
// and even in templates - they can use template arguments in such cases. They are not idiom
// enabling feature, but they improve locality of symbols and can simplify implementation.

class Interface {
public:
    virtual void foo() = 0;
    virtual ~Interface() = default;
};

template<typename T, typename P>
std::unique_ptr<Interface> make_adapter(T&& obj, P&& arg)
{
    class Local : public Interface {
    public:
        Local(T&& obj, P&& arg)
            : obj_{std::forward<T>(obj)}
            , arg_{std::forward<P>(arg)}
        {
        }

        void foo() override { obj_.call(arg_); }
    private:
        T obj_;
        P arg_;
    };

    return std::make_unique<Local>(std::forward<T>(obj), std::forward<P>(arg));
}


int main()
{

}
