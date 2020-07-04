#pragma once

#include "reflist.h"

namespace mpaop::linkedtree
{
    template<typename T> class MLinkedTree;

    template <typename T>
    class MLinkedLeaf : public reflist::MNode<T>
    {
    private:
        MLinkedLeaf<T> * m_parent;
        MLinkedTree<T> m_children;

    public:
        template<class... Args>
        MLinkedLeaf<T>(MLinkedLeaf<T> * prev, MLinkedLeaf<T> * next, MLinkedLeaf<T> * parent, Args... args) : reflist::MNode<T>(prev, next, args...), m_parent(parent), m_children()
        {
            if(m_parent) m_parent->pushChildNode(this);
        }

        MLinkedLeaf<T>(MLinkedLeaf<T> & leaf) : reflist::MNode<T>(leaf), m_parent(leaf.m_parent), m_children(leaf.m_children) {}

        virtual ~MLinkedLeaf<T>()
        {
            for(MLinkedLeaf<T> & leaf : m_children)
            {
                leaf.m_parent = nullptr;
            }
        }

        auto Children() const & -> const MLinkedTree<T> & { return m_children; }
        auto Prev() & -> MLinkedLeaf<T> * { return (MLinkedLeaf<T> *)this->m_prev; }
        auto Next() & -> MLinkedLeaf<T> * { return (MLinkedLeaf<T> *)this->m_next; }

        MLinkedLeaf<T> * getChild(const uint32_t & idx) 
        {
            if(m_children.Size() <= idx) throw std::out_of_range("LOG: index exceeds number of children!");
            return this->m_children.getNode(idx);
        }

        template<class... Args>
        MLinkedLeaf<T> * pushChild(Args... args)
        {
            MLinkedLeaf<T> * leaf = m_children.Push(args...);
            leaf->m_parent = this;
            return leaf;
        }

        MLinkedLeaf<T> * pushChildNode(MLinkedLeaf<T> * leaf)
        {
            m_children.PushNode(* leaf);
            leaf->m_parent = this;
            return leaf;
        }

        template<class... Args>
        MLinkedLeaf<T> * insertChild(const uint32_t & idx, Args... args)
        {
            MLinkedLeaf<T> * leaf = m_children.Insert(idx, args...);
            leaf->m_parent = this;
            return leaf;
        }

        void popChild()
        {
            m_children.Pop();
        }

        void removeChild(const uint32_t & idx)
        {
            m_children.Remove(idx);
        }

        void copyChild(const uint32_t & idx, MLinkedLeaf<T> & other)
        {
            MLinkedLeaf<T> *child = this->getChild(idx);
            other.m_children.PushNode(*child);
        }

        void moveChild(const uint32_t & idx, MLinkedLeaf<T> & other)
        {
            copyChild(idx, other);
            m_children.Remove(idx);
        }
    };

    template<typename T>
    class MLinkedTree : public tlinkedlist::TLinkedList<MLinkedLeaf, T>
    {
    public:
        MLinkedTree() : tlinkedlist::TLinkedList<MLinkedLeaf, T>() {}
        virtual ~MLinkedTree() {}
        // specialization
        template <class... Args>
        MLinkedLeaf<T> * Push(Args... args)
        {
            ++this->m_size;
            if (this->m_last == nullptr)
            {
                this->m_last = this->m_first = this->m_current = new MLinkedLeaf<T>(nullptr, nullptr, nullptr, args...);
                return this->m_last;
            }

            this->m_last = new MLinkedLeaf<T>(this->m_last, nullptr, nullptr, args...);

            return this->m_last;
        }

        template <class... Args>
        MLinkedLeaf<T> * Insert(const uint32_t & idx, Args... args)
        {
            if (idx > this->m_size) throw std::out_of_range("LOG: idx exceeds size of list");

            MLinkedLeaf<T> * res;

            if (idx == this->m_size) res = this->Push(args...);
            else if (idx == 0)
            {
                this->m_first = new MLinkedLeaf<T>(nullptr, this->m_first, nullptr, args...);
                res = this->m_first;
            }
            else
            {
                MLinkedLeaf<T> *node = this->getNode(idx);
                MLinkedLeaf<T> *newNode = new MLinkedLeaf<T>(node->Prev(), node, nullptr, args...);
                res = newNode;
            }

            ++this->m_size;
            return res;
        }        
    };
}