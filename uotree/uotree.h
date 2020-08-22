#pragma once

#include "../reflist/reflist.h"
#include "../smartptr/smartptr.h"
#include <optional>

namespace mpaop::smartptr::uotree
{

    template<typename T> class MUnorderedTree;

    template<typename T>
    class MUnorderedLeaf : public MSharedPtr<T>
    {
    private:
        int32_t m_key;
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
            std::cout << "Inserted parent key = " << parent->m_key << " val = " << *parent->m_ptr << std::endl;
            m_parent = parent;
        }

        void insertAsLeftLeaf(MUnorderedLeaf<T> * leaf)
        {
            printf("Inserting Self as parent address: %x\n", this);
            leaf->insertParent(this);
            this->m_left = leaf;
            printf("left post insert: %x\n", this->m_left);
        }

        void insertAsRightLeaf(MUnorderedLeaf<T> * leaf)
        {
            printf("Inserting Self as parent address: %x\n", this);
            leaf->insertParent(this);
            this->m_right = leaf;
        }

        MUnorderedLeaf<T> * findDescendantOfRightLeafLeastKey()
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

        MUnorderedLeaf<T> * findDescendantOfLeftLeafGreatestKey()
        {
            if(! m_left->m_right)
            {
                return m_left;
            }

            MUnorderedLeaf<T> * descendant = m_left;
            do
            {
                descendant = descendant->m_right;
            } while (descendant->m_right);

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

        MUnorderedLeaf<T> * createChildTree()
        {
            // return null or one leaf if unable to create a tree
            if(! m_right && ! m_left)
            {
                return nullptr;
            }
            else if(! m_right)
            {
                return m_left;
            }
            else if(! m_left)
            {
                return m_right;
            }

            std::optional<int32_t> lChildNum = 0, rChildNum = 0;
            countChildren(m_left, lChildNum);
            countChildren(m_right, rChildNum);

            // since we have less children on the left leaf, we bind it to the right leaf
            if(lChildNum < rChildNum)
            {
                MUnorderedLeaf<T> * leaf = findDescendantOfRightLeafLeastKey();
                leaf->insertAsLeftLeaf(m_left);
                return m_right;
            }
            // since we have less children on the right leaf, we bind it to the left leaf
            else
            {
                MUnorderedLeaf<T> * leaf = findDescendantOfLeftLeafGreatestKey();
                leaf->insertAsRightLeaf(m_right);
                return m_left;
            }
        }

        MUnorderedLeaf<T> * findLeaf(const int32_t & key)
        {
            if(this->m_key == key)
            {
                return this;
            }
            else if(this->m_key < key)
            {
                if(m_right) return m_right->findLeaf(key);
            }
            else
            {
                if(m_left) return m_left->findLeaf(key);
            }
            
            return nullptr;
        }
private:
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
                    insertAsLeftLeaf(leaf);
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
                    insertAsRightLeaf(leaf);
                    return;
                }
                else
                {
                    this->m_right->insert(leaf);
                }
            }
        }

public:
        template<class... Args>
        MUnorderedLeaf<T> insert(const int32_t key, Args... args)
        {
            if(key == this->m_key)
            {
                m_list.Push(args...);
                return * (this);
            }

            // should this leaf be to the left of parent?
            if(key < this->m_key)
            {
                // if parent does not have any children simply insert
                if(! this->m_left)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(key, args...);
                    printf("new leaf address: %x\n", leaf);
                    this->insertAsLeftLeaf(leaf);
                    return * leaf;
                }
                else
                {
                    std::cout << "Insert to left leaf\n";
                    return * this->m_left->insert(key, args...);
                }
            }
            else
            {
                // if parent does not have any children simply insert
                if(! this->m_right)
                {
                    MUnorderedLeaf<T> * leaf = new MUnorderedLeaf<T>(key, args...);
                    printf("new leaf address: %x\n", leaf);
                    this->insertAsRightLeaf(leaf);
                    return * leaf;
                }
                else
                {
                    std::cout << "Insert to right leaf\n";
                    return * this->m_right->insert(key, args...);
                }
            }
        }

        void removeSelf()
        {
            this->m_parent = nullptr;
            if(this->m_left)
            {
                std::cout << "Delete left leaf\n";
                this->m_left->m_parent = nullptr;
                delete this->m_left;
                this->m_left = nullptr;
            }
            if(this->m_right)
            {
                std::cout << "Delete right leaf\n";
                this->m_right->m_parent = nullptr;
                delete this->m_right;
                this->m_right = nullptr;
            }
        }

        template<class... Args>
        MUnorderedLeaf<T>(const int32_t & key, Args... args) : 
            MSharedPtr<T>(args...),
            m_key(key), m_list(reflist::MRefList<T>()), 
            m_parent(nullptr), m_left(nullptr), m_right(nullptr) {}

        MUnorderedLeaf<T>(const MUnorderedLeaf<T> & leaf) : 
            MSharedPtr<T>(leaf.m_ptr, leaf.m_refCount),
            m_key(leaf.m_key), m_list(leaf.m_list), 
            m_parent(leaf.m_parent), m_left(leaf.m_left), m_right(leaf.m_right) 
            {
                printf("copy constructor called. leaf address: %x\n", &leaf);
                printf("copy constructor called. myaddress: %x\n", this);
            }

        MUnorderedLeaf<T>(const MSharedPtr<T> & sPtr) : MSharedPtr<T>(sPtr) {}

        virtual ~MUnorderedLeaf<T>()
        {
            std::cout << "leaf destructor called\n";
            printf("my address: %x\n", this);
            if(m_parent)
            { 
                std::cout << "Parent: " << * m_parent->m_ptr << std::endl;
                printf("parent address: %x\n", this->m_parent);
            }
            removeSelf();
        }
        
        // deleted constructors
        MUnorderedLeaf<T>() = delete;

        friend class MUnorderedTree<T>;
    };

    template<class T>
    class MUnorderedTree
    {
    private:
        MUnorderedLeaf<T> m_root;

    public:
        template<class... Args>
        MUnorderedLeaf<T> insert(const int32_t & key, Args... args)
        {
            return m_root.insert(key, args...);
        }

        MUnorderedLeaf<T> * find(const int32_t & key)
        {
            return m_root.findLeaf(key);
        }

        bool remove(const int32_t & key)
        {
            MUnorderedLeaf<T> * leaf;

            if (m_root.m_key == key)
            {
                leaf = m_root.createChildTree();
                m_root = * leaf;
                return true;
            }

            leaf = m_root.findLeaf(key);
            if(! leaf) return false;

            MUnorderedLeaf<T> * cht = leaf->createChildTree();
            delete leaf;
            m_root.insert(cht);
            return true;
        }

        template<class... Args>
        MUnorderedTree<T>(const int32_t & key, Args... args) : m_root(key, args...)
        {

        }
        
        MUnorderedTree<T>() = delete;
        MUnorderedTree<T>(const MUnorderedLeaf<T> & leaf) = delete;
        MUnorderedTree<T>(const MUnorderedTree<T> & tree) = delete;
        MUnorderedTree<T> * operator =(const MUnorderedTree<T> & tree) = delete;

        virtual ~MUnorderedTree<T>()
        {

        }
    };
}