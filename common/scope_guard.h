#ifndef SCOPE_GUARD_H
#define SCOPE_GUARD_H

#include <utility>

namespace utils {


template<typename F>
class ScopeGuard{
private:
    F functor_;
    bool active_;
public:
    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;

    ScopeGuard(ScopeGuard&& that)
        : functor_(std::move(that.functor_))
        , active_(that.active_)
    {
        that.dismiss();
    }

    ScopeGuard(const F& f) : functor_(f), active_(true){}
    ScopeGuard(F&& f) : functor_(std::move(f)), active_(true) {}
    ~ScopeGuard(){ if(active_) functor_(); }

    void dismiss(){ active_ = false; }
};

template <typename F>
inline ScopeGuard<F> make_scope_guard(F&& f){ return ScopeGuard<F>(std::forward<F>(f)); }



} // namespace utils

#endif // SCOPE_GUARD_H
