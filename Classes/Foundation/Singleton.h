/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Singleton_h
#define Foundation_Singleton_h

#include <mutex>
#include <stdlib.h>
#include "noncopyable.hpp"

namespace Foundation{

template< typename T, bool destroy_on_exit = true >
class Singleton : noncopyable
{
    typedef T                         instance_type;
    typedef instance_type*            instance_pointer;
    typedef volatile instance_pointer volatile_instance_pointer;

    static std::once_flag            m_once;
    static volatile_instance_pointer m_instance;
    static void destroy();

public:
    inline static instance_pointer getInstance()
    {
        /** 
         * 避免在对象的析构函数中调用单例对象的getInstance()方法，
         * 因为在程序结束的时候如果在析构函数中调用getInstance()，
         * 但是该单例对象已经被系统调用destroy删除了，那面将导致
         * 对空指针的使用而导致程序非正常结束。
         */
        std::call_once(m_once, [&]{
            m_instance = new instance_type();
            if (destroy_on_exit)
            {
                ::atexit(destroy);
            }
        });
        return m_instance;
    }

    inline static void destroyInstance()
    {
        if (!destroy_on_exit)
        {
            destroy();
        }
    }
};


template< typename T, bool destroy_on_exit >
void Singleton< T, destroy_on_exit>::destroy()
{
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    T_must_be_complete_type dummy; (void) dummy;
    delete m_instance;
    m_instance = nullptr;
}

template< typename T, bool destroy_on_exit >
typename Singleton< T, destroy_on_exit >::volatile_instance_pointer
    Singleton<T, destroy_on_exit>::m_instance = nullptr;

template< class T, bool destroy_on_exit >
std::once_flag Singleton<T, destroy_on_exit>::m_once;


template< typename T>
class CocosSingleton : noncopyable
{
    typedef T                         instance_type;
    typedef instance_type*            instance_pointer;
    typedef volatile instance_pointer volatile_instance_pointer;

    static volatile_instance_pointer  m_instance;
public:
    inline static T* getInstance()
    {
        if (! m_instance)
        {
            m_instance = new T();
            m_instance->init();
        }
        return m_instance;
    }

    inline static void destroyInstance()
    {
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy; (void) dummy;
        delete m_instance;
        m_instance = nullptr;
    }
};
template< typename T >
typename CocosSingleton< T >::volatile_instance_pointer
    CocosSingleton<T>::m_instance = nullptr;

} // namespace Foundation

#endif // Foundation_Singleton_h