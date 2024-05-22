#ifndef _OWN_SET_H_
#define _OWN_SET_H_

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
    struct node;


public:
    using Compare = std::less<Key>;
    using Allocator = std::allocator<node>;
    using key_type = Key;
    using value_type = Key;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using key_compare = Compare;
    using value_compare = Compare;
    using allocator_type = Allocator;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = std::allocator_traits<Allocator>::pointer;
    using const_pointer = std::allocator_traits<Allocator>::const_pointer;
    using iterator = tree_iter;
    using const_iterator = const tree_iter;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_type = node;
    using alloc_traits = std::allocator_traits<allocator_type>;

public:
    set();
    template< std::input_iterator InputIt >
    set( InputIt first, InputIt last);
    set( const set& other );
    set( set&& other );
    set( std::initializer_list<value_type> init );
    ~set();

    set& operator=( const set& other );
    set& operator=( set&& other ) noexcept;
    set& operator=( std::initializer_list<value_type> ilist );

    allocator_type get_allocator() const { return m_alloc; }

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const noexcept;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() { return reverse_iterator(--end()); }
    const_reverse_iterator rbegin() const { return const_reverse_iterator(--end()); }
    const_reverse_iterator crbegin() const noexcept {{ return const_reverse_iterator(--end()); }}

    reverse_iterator rend() { return reverse_iterator(--begin()); }
    const_reverse_iterator rend() const { return reverse_iterator(--begin()); }
    const_reverse_iterator crend() const noexcept { return reverse_iterator(--begin()); }

    bool empty() const noexcept { return m_size == 0; }
    size_type size() const { return m_size; }
    size_type max_size() const { return alloc_traits::max_size(m_alloc); }

    void clear();

    std::pair<iterator, bool> insert( const value_type& value );
    std::pair<iterator, bool> insert( value_type&& value );
    // iterator insert( const_iterator pos, const value_type& value );
    // iterator insert( const_iterator pos, value_type&& value );
    // template< std::input_iterator InputIt >
    // void insert( InputIt first, InputIt last );
    // void insert( std::initializer_list<value_type> ilist );

    iterator erase( const_iterator pos );
    iterator erase( const_iterator first, const_iterator last );
    

    iterator find( const Key& key );
    const_iterator find( const Key& key ) const;


private:

    class tree_iter
    {
    private:
        friend class set;

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

        reference operator * () const noexcept { return static_cast<node*>( m_node )->key; } 
        pointer operator -> () const noexcept { return &static_cast<node*>( m_node )->key; }
        set::iterator& operator ++ () { m_node = set::next( m_node ); return *this; }
        set::iterator& operator -- () { m_node = set::prev( m_node ); return *this; }
        set::iterator operator ++ (int) { tree_iter tmp = *this; ++(*this); return tmp; } 
        set::iterator operator -- (int) { tree_iter tmp = *this; --(*this); return tmp; } 
        bool operator == ( const tree_iter& other ) { return m_node == other.m_node; }
        bool operator != ( const tree_iter& other ) { return !(*this == other.m_node); }
    };

    struct base_node
    {
        char height;
        base_node* left;
        base_node* right;
        base_node* parent;

        friend class set;
    };

    struct node : base_node 
    {
        Key key;

        node() = default;
        node( const node& other ) = delete;
        node( const Key& value ) : key(value) {}

        template< class... Args >
        node( Args&& ...args ) : key( std::forward<Args>( args )... ) {}

        friend class set;
    };

    //members of class 
    base_node fake_node{ 1, &fake_node, &fake_node, &fake_node };
    node* m_root = nullptr;
    allocator_type m_alloc;
    size_type m_size = 0;

    // creating and destroying node methods
    node* create_node(const Key& key) noexcept { 
        node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, key);

        new_node->height = 1;
        new_node->left = nullptr;
        new_node->right = nullptr;
        new_node->parent = nullptr;

        return new_node;
    }

    void destroy_node(base_node* node) noexcept {
        alloc_traits::destroy(m_alloc, node);
        alloc_traits::deallocate(m_alloc, static_cast<set<Key>::node*>(node), 1);
    }

    // healping methods for AVL-tree
    static base_node* next( base_node* node ) noexcept;
    static base_node* prev( base_node* node ) noexcept;

    void recursive_clear( base_node* node ) noexcept;
    char height( base_node* node ) noexcept { return node ? node->height : 0;}
    void fix_height( base_node* node ) noexcept;
    int balance_factor( base_node* node ) noexcept { return height(node->right) - height(node->left); }

    base_node* left_rotate( iterator it ) noexcept;
    base_node* right_rotate( iterator it ) noexcept;

    // balancing method for insertion and deletion
    void balance( base_node* node ) noexcept;
};

template< class Key >
inline set<Key>::base_node* set<Key>::next( base_node* node ) noexcept
{
    if (node->right != nullptr)
    {
        node = node->right;
        while (node->left != nullptr) node = node->left;
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
inline set<Key>::base_node* set<Key>::prev( base_node* node ) noexcept
{
    if (node->left != nullptr)
    {
        node = node->left;
        while (node->right != nullptr) node = node->right;
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
inline void set<Key>::recursive_clear( set<Key>::base_node* node ) noexcept
{
    if (node == nullptr) return;
    recursive_clear(node->left);
    recursive_clear(node->right);
    destroy_node(node);
}

template< class Key >
inline void set<Key>::fix_height( set<Key>::base_node* node ) noexcept
{
    char hl = height(node->left);
    char hr = height(node->right);
    node->height = (hl > hr? hl : hr) + 1;
}

template< class Key >
inline set<Key>::base_node* set<Key>::left_rotate( set<Key>::iterator it ) noexcept
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

    // так как мы работает с АВЛ-деревом необходимо сохранить св-во
    fix_height(node);
    fix_height(right_node);

    return right_node;
}

template< class Key >
inline set<Key>::base_node* set<Key>::right_rotate( set<Key>::iterator it ) noexcept
{
    base_node* node = it.m_node;
    base_node* left_node = node->left;
    left_node->parent = node->parent;

    if (node->parent->left == node) node->parent->left = left_node;
    else node->parent->right = left_node;

    node->left = left_node->right;
    if (node->right!= nullptr) node->left->parent = node;
    
    left_node->right = node;
    node->parent = left_node;

    // так как мы работает с АВЛ-деревом необходимо сохранить св-во
    fix_height(node);
    fix_height(left_node);

    return left_node;
}

template< class Key > 
inline void set<Key>::balance( set<Key>::base_node* node ) noexcept
{
    while(node != &fake_node)
    {
        int left_height = height(node->left);
        int right_height = height(node->right);
        int p_height = std::max(left_height, right_height) + 1;

        if (p_height != node->height) node->height = p_height;

        int balance_factor = left_height - right_height;
        if (balance_factor == 2)
        {
            if (height(node->left->left) >= height(node->left->right)) right_rotate(node);
            else 
            {
                left_rotate(node->left);
                right_rotate(node);
            }
        }

        else if (balance_factor == -2)
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
inline set<Key>::set() : m_alloc(allocator_type()) {}

template< class Key >
template< std::input_iterator InputIt >
inline set<Key>::set( InputIt first, InputIt last )
{
    for (; first != last; ++first)
    {
        insert(*first);
    }
}

template< class Key >
inline set<Key>::set( const set<Key>& other )
{
    for (auto it = other.begin(); it != other.end; ++it)
        insert(*it);

    m_alloc = other.m_alloc;
}

template< class Key >
inline set<Key>::set( set<Key>&& other )
{
    m_root = other.m_root;
    m_size = other.m_size;
    m_alloc = std::move(other.m_alloc);

    other.m_root = nullptr;
    other.m_size = 0;
}

template< class Key >
inline set<Key>::set( std::initializer_list<set<Key>::value_type> init ) : m_alloc(allocator_type())
{
    for (auto it = init.begin(); it!= init.end(); ++it)
    {
        insert(*it);
    }
}

template< class Key >
inline set<Key>::~set()
{
    clear();
}

template< class Key >
inline set<Key>& set<Key>::operator=( const set& other )
{
    if (this != &other)
    {
        this->clear();

        for (auto it = other.begin(); it!= other.end(); ++it)
            insert(*it);
    }

    return *this;
}

template< class Key >
inline set<Key>& set<Key>::operator=( set&& other ) noexcept
{
    if (this == &other) return *this;
    this->clear();

    m_root = other.m_root;
    m_size = other.m_size;
    m_alloc = std::move(other.m_alloc);

    other.m_root = nullptr;
    other.m_size = 0;
}

template< class Key >
set<Key>& set<Key>::operator=( std::initializer_list<value_type> ilist )
{
    this->clear();

    for (auto it = ilist.begin(); it!= ilist.end(); ++it)
    {
        insert(*it);
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
    if (fake_node.left == nullptr) return iterator(fake_node.left);
    base_node* node = fake_node.left;

    while (node->right!= nullptr) node = node->right;
    return next(iterator(node).m_node);
}

template< class Key >
inline set<Key>::const_iterator set<Key>::end() const
{
    if (fake_node.left == nullptr) return const_iterator(fake_node.left);
    base_node* node = fake_node.left;

    while (node->right!= nullptr) node = node->right;
    return next(const_iterator(node).m_node);
}

template< class Key >
inline set<Key>::const_iterator set<Key>::cend() const noexcept
{
    if (fake_node.left == nullptr) return const_iterator(fake_node.left);
    base_node* node = fake_node.left;

    while (node->right!= nullptr) node = node->right;
    return next(const_iterator(node).m_node);
}

template< class Key >
inline void set<Key>::clear()
{
    recursive_clear(fake_node.left);
    fake_node.left = nullptr;
    m_size = 0;
}

template< class Key >
std::pair< typename set<Key>::iterator, bool> set<Key>::insert( const set<Key>::value_type& value )
{
    if (fake_node.left == &fake_node)
    {
        fake_node.left = create_node( value );
        ++m_size;
        return { iterator( fake_node.left ), true };
    }

    
    node* node = static_cast<set<Key>::node*>(fake_node.left);

    while (true)
    {
        if (value < static_cast<set<Key>::node*>(node)->key)
        {
            if (node->left == nullptr)
            {
                node->left = create_node( value );
                ++m_size;
                balance( node );
                return { iterator( node->left ), true };
            }
            node = static_cast<set<Key>::node*>(node->left);
        }

        else if (value > static_cast<set<Key>::node*>(node)->key)
        {
            if (node->right == nullptr)
            {
                node->right = create_node( value );
                ++m_size;
                balance( node );
                return { iterator( node->right ), true };
            }
            node = static_cast<set<Key>::node*>(node->right);
        }
        else return { iterator( node ), false };
    }
}

template< class Key >
std::pair< typename set<Key>::iterator, bool> set<Key>::insert( set<Key>::value_type&& value )
{
    if (fake_node.left == &fake_node)
    {
        fake_node.left = create_node( std::move(value) );
        ++m_size;
        return { iterator( fake_node.left ), true };
    }

    node* node = static_cast<set<Key>::node*>(fake_node.left);

    while (true)
    {
        if (value < static_cast<set<Key>::node*>(node)->key)
        {
            if (node->left == nullptr)
            {
                node->left = create_node( std::move(value) );
                ++m_size;
                balance( node );
                return { iterator( node->left ), true };
            }
            node = static_cast<set<Key>::node*>(node->left);
        }

        else if (value > static_cast<set<Key>::node*>(node)->key)
        {
            if (node->right == nullptr)
            {
                node->right = create_node( std::move(value) );
                ++m_size;
                balance( node );
                return { iterator( node->right ), true };
            }
            node = static_cast<set<Key>::node*>(node->right);
        }
        else return { iterator( node ), false };
    }
}

template< class Key >
inline set<Key>::iterator set<Key>::erase(const_iterator pos)
{
    node* node = static_cast<set::node*>(pos.m_node);
    if (node == nullptr) return set::end();

    base_node* to_delete = nullptr;
    if (node->left == nullptr || node->right == nullptr) to_delete = node;

    else 
    {
        to_delete = next(node);
        node->key = static_cast<set::node*>(to_delete)->key;
    }

    base_node* child = (to_delete->left != nullptr) ? to_delete->left : to_delete->right;

    if (child != nullptr) child->parent = to_delete->parent;
    if(to_delete->parent == nullptr) fake_node.left = child;
    else if (to_delete == to_delete->parent->left) to_delete->parent->left = child;
    else to_delete->parent->right = child;

    base_node* p_balance = to_delete->parent;
    destroy_node(to_delete);
    --m_size;

    if (p_balance != nullptr) balance(p_balance);

    return iterator(p_balance);
}

template< class Key >
inline set<Key>::iterator set<Key>::erase(const_iterator first, const_iterator last)
{
    auto it = first;

    for (; first != last; ++first)
    {
        it = erase(first);
    }

    return it;
}

template< class Key >
inline set<Key>::iterator set<Key>::find(const Key& key) 
{
    base_node* node = fake_node.left();
    while (node != nullptr)
    {
        if (key < static_cast<set::node*>(node)->key) node = node->left;
        else if (key > static_cast<set::node*>(node)->key) node = node->right;
        else return iterator(node);
    }
    return end();
}

template< class Key >
inline set<Key>::const_iterator set<Key>::find(const Key& key) const 
{
    if (find(key) == end()) return cend();
    else 
    {
        base_node* node = fake_node.left();

        while (node != nullptr)
        {
            if (key < static_cast<set::node*>(node)->key) node = node->left;
            else if (key > static_cast<set::node*>(node)->key) node = node->right;
            else return const_iterator(node);
        }
    }
}

#endif //!_OWN_SET_H_