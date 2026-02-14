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

        void incRefCount() noexcept
        {
            if(m_refCount)
            {
                ++(* m_refCount);
            }
        }

        void decRefCount() noexcept
        {
            if(m_refCount)
            {
                --(* m_refCount);
            }
        }

        void copyPtr(const MSharedPtr<T> & ptr)
        {
            this->release();
            this->m_ptr = ptr.m_ptr;
            this->m_refCount = ptr.m_refCount;

            incRefCount();
        }

    public:
        MSharedPtr<T>(std::nullptr_t) : m_ptr(nullptr), m_refCount(nullptr)
        {
            std::cout << "Calling empty constructor\n";
        }

        MSharedPtr<T>(const MSharedPtr<T> & sp) : m_ptr(sp.m_ptr), m_refCount(sp.m_refCount)
        {
            // increment reference count
            std::cout << "Calling Copy Constructor\n";
            incRefCount();
        }

        template<class Derived>
        MSharedPtr<T>(const MSharedPtr<Derived> & sp) noexcept
        {
            if (std::is_base_of<T, Derived>())
            {
                m_ptr = sp.m_ptr;
                m_refCount = sp.m_refCount;
                incRefCount();
                std::cout << "called derived copy constructor\n";
            }
            else
            {
                m_ptr = nullptr;
                m_refCount = nullptr;
            }
        }

        template <class... Args>
        static MSharedPtr<T> create(Args... args)
        {
            MSharedPtr<T> res(nullptr);
            res.m_ptr = new T(args...);
            res.m_refCount = new uint32_t(1);

            return res;
        }

        void release()
        {
            if (m_ptr)
            {
                decRefCount();
                std::cout << "refCount: " << * m_refCount << " of val = " << m_ptr << "\n" << std::flush;

                if (0 == * m_refCount) cleanup();

                m_ptr = nullptr;
                m_refCount = nullptr;
            }
        }

        virtual ~MSharedPtr()
        {
            std::cout << "Calling destructor\n";

            this->release();
        }

        T & operator * () const
        {
            return * m_ptr;
        }

        T * operator -> () const noexcept
        {
            return m_ptr;
        }

        operator bool() const noexcept
        {
            return m_ptr != nullptr;
        }

        MSharedPtr<T> & operator=(const MSharedPtr<T> & pt);
        std::nullptr_t operator=(std::nullptr_t);

        T * get() const
        {
            return m_ptr;
        }

        bool hasValue() const noexcept
        {
            return m_ptr != nullptr;
        }

    private:
        inline void cleanup()
        {
            if(m_ptr)
            {
                delete m_ptr;
                m_ptr = nullptr;
            }
            if(m_refCount)
            {
                delete m_refCount;
                m_refCount = nullptr;
            }
            std::cout << "deleted ptr\n";
        }

        template<typename>
        friend class MSharedPtr;
    };

    template<typename T>
    MSharedPtr<T> & MSharedPtr<T>::operator=(const MSharedPtr<T> & pt)
    {
        if(this->m_ptr == pt.m_ptr) return * this;

        std::cout << "Calling assignment\n";

        this->copyPtr(pt);

        std::cout << "Ref Count: " << * this->m_refCount << "\n";

        return * this;
    }

    template<typename T>
    std::nullptr_t MSharedPtr<T>::operator=(std::nullptr_t)
    {
        this->release();

        std::cout << "Called nullify\n";

        return nullptr;
    }
}