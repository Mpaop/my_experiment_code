#pragma once

#include "../smartptr/smartptr.h"
#include "TLinkedList.h"

namespace mpaop::reflist
{
    // node class that is to be used by the reference list
    template<typename T>
    class MNode : public smartptr::MSharedPtr<T>
    {
    protected:
        MNode<T> * m_prev;
        MNode<T> * m_next;

    public:
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
    };

    template<typename T>
    class MRefList : public tlinkedlist::TLinkedList<MNode, T>
    {

    };
}