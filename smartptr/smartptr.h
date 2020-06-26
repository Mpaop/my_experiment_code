#pragma once

#include <iostream>

namespace mpaop::smartptr
{
    template<typename T>
    class MSharedPtr
    {
    protected:
        T * m_ptr;
        uint32_t * m_refCount;

        MSharedPtr() : m_ptr(nullptr), m_refCount(nullptr) {}

    public:
        template<class... Args>
        MSharedPtr(Args... args)
        {
            m_ptr = new T(args...);
            m_refCount = new uint32_t(1);

            std::cout << "Calling constructor\n";
        }

        MSharedPtr(const MSharedPtr<T> & sp) : m_ptr(sp.m_ptr), m_refCount(sp.m_refCount)
        {
            // increment reference count
            ++(* m_refCount);

            std::cout << "Calling copy constructor\n";
            std::cout << "refCount: " << * m_refCount << "\n";
        }

protected:
        MSharedPtr(T * p, uint32_t * ref) : m_ptr(p), m_refCount(ref)
        {
            ++(* m_refCount);
        }

public:
        virtual ~MSharedPtr()
        {

            std::cout << "Calling destructor\n";

            --(* m_refCount);
            std::cout << "refCount: " << * m_refCount << "\n";

            if (0 == * m_refCount) cleanup();

            m_ptr = nullptr;
            m_refCount = nullptr;
        }

        T & operator * () const
        {
            return * m_ptr;
        }

        T * operator -> () const
        {
            return m_ptr;
        }

        T * get() const
        {
            return m_ptr;
        }

    private:
        inline void cleanup()
        {
            if(m_ptr) delete m_ptr;
            if(m_refCount) delete m_refCount;
            std::cout << "deleted ptr\n";
        }
    };
}