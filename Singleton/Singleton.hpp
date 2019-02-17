#pragma once

#include <cstdlib>
#include <mutex>
#include <type_traits>
#include <utility>

#define GSL_THROW_ON_CONTRACT_VIOLATION
#include <gsl/gsl>
#if defined(NDEBUG)
#undef Expects
#undef Ensures
#define Expects(cond)
#define Ensures(cond)
#endif

// --- Creation policy
template<typename T>
class CreateUsingNew {
public:
    using value_type = T;

    template<typename... Args>
    static std::enable_if_t<std::is_constructible_v<T, Args...>, T*>
    create(Args&&... args) noexcept(std::is_nothrow_constructible_v<T, Args...>)
    {
        return new T{std::forward<Args>(args)...};
    }

    static void destroy(T* p) noexcept { delete p; }
};

// --- Lifetime policy

template<typename T>
class DefaultLifetime {
public:
    struct Exception : public std::exception { using std::exception::exception; };
    static void on_dead_reference() { throw Exception{}; }

    template<typename Function>
    static void schedule_destruction(Function f) { std::atexit(f); }
};

template<typename T>
class PhoenixLifetime {

};

// --- Threading policy

template<typename T>
class SingleThreaded {
private:
    struct [[maybe_unused]] Lock { };

public:
    using lock_type = Lock;
};

template<typename T>
class ClassLevelLock {
private:
    using mutex_type = std::mutex;
    class Lock : private std::lock_guard<mutex_type> {
    public:
        constexpr Lock()
            : std::lock_guard<mutex_type>{mutex_}
        {
        }
    };

// --- static data
    static inline mutex_type mutex_{};

public:
    using lock_type = Lock;
};


template<
    typename T,
    template<class> class CreationPolicy = CreateUsingNew,
    template<class> class LifetimePolicy = DefaultLifetime,
    template<class> class ThreadingModel = SingleThreaded
    >
class SingletonHolder /* : CreationPolicy<T>, LifetimePolicy<T>, ThreadingModel<T> */{
public:
    using instance_type = T;
    using Creation = CreationPolicy<T>;
    using Lifetime = LifetimePolicy<T>;
    using Threading = ThreadingModel<T>;
    using lock_type = typename Threading::lock_type;

    static instance_type& instance();

private:
    constexpr SingletonHolder() noexcept = default;
    static void destroy_singleton() noexcept;

// --- static data
    static inline instance_type* p_instance_{nullptr};
    static inline bool destroyed_{false};
};

template<
    typename T,
    template<class> class CreationPolicy,
    template<class> class LifetimePolicy,
    template<class> class ThreadingModel
    >
auto SingletonHolder<T,
                     CreationPolicy,
                     LifetimePolicy,
                     ThreadingModel
                    >::instance() -> instance_type&
{
    if (!p_instance_) {
        [[maybe_unused]] lock_type guard{};
        if (!p_instance_) {
            if (destroyed_) {
                Lifetime::on_dead_reference();
                destroyed_ = false;
            }
            p_instance_ = Creation::create();
            Lifetime::schedule_destruction(&destroy_singleton);
        }
    }
    return *p_instance_;
}

template<
    typename T,
    template<class> class CreationPolicy,
    template<class> class LifetimePolicy,
    template<class> class ThreadingModel
    >
void SingletonHolder<T,
                     CreationPolicy,
                     LifetimePolicy,
                     ThreadingModel
                    >::destroy_singleton() noexcept
{
    Expects(!destroyed_);
    Creation::destroy(p_instance_);
    p_instance_ = nullptr;
    destroyed_ = true;
}
