#include <iostream>
#include "linear_hierarchy.hpp"

template<typename T, typename Base>
class EventHandler : public Base {
public:
    virtual void onEvent(T& obj, int eventId) {
        std::cout << eventId << ": " << &obj << "\n";
    }
    virtual ~EventHandler() noexcept = default;
};

struct Widget { };
struct Button { };
struct ScrollBar { };

using Lh1 = linear_hierarchy<EventHandler, Widget, Button, ScrollBar>;

int main()
{
    Lh1 inst1{};
    Widget w{};
    Button b{};
    ScrollBar sb{};
    inst1.onEvent(w, 0);
// doesn't really work - hidden overloads of onEvent() ...
    // inst1.onEvent(b, 1);
    // inst1.onEvent(sb, 2);
}
