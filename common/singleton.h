#ifndef SINGLETON_H
#define SINGLETON_H

namespace utils {

template < typename T >
class Singleton : public T
{
public:
    static Singleton< T >& instance()
    {
        static Singleton< T > obj;
        return obj;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
};

} // namespace utils

#endif // SINGLETON_H
