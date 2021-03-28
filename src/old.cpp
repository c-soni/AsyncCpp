#include <iostream>

// concept for callback return type
// concept for callback args type
// concept for callback fn type

template <typename T>
concept CallableFn = requires(T t) { t(); };

auto call(CallableFn auto x) -> void
{
    x();
}

template <typename F, typename R, typename... Args>
auto callTheCallback(F f, Args &&...a) -> R
{
    return f(a...);
}

class Callable
{
    int val = 5;

public:
    auto operator()() -> int
    {
        std::cout << val + 5 << std::endl;
        return val + 5;
    }
};

int main()
{
    Callable c;
    int a = 5;
    float f = static_cast<float>(a);
    std::cout << "Val: " << f << std::endl; 
    // std::cout << callTheCallback<Callable, int>(c, 10, 20) << std::endl;
    call(c);
}