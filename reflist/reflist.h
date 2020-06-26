#pragma once

#include "../smartptr/smartptr.h"

namespace mpaop::reflist
{
    // node class that is to be used by the reference list
    template<typename T>
    class MNode : public smartptr::MSharedPtr<T>
    {
    private:
        MNode<T> * m_prev;
        MNode<T> * m_next;

    public:
        template<class... Args>
        MNode(MNode<T> * prev, MNode<T> * next, Args... args) : smartptr::MSharedPtr<T>(args...), m_prev(prev), m_next(next)
        {
            insertSelf(prev, next);
        }

        MNode(MNode<T> & n) : smartptr::MSharedPtr<T>(n.m_ptr, n.m_refCount), m_prev(nullptr), m_next(nullptr) {}

        virtual ~MNode()
        {
            removeSelf();
            std::cout << "Deleted node\n";
        }

        auto Prev() & -> MNode<T> * { return m_prev; }
        auto Next() & -> MNode<T> * { return m_next; }

        void insertSelf(MNode<T> * prev, MNode<T> * next)
        {
            if(m_prev) m_prev->m_next = this;
            if(m_next) m_next->m_prev = this;
        }
        
        void removeSelf()
        {
            if (m_prev && m_next)
            {
                m_prev->m_next = m_next;
                m_next->m_prev = m_prev;
            }
            else if (m_prev)
            {
                m_prev->m_next = nullptr;
            }
            else if (m_next)
            {
                m_next->m_prev = nullptr;
            }

            m_prev = nullptr;
            m_next = nullptr;
        }
    };

    template<typename T>
    class MNodeIterator
    {
    private:
        MNode<T> * m_ptr;
    public:
        MNodeIterator(MNode<T> * p) : m_ptr(p) {}
        virtual ~MNodeIterator() { m_ptr = nullptr; }
        MNodeIterator operator++()
        {
            m_ptr = m_ptr->Next();
            return * this;
        }

        bool operator != (const MNodeIterator & it) const
        {
            return m_ptr != it.m_ptr;
        }

        MNode<T> & operator * () const
        {
            return * this->m_ptr;
        }

        MNode<T> * operator -> () const
        {
            return this->m_ptr;
        }
    };

    template<typename T>
    class MRefList
    {
    private:
        MNode<T> * m_first;
        MNode<T> * m_last;
        MNode<T> * m_current;
        int32_t m_size;

    public:
        MRefList() : m_first(nullptr), m_last(nullptr), m_current(nullptr), m_size(0) {}
        virtual ~MRefList()
        {
            if(m_size > 0)
            {
                MNode<T> * node = m_last;

                // delete while tmp is valid ptr
                do
                {
                    MNode<T> * tmp = node->Prev();
                    delete node;
                    node = tmp;
                } while (node);
                m_first = m_last = m_current = nullptr;
            }
        }

        auto Size() const & -> const int32_t & { return m_size; }
        auto First() const & -> const MNode<T> * { return m_first->get(); }
        auto First() & -> T * { return m_first->get(); }
        auto Last() const & -> const T * { return m_last->get(); }
        auto Last() & -> T * { return m_last->get(); }
        auto Current() const & -> const T * { return m_current->get(); }
        auto Current() & -> T * { return m_current->get(); }

        MNodeIterator<T> begin() const { return MNodeIterator<T>(m_first); }
        MNodeIterator<T> end() const { return MNodeIterator<T>(nullptr); }

        MNode<T> * operator[](const uint32_t & idx)
        {
            m_current = getNode(idx);
            return m_current;
        }

        MNode<T> * moveCurrentNodeTo(const uint32_t & idx)
        {
            return this[idx];
        }

        MNode<T> * getNode(const uint32_t & idx)
        {
            if (m_size <= idx) throw std::out_of_range("LOG: idx is out of range!");
            
            MNode<T> * node;

            if ((m_size / 2.0f) >= idx)
            {
                node = m_first;
                for (uint32_t i = 0; i < idx; ++i) node = node->Next();
            }
            else
            {
                node = m_last;
                for (uint32_t i = m_size - 1; i >= 0; --i)
                {
                    if (idx == i) break;
                    node = node->Prev();
                }
            }

            return node;
        }

        template<class... Args>
        MNode<T> * Push(Args... args)
        {
            ++m_size;
            if(m_last == nullptr)
            {
                m_last = m_first = m_current = new MNode<T>(nullptr, nullptr, args...);
                return m_last;
            }
            
            m_last = new MNode<T>(m_last, nullptr, args...);

            return m_last;
        }

        MNode<T> * PushNode(MNode<T> & node)
        {
            ++m_size;
            if(m_last == nullptr)
            {
                m_last = m_first = m_current = new MNode<T>(node);
                return m_last;
            }
            
            MNode<T> * oldLast = m_last;
            m_last = new MNode<T>(node);
            m_last->insertSelf(oldLast, nullptr);

            return m_last;
        }

        void Pop()
        {
            if (m_size > 1)
            {
                MNode<T> * prev = m_last->Prev();
                if (m_current == m_last) m_current = prev;

                delete m_last;
                m_last = prev;
            }
            else if(m_size == 1)
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

        template<class... Args>
        MNode<T> * Insert(const uint32_t & idx, Args... args)
        {
            if(idx > m_size) throw std::out_of_range("LOG: idx exceeds size of list");

            MNode<T> * res;

            if(idx == m_size) res = Push(args...);
            else if(idx == 0)
            {
                m_first = new MNode<T>(nullptr, m_first, args...);
                res = m_first;
            }
            else
            {
                MNode<T> * node = getNode(idx);
                MNode<T> * newNode = new MNode<T>(node->Prev(), node, args...);
                res = newNode;
            }

            ++m_size;
            return res;
        }

        MNode<T> * InsertNode(const uint32_t & idx, MNode<T> & node)
        {
            if(idx > m_size) throw std::out_of_range("LOG: idx exceeds size of list");

            MNode<T> * res;

            if(idx == m_size) res = Push(node);
            else if(idx == 0)
            {
                MNode<T> * oldFirst = m_first;
                m_first = new MNode<T>(node);
                m_first->insertSelf(nullptr, oldFirst);
                res = m_first;
            }
            else
            {
                MNode<T> * n = getNode(idx);
                MNode<T> * newNode = new MNode<T>(node);
                newNode->insertSelf(n->Prev(), n);
                res = newNode;
            }

            ++m_size;
            return res;
        }

        void Remove(const uint32_t & idx)
        {
            if (idx >= m_size) throw std::out_of_range("LOG: Cannot access this idx");

            MNode<T> * node = getNode(idx);

            if (m_size == 1)
            {
                m_current = m_first = m_last = nullptr;
                delete node;
                --m_size;
                return;
            }

            if(idx == m_size - 1)
            {
                m_last = node->Prev();
                if(m_current == node) m_current = m_last;
                delete node;                
            }
            else if (idx == 0)
            {
                m_first = node->Next();
                if(m_current == node) m_current = m_first;
                delete node;
            }
            else
            {
                if(m_current == node) m_current = node->Prev() ? node->Prev() : node->Next();
                delete node;
            }

            --m_size;
        }
    };
}