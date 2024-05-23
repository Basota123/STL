#ifndef _OWN_SET_HPP_
#define _OWN_SET_HPP_

#include <algorithm>
#include <functional>
#include <memory>
#include <cstddef>
#include <concepts>
#include <iterator>
#include <utility>
#include <initializer_list>
#include <cmath>


template< class Key >
class set
{
private:
    class tree_iter;
    struct base_node;
    struct avl_node;


public:
    using key_type = Key;
    using value_type = Key;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using Compare = std::less<Key>;
    using reference = Key&;
    using const_reference = const Key&;
    using pointer = Key*;
    using const_pointer = const Key*;
    using iterator = tree_iter;
    using const_iterator = const tree_iter;
    

public:
    // constructors and destructor
    set() : fake_node(nullptr), m_size(0ull) {}
    template< std::input_iterator InputIt >
    set( InputIt first, InputIt last);
    set( const set& other );
    set( std::initializer_list<value_type> init );
    ~set() { clear(); }

    // assignment operators
    set& operator=( const set& other );
    set& operator=( std::initializer_list<value_type> ilist );

    // iterators
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const noexcept;

    // capacity
    bool empty() const noexcept { return m_size == 0ull; }
    size_type size() const { std::distance(begin(), end()); }

    // modifiers
    void clear();

    std::pair<iterator, bool> insert( const value_type& key );
    std::pair<iterator, bool> insert( value_type&& key );

    iterator erase( const_iterator pos );

    // lookup
    iterator find( const value_type& key );
    const_iterator find( const value_type& key ) const;


private:
    struct base_node
    {
        base_node* left;
        base_node* right;
        base_node* parent;
        char height;

        base_node() : left(nullptr), right(nullptr), parent(nullptr), height(0) {}
        base_node( base_node* p ) : left(nullptr), right(nullptr), parent(p), height(0) {}
    };

    struct avl_node : base_node
    {
        Key key;

        avl_node(const Key& _key, base_node* p) : key(_key), base_node(p) {}

        template< class... Args >
        avl_node(Args&&... args, base_node* p) : key(std::forward< Args >(args)...), base_node(p) {}
        friend class set;
    };

    class tree_iter
    {
    private:
        friend class set;
        friend struct base_node;

    public:
        using iterator_type = set::value_type;
        using value_type = iterator_type;
        using difference_type = ptrdiff_t;
        using reference = value_type&;
        using pointer = value_type*;
        using iterator_category = std::bidirectional_iterator_tag;

    private:
        tree_iter() = delete;
        tree_iter( base_node* node ) : m_node( node ) {}

        base_node* m_node;

    public:
        tree_iter( const tree_iter& other ) : m_node( other.m_node ) {}

        reference operator * () const noexcept { return static_cast<avl_node*>( m_node )->key; } 
        pointer operator -> () const noexcept { return &static_cast<avl_node*>( m_node )->key; }
        tree_iter& operator ++ () { m_node = set::next( m_node ); return *this; }
        tree_iter& operator -- () { m_node = set::prev( m_node ); return *this; }
        tree_iter operator ++ (int) { tree_iter tmp = *this; ++(*this); return tmp; } 
        tree_iter operator -- (int) { tree_iter tmp = *this; --(*this); return tmp; } 
        bool operator == ( const tree_iter& other ) { return m_node == other.m_node; }
        bool operator != ( const tree_iter& other ) { return !(*this == other.m_node); }
    };

    base_node fake_node;
    size_type m_size;

    // healping methods for avl-tree
    void recursive_clear( base_node* node );

    char height( base_node* node );
    void fix_height( base_node* node );

    int balance_factor( base_node* node );
    void balance_tree( base_node* node );
    base_node* left_rotate(  iterator it );
    base_node* right_rotate( iterator it );

    // for iteration
    static base_node* next( base_node* node );
    static base_node* prev( base_node* node );
};

template< class Key >
template< std::input_iterator InputIt >
inline set<Key>::set( InputIt first, InputIt last )
{
    for (; first != last; ++first) insert(*first);
}

template< class Key >
inline set<Key>::set( std::initializer_list<value_type> init )
{
    for (const auto& k : init) insert(k);
}

template< class Key >
inline set<Key>::set( const set<Key>& other )
{
    for (auto it = other.begin(); it != other.end; ++it) insert(*it);
}

template< class Key >
inline void set<Key>::recursive_clear( base_node* node )
{
    if ( node != nullptr )
    {
        recursive_clear(node->left);
        recursive_clear(node->right);
        delete node;
    }

}

template< class Key >
inline char set<Key>::height( base_node* node )
{
    return node ? node->height : 0;
}

template< class Key >
inline void set<Key>::fix_height( base_node* node )
{
    char hl = height(node->left);
    char hr = height(node->right);
    
    node->height = (hl > hr? hl : hr) + 1;
}

template< class Key >
inline int set<Key>::balance_factor( base_node* node )
{
    return static_cast<int>(height(node->right)) - static_cast<int>(height(node->left));
}

template< class Key >
inline void set<Key>::balance_tree( base_node* node )
{
    while (node != &fake_node)
    {   
        int left_height = height(node->left);
        int right_height = height(node->right);
        int p_height = std::max(left_height, right_height) + 1;
        int balance = left_height - right_height;

        if (p_height != node->height) node->height = p_height;
        if (balance == 2)
        {
            if (height(node->left->left) >= height(node->left->right)) right_rotate(node);
            else 
            {
                left_rotate(node->left);
                right_rotate(node);
            }
        }

        else if (balance == -2)
        {
            if (height(node->right->right) >= height(node->right->left)) left_rotate(node);
            else 
            {
                right_rotate(node->right);
                left_rotate(node);
            }
        }

        node = node->parent;
    }
}

template< class Key >
inline set<Key>::base_node* set<Key>::left_rotate( set<Key>::iterator it )
{
    base_node* node = it.m_node;
    base_node* right_node = node->right;

    right_node->parent = node->parent;

    if (node->parent->right == node) node->parent->right = right_node;
    else node->parent->left = right_node;

    node->right = right_node->left;

    if (right_node->left != nullptr) right_node->left->parent = node;
    right_node->left = node;

    node->parent = right_node;

    fix_height(node);
    fix_height(right_node);

    return right_node;
}

template< class Key >
inline set<Key>::base_node* set<Key>::right_rotate( set<Key>::iterator it )
{
    base_node* node = it.m_node;
    base_node* left_node = node->left;

    left_node->parent = node->parent;

    if (node->parent->left == node) node->parent->left = left_node;
    else node->parent->right = left_node;

    node->left = left_node->right;

    if (left_node->right != nullptr) left_node->right->parent = node;
    left_node->right = node;

    node->parent = left_node;

    fix_height(node);
    fix_height(left_node);

    return left_node;
}

template< class Key >
inline set<Key>::base_node* set<Key>::next( base_node* node )
{
    if (node->right != nullptr) 
    {
        node = node->right;
        while (node->left != nullptr) node = node->left;

        return node;
    }

    else 
    {
        base_node* parent_node = node->parent;
        while (parent_node != nullptr && node == parent_node->right) 
        {
            node = parent_node;
            parent_node = parent_node->parent;
        }
        return parent_node;
    }
}

template< class Key >
inline set<Key>::base_node* set<Key>::prev( base_node* node )
{
    if (node->left != nullptr)
    {
        node = node->left;
        while (node->right != nullptr) node = node->right;

        return node;
    }

    else 
    {
        base_node* parent_node = node->parent;
        while (parent_node != nullptr && node == parent_node->left) 
        {
            node = parent_node;
            parent_node = parent_node->parent;
        }
        return parent_node;
    }
}

template< class Key >
inline set<Key>::iterator set<Key>::begin()
{
    if (fake_node.left == nullptr) return end();
    
    base_node* node = fake_node.left;
    while (node->left != nullptr) node = node->left;
    return iterator(node);
}

template< class Key >
inline set<Key>::const_iterator set<Key>::begin() const
{
    if (fake_node.left == nullptr) return end();
    
    base_node* node = fake_node.left;
    while (node->left != nullptr) node = node->left;
    return const_iterator(node);
}

template< class Key >
inline set<Key>::const_iterator set<Key>::cbegin() const noexcept
{
    if (fake_node.left == nullptr) return cend();
    
    base_node* node = fake_node.left;
    while (node->left != nullptr) node = node->left;
    return const_iterator(node);
}


template< class Key >
inline set<Key>::iterator set<Key>::end()
{
    if (fake_node.left == nullptr) return iterator(nullptr);
    
    base_node* node = fake_node.left;
    while (node->right != nullptr) node = node->right;
    return iterator(next(node));
}

template< class Key >
inline set<Key>::const_iterator set<Key>::end() const
{
    if (fake_node.left == nullptr) return iterator(nullptr);
    
    base_node* node = fake_node.left;
    while (node->right != nullptr) node = node->right;
    return const_iterator(next(node));
}

template< class Key >
inline set<Key>::const_iterator set<Key>::cend() const noexcept
{
    if (fake_node.left == nullptr) return iterator(nullptr);
    
    base_node* node = fake_node.left;
    while (node->right != nullptr) node = node->right;
    return const_iterator(next(node));
}

template< class Key >
inline void set<Key>::clear()
{
    recursive_clear(fake_node.left);
    fake_node.left = nullptr;
    m_size = 0;
}

template< class Key >
inline std::pair<typename set<Key>::iterator, bool> set<Key>::insert( const set<Key>::value_type& key )
{
    if (fake_node.left == nullptr)
    {
        fake_node.left = new avl_node(key, &fake_node);
        ++m_size;
        return std::make_pair(iterator(fake_node.left), true);
    }

    base_node* node = fake_node.left;
    while (true)
    {
        if (key < static_cast<avl_node*>(node)->key)
        {
            if (node->left == nullptr)
            {
                node->left = new avl_node(key, node);
                ++m_size;
                balance_tree(node);
                return std::make_pair(iterator(node->left), true);
            }
            node = node->left;
        }

        else if (static_cast<avl_node*>(node)->key < key)
        {
            if (node->right == nullptr)
            {
                node->right = new avl_node(key, node);
                ++m_size;
                balance_tree(node);
                return std::make_pair(iterator(node->right), true);
            }
            node = node->right;
        }
        else return std::make_pair(iterator(node), false);
    }

}

template< class Key >
inline std::pair<typename set<Key>::iterator, bool> set<Key>::insert( set<Key>::value_type&& key )
{
    if (fake_node.left == nullptr)
    {
        fake_node.left = new avl_node(std::move(key), &fake_node);
        ++m_size;
        return std::make_pair(iterator(fake_node.left), true);
    }

    base_node* node = fake_node.left;
    while (true)
    {
        if (key < static_cast<avl_node*>(node)->key)
        {
            if (node->left == nullptr)
            {
                node->left = new avl_node(std::move(key), node);
                ++m_size;
                balance_tree(node);
                return std::make_pair(iterator(node->left), true);
            }
            node = node->left;
        }

        else if (static_cast<avl_node*>(node)->key < key)
        {
            if (node->right == nullptr)
            {
                node->right = new avl_node(std::move(key), node);
                ++m_size;
                balance_tree(node);
                return std::make_pair(iterator(node->right), true);
            }
            node = node->right;
        }
        else return std::make_pair(iterator(node), false);
    }

}

template< class Key >
inline set<Key>::iterator set<Key>::erase(const_iterator pos)
{
    avl_node* node = static_cast<avl_node*>(pos.m_node);
    if (node == nullptr) return end();

    base_node* to_delete = nullptr;
    if (node->left == nullptr || node->right == nullptr) to_delete = node;

    else 
    {
        to_delete = next(node);
        node->key = static_cast<avl_node*>(to_delete)->key;
    }

    base_node* child = (to_delete->left != nullptr) ? to_delete->left : to_delete->right;

    if (child != nullptr) child->parent = to_delete->parent;
    if(to_delete->parent == nullptr) fake_node.left = child;
    else if (to_delete == to_delete->parent->left) to_delete->parent->left = child;
    else to_delete->parent->right = child;

    base_node* p_balance = to_delete->parent;
    delete to_delete;
    --m_size;

    if (p_balance != nullptr) balance_tree(p_balance);

    return iterator(p_balance);
}

template< class Key >
inline set<Key>::iterator set<Key>::find(const Key& key) 
{
    base_node* node = fake_node.left;
    while (node != nullptr)
    {
        if (key < static_cast<avl_node*>(node)->key) node = node->left;
        else if (key > static_cast<avl_node*>(node)->key) node = node->right;
        else return iterator(node);
    }
    return end();
}

template< class Key >
inline set<Key>::const_iterator set<Key>::find(const Key& key) const 
{
    base_node* node = fake_node.left;
    while (node != nullptr)
    {
        if (key < static_cast<avl_node*>(node)->key) node = node->left;
        else if (key > static_cast<avl_node*>(node)->key) node = node->right;
        else return const_iterator(node);
    }
    return cend();
}

template< class Key >
inline set<Key>& set<Key>::operator=( const set& other )
{
    for (auto it = other.begin(); it != other.end(); ++it) insert(*it);
}

template< class Key >
inline set<Key>& set<Key>::operator=( std::initializer_list<value_type> ilist )
{
    for (const auto& it : ilist) insert(it); 
}



#endif //!_OWN_SET_HPP_