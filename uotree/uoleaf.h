#include "../reflist/reflist.h"
#include "../smartptr/smartptr.h"

namespace mpaop
{
    template<typename T>
    class MUnorderedLeaf : smartptr::MSharedPtr<T>
    {
    private:
        const int32_t m_key;
        reflist::MRefList<T> m_list;

        MUnorderedLeaf<T> * m_parent;
        MUnorderedLeaf<T> * m_left;
        MUnorderedLeaf<T> * m_right;

        void insertAs(MUnorderedLeaf<T> ** parent, MUnorderedLeaf<T> ** parentlr)
        {
            m_parent = * parent;
            * parentlr = this;
        }

        void insertAsLeftChild(MUnorderedLeaf<T> ** parent)
        {
            insertAs(*parent, & (* parent)->m_left);
        }

        void insertAsRightChild(MUnorderedLeaf<T> ** parent)
        {
            insertAs(parent, & (* parent)->m_right);
        }
    public:

        void insert(MUnorderedLeaf<T> ** parent)
        {
            if(!parent) return;

            MUnorderedLeaf<T> * p = * parent;

            if(p->m_key == this->m_key) return;

            // should this leaf be to the left of parent?
            if(this->m_key < p->m_key)
            {
                // if parent does not have any children simply insert
                if(! p->m_left)
                {
                    insertAsLeftChild(parent);
                    return;
                }
                else
                {
                    insert(& p->m_left);
                }
            }
            else
            {
                // if parent does not have any children simply insert
                if(! p->m_right)
                {
                    insertAsRightChild(parent);
                    return;
                }
                else
                {
                    insert(& p->m_right);
                }
            }
        }

        template<class... Args>
        void insert(const int32_t key, Args... args)
        {
            if(key == this->m_key)
            {
                m_list.Push(args...);
                return;
            }

            // should this leaf be to the left of parent?
            if(key < this->m_key)
            {
                // if parent does not have any children simply insert
                if(! this->m_left)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(args...);
                    leaf->insertAsLeftChild(& this);
                    return;
                }
                else
                {
                    this->m_left->insert(key, args...);
                    return;
                }
            }
            else
            {
                // if parent does not have any children simply insert
                if(! this->m_right)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(args...);
                    leaf->insertAsRightChild(& this);
                    return;
                }
                else
                {
                    this->m_right->insert(key, args...);
                    return;
                }
            }
        }

        void removeLeaf()
        {
            if(m_parent)
            {

            }
            else
            {
                
            }
        }

        MUnorderedLeaf(const int32_t & key) : 
            m_key(key), m_list(reflist::MRefList<T>()), 
            m_parent(nullptr), m_left(nullptr), m_right(nullptr) {}
        
        // deleted constructors
        MUnorderedLeaf() = delete;
        MUnorderedLeaf(const MUnorderedLeaf &) = delete;
        MUnorderedLeaf(MUnorderedLeaf &&) = delete;
    };
}