#pragma once

#include "../reflist/reflist.h"
#include "../smartptr/smartptr.h"
#include <optional>

namespace mpaop::smartptr::uoleaf
{
    template<typename T>
    class MUnorderedLeaf : public smartptr::MSharedPtr<T>
    {
    private:
        const int32_t m_key;
        reflist::MRefList<T> m_list;

        MUnorderedLeaf<T> * m_parent;
        MUnorderedLeaf<T> * m_left;
        MUnorderedLeaf<T> * m_right;

        void insertAs(MUnorderedLeaf<T> * parent, MUnorderedLeaf<T> ** parentlr)
        {
            m_parent = parent;
            * parentlr = this;
        }

        void insertParent(MUnorderedLeaf<T> * parent)
        {
            m_parent = parent;
        }

        void insertAsLeftLeaf(MUnorderedLeaf<T> * leaf)
        {
            leaf->insertParent(this);
            this->m_left = leaf;
        }

        void insertAsRightLeaf(MUnorderedLeaf<T> * leaf)
        {
            leaf->insertParent(this);
            this->m_right = leaf;
        }

        MUnorderedLeaf<T> * findDescendantOfRightLeafWithSmallestKey()
        {
            if(! m_right->m_left)
            {
                return m_right;
            }

            MUnorderedLeaf<T> * descendant = m_right;
            do
            {
                descendant = descendant->m_left;
            } while (descendant->m_left);

            return descendant;
        }

        MUnorderedLeaf<T> * findDescendantOfLeftLeafWithLargestKey()
        {
            if(! m_left->m_right)
            {
                return m_left;
            }

            MUnorderedLeaf<T> * descendant = m_left;
            do
            {
                descendant = descendant->m_right;
            } while (descendant->m_rigth);

            return descendant;
        }

    public:

        void countChildren(MUnorderedLeaf<T> * leaf, std::optional<int32_t> & outCount)
        {
            if(! leaf) return;
            if(! outCount.has_value())
            {
                outCount = 0;
            }

            if(leaf->m_left)
            {
                ++outCount.value();
                countChildren(leaf->m_left, outCount);
            }

            if (leaf->m_right)
            {
                ++outCount.value();
                countChildren(leaf->m_right, outCount);
            }
        }

        void insert(MUnorderedLeaf<T> * leaf)
        {
            if(! leaf) return;
            else if(leaf->m_key == this->m_key) return;

            // should this leaf be to the left of parent?
            if(this->m_key < leaf->m_key)
            {
                // if we do not have any children simply insert
                if(! this->m_left)
                {
                    insertAsLeftChild(leaf);
                    return;
                }
                else
                {
                    this->m_left->insert(leaf);
                }
            }
            else
            {
                // if we do not have any children simply insert
                if(! this->m_right)
                {
                    insertAsRightChild(leaf);
                    return;
                }
                else
                {
                    this->m_right->insert(leaf);
                }
            }
        }

        template<class... Args>
        MUnorderedLeaf<T> * insert(const int32_t key, Args... args)
        {
            if(key == this->m_key)
            {
                m_list.Push(args...);
                return this;
            }

            // should this leaf be to the left of parent?
            if(key < this->m_key)
            {
                // if parent does not have any children simply insert
                if(! this->m_left)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(key, args...);
                    this->insertAsLeftLeaf(leaf);
                    return leaf;
                }
                else
                {
                    return this->m_left->insert(key, args...);
                }
            }
            else
            {
                // if parent does not have any children simply insert
                if(! this->m_right)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(key, args...);
                    this->insertAsRightLeaf(leaf);
                    return leaf;
                }
                else
                {
                    return this->m_right->insert(key, args...);
                }
            }
        }

        void removeSelf()
        {
            // if we have a parent and we have children
            if(m_parent && (m_left || m_right))
            {
                
            }
            // if we only have a parent
            else if(m_parent)
            {
                if(this->m_parent->m_left == this) this->m_parent->m_left = nullptr;
                else this->m_parent->m_right = nullptr;
            }
            // if we only have children
            else if(m_left || m_right)
            {
                // if we have multiple children
                if(m_left && m_right)
                {

                }
                // if we only have one child
                if(m_left)
                {
                    m_left->m_parent = nullptr;
                }
                else
                {
                    m_right->m_parent = nullptr;
                }
            }

            this->m_parent = nullptr;
            this->m_left = nullptr;
            this->m_right = nullptr;
        }

        template<class... Args>
        MUnorderedLeaf<T>(const int32_t & key, Args... args) : 
            MSharedPtr<T>(args...),
            m_key(key), m_list(reflist::MRefList<T>()), 
            m_parent(nullptr), m_left(nullptr), m_right(nullptr) {}

        MUnorderedLeaf<T>(const MUnorderedLeaf<T> & leaf) : 
            MSharedPtr<T>(leaf), 
            m_key(leaf->m_key), m_list(leaf->m_list), 
            m_parent(leaf->m_parent), m_left(leaf->m_left), m_right(leaf->m_right) {}

        MUnorderedLeaf<T>(const MSharedPtr<T> & sPtr) : MSharedPtr<T>(sPtr) {}

        virtual ~MUnorderedLeaf<T>()
        {
            removeSelf();
            std::cout << "Removed Self in leaf\n";
        }
        
        // deleted constructors
        MUnorderedLeaf<T>() = delete;
        MUnorderedLeaf<T>(MUnorderedLeaf &&) = delete;
    };
}