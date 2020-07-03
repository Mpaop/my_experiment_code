#pragma once

#include "reflist.h"

namespace mpaop::tlinkedlist
{
    template <template<typename U> class T, typename K>
    class TNodeIterator
    {
    private:
        T<K> * m_ptr;

    public:
        TNodeIterator(T<K> *p) : m_ptr(p) {}
        virtual ~TNodeIterator() { m_ptr = nullptr; }
        TNodeIterator operator++()
        {
            m_ptr = m_ptr->Next();
            return *this;
        }

        bool operator!=(const TNodeIterator &it) const
        {
            return m_ptr != it.m_ptr;
        }

        T<K> & operator * () const
        {
            return * this->m_ptr;
        }

        T<K> * operator -> () const
        {
            return this->m_ptr;
        }
    };

    template <template<class U> class T, typename K>
    class TLinkedList
    {
    protected:
        T<K> * m_first;
        T<K> * m_last;
        T<K> * m_current;
        int32_t m_size;

    public:
        TLinkedList() : m_first(nullptr), m_last(nullptr), m_current(nullptr), m_size(0) {}
        virtual ~TLinkedList()
        {
            if (m_size > 0)
            {
                T<K> * node = m_last;

                // delete while tmp is valid ptr
                do
                {
                    T<K> *tmp = node->Prev();
                    delete node;
                    node = tmp;
                } while (node);
                m_first = m_last = m_current = nullptr;
            }
        }

        auto Size() const & -> const int32_t & { return m_size; }
        auto First() const & -> const T<K> * { return m_first->get(); }
        auto First() & -> K * { return m_first->get(); }
        auto Last() const & -> const K * { return m_last->get(); }
        auto Last() & -> K * { return m_last->get(); }
        auto Current() const & -> const K * { return m_current->get(); }
        auto Current() & -> K * { return m_current->get(); }

        TNodeIterator<T, K> begin() const { return TNodeIterator<T, K>(m_first); }
        TNodeIterator<T, K> end() const { return TNodeIterator<T, K>(nullptr); }

        T<K> * operator[](const uint32_t &idx)
        {
            m_current = getNode(idx);
            return m_current;
        }

        T<K> * moveCurrentNodeTo(const uint32_t &idx)
        {
            return this[idx];
        }

        T<K> * getNode(const uint32_t &idx)
        {
            if (m_size <= idx)
                throw std::out_of_range("LOG: idx is out of range!");

            T<K> * node;

            if ((m_size / 2.0f) >= idx)
            {
                node = m_first;
                for (uint32_t i = 0; i < idx; ++i)
                    node = node->Next();
            }
            else
            {
                node = m_last;
                for (uint32_t i = m_size - 1; i >= 0; --i)
                {
                    if (idx == i)
                        break;
                    node = node->Prev();
                }
            }

            return node;
        }

        template <class... Args>
        T<K> * Push(Args... args)
        {
            ++m_size;
            if (m_last == nullptr)
            {
                m_last = m_first = m_current = new T<K>(nullptr, nullptr, args...);
                return m_last;
            }

            m_last = new T<K>(m_last, nullptr, args...);

            return m_last;
        }

        T<K> * PushNode(T<K> & node)
        {
            ++m_size;
            if (m_last == nullptr)
            {
                m_last = m_first = m_current = new T<K>(node);
                return m_last;
            }

            T<K> * oldLast = m_last;
            m_last = new T<K>(node);
            m_last->insertSelf(oldLast, nullptr);

            return m_last;
        }

        void Pop()
        {
            if (m_size > 1)
            {
                T<K> *prev = m_last->Prev();
                if (m_current == m_last)
                    m_current = prev;

                delete m_last;
                m_last = prev;
            }
            else if (m_size == 1)
            {
                delete m_last;
                m_current = m_first = m_last = nullptr;
            }
            else
            {
                throw std::runtime_error("LOG: List is empty!");
            }

            --m_size;
        }

        template <class... Args>
        T<K> * Insert(const uint32_t & idx, Args... args)
        {
            if (idx > m_size)
                throw std::out_of_range("LOG: idx exceeds size of list");

            T<K> * res;

            if (idx == m_size)
                res = Push(args...);
            else if (idx == 0)
            {
                m_first = new T<K>(nullptr, m_first, args...);
                res = m_first;
            }
            else
            {
                T<K> * node = getNode(idx);
                T<K> * newNode = new T<K>(node->Prev(), node, args...);
                res = newNode;
            }

            ++m_size;
            return res;
        }

        T<K> * InsertNode(const uint32_t &idx, T<K> & node)
        {
            if (idx > m_size)
                throw std::out_of_range("LOG: idx exceeds size of list");

            T<K> *res;

            if (idx == m_size)
                res = Push(node);
            else if (idx == 0)
            {
                T<K> *oldFirst = m_first;
                m_first = new T<K>(node);
                m_first->insertSelf(nullptr, oldFirst);
                res = m_first;
            }
            else
            {
                T<K> * n = getNode(idx);
                T<K> * newNode = new T<K>(node);
                newNode->insertSelf(n->Prev(), n);
                res = newNode;
            }

            ++m_size;
            return res;
        }

        void Remove(const uint32_t &idx)
        {
            if (idx >= m_size)
                throw std::out_of_range("LOG: Cannot access this idx");

            T<K> *node = getNode(idx);

            if (m_size == 1)
            {
                m_current = m_first = m_last = nullptr;
                delete node;
                --m_size;
                return;
            }

            if (idx == m_size - 1)
            {
                m_last = node->Prev();
                if (m_current == node)
                    m_current = m_last;
                delete node;
            }
            else if (idx == 0)
            {
                m_first = node->Next();
                if (m_current == node)
                    m_current = m_first;
                delete node;
            }
            else
            {
                if (m_current == node)
                    m_current = node->Prev() ? node->Prev() : node->Next();
                delete node;
            }

            --m_size;
        }
    };
}