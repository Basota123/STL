#ifndef _LIST_HPP_
#define _LIST_HPP_


#ifndef STL_HEADER_CXX20
#define STL_HEADER_CXX20

#include <iterator>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <limits>


template < class T, class Allocator = std::allocator<T> >
class list
{
private:
	struct base_node;
	struct node;
	class twindiriter;

public:
	using value_type = T;
	using allocator_type = Allocator;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = std::allocator_traits<Allocator>::pointer;
	using const_pointer = std::allocator_traits<Allocator>::const_pointer;
	using iterator = twindiriter;
	using const_iterator = const twindiriter;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

public:
	// ctors and dctor
	list();
	explicit list( const Allocator& alloc );
	list( size_type count,
      	const T& value,
      	const Allocator& alloc = Allocator() );
	explicit list( size_type count,
        const Allocator& alloc = Allocator() );
	template< std::input_iterator InputIt >
	list( InputIt first, InputIt last,
      	const Allocator& alloc = Allocator() );
	list( const list& other );
	list( const list& other, const Allocator& alloc );
	list( list&& other );
	list( list&& other, const Allocator& alloc );
	list( std::initializer_list<T> init,
      	const Allocator& alloc = Allocator() );
	~list();

	// assignment operator
	list& operator=( const list& other );
	list& operator=( list&& other ) noexcept;
	list& operator=( std::initializer_list<value_type> ilist );

	// assign methods
	void assign( size_type count, const T& value );
	template< std::input_iterator InputIt >
	void assign( InputIt first, InputIt last );
	void assign( std::initializer_list<T> ilist );

	allocator_type get_allocator() const noexcept { return m_alloc; }

	// element access
	reference front() { return m_head->value; }
	const_reference front() const { return m_head->value; }

	reference back() { return m_tail->value; }
	const_reference back() const { return m_tail->value; }

	// iterators
	iterator begin() noexcept { return fake_node.next; }
	const_iterator begin() const noexcept { return const_iterator(const_cast<base_node*>(fake_node.next)); }
	const_iterator cbegin() const noexcept { return const_iterator(const_cast<base_node*>(fake_node.next)); }

	iterator end() noexcept { return &fake_node; }
	const_iterator end() const noexcept { return const_iterator(const_cast<base_node*>(&fake_node)); }
	const_iterator cend() const noexcept { return const_iterator(const_cast<base_node*>(&fake_node)); }

	reverse_iterator rbegin() noexcept { return reverse_iterator(&fake_node); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(const_cast<base_node*>(&fake_node)); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(const_cast<base_node*>(&fake_node)); }

	reverse_iterator rend() noexcept { return reverse_iterator(fake_node.next); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(const_cast<base_node*>(fake_node.next)); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(const_cast<base_node*>(fake_node.next)); }

	// capacity
	bool empty() const noexcept { return size() == 0; }
	size_type size() const noexcept { return std::distance(begin(), end()); }
	size_type max_size() const noexcept { return node_allocator_traits::max_size(m_alloc); }

	// modifiers
	void clear();

	iterator insert( const_iterator pos, const T& value );
	iterator insert( const_iterator pos, T&& value );
	iterator insert( const_iterator pos, size_type count, const T& value );
	template< std::input_iterator InputIt >
	iterator insert( const_iterator pos, InputIt first, InputIt last );
	iterator insert( const_iterator pos, std::initializer_list<T> ilist );

	template< class... Args >
	iterator emplace( const_iterator pos, Args&&... args );

	iterator erase( const_iterator pos );
	iterator erase( const_iterator first, const_iterator last );

	void push_back( const T& value );
	void push_back( T&& value );

	template< class... Args >
	reference emplace_back( Args&&... args );

	void pop_back();

	void push_front( const T& value );
	void push_front( T&& value );

	template< class... Args >
	reference emplace_front( Args&&... args );

	void pop_front();

	void resize( size_type count );
	void resize( size_type count, const value_type& value );

	void swap( list& other ) noexcept;

	// operations
	void merge( list& other );
	void merge( list&& other );
	template< class Compare >
	void merge( list& other, Compare comp );
	template< class Compare >
	void merge( list&& other, Compare comp );

	void splice( const_iterator pos, list& other );
	void splice( const_iterator pos, list&& other );
	void splice( const_iterator pos, list& other, const_iterator it );
	void splice( const_iterator pos, list&& other, const_iterator it );
	void splice( const_iterator pos, list& other,
        const_iterator first, const_iterator last);
	void splice( const_iterator pos, list&& other,
        const_iterator first, const_iterator last);

	size_type remove( const T& value );
	template< class UnaryPredicate >
	size_type remove_if( UnaryPredicate p );

	void reverse();

	size_type unique();
	template< class BinaryPredicate >
	size_type unique( BinaryPredicate p );	

	void sort();
	template< class Compare >
	void sort( Compare comp );


private:
	class twindiriter
	{
	private:
		friend class list;

	public:
		using iterator_type = list::value_type;
		using value_type = iterator_type;
		using difference_type = ptrdiff_t;
		using reference = value_type&;
		using pointer = value_type*;
		using iterator_category = std::bidirectional_iterator_tag;

	private:
		twindiriter() = default;
		twindiriter(base_node* node) : m_node(node) { }

		base_node* m_node;

	public:
		twindiriter(const twindiriter& other) : m_node(other.m_node) { }

		reference operator * () const noexcept { return static_cast<node*>(m_node)->value; }
		pointer operator -> () const noexcept { return &static_cast<node*>(m_node)->value; }

		twindiriter& operator ++ () noexcept { m_node = m_node->next; return *this; }
		twindiriter operator ++ (int) noexcept { twindiriter tmp = *this; m_node = m_node->next; return tmp; }
		twindiriter& operator -- () noexcept { m_node = m_node->prev; return *this; }
		twindiriter operator -- (int) noexcept { twindiriter tmp = *this; m_node = m_node->prev; return tmp; }

		bool operator == (const twindiriter& other) const noexcept { return m_node == other.m_node; }
		bool operator!= (const twindiriter& other) const noexcept { return m_node != other.m_node; }
	};

	struct base_node
	{
		base_node* next;
		base_node* prev;

		friend class list;
	};

	struct node : base_node
	{
		T value;
		node() = default;
		node(const node&) = delete;
		node(const T& _val) : value(_val) { }

		template< class... Args>
		node(Args&& ...args) : value(std::forward<Args>(args)...) {}
	};

	void fill_list(size_type count, const T& value)
	{
		for (size_t i = 0; i < count; ++i)
		{
			node* new_node = create_node(value);

			if (m_head == nullptr)
			{
				m_head = new_node;
				m_tail = new_node;
				m_head->next = &fake_node;
				m_head->prev = &fake_node;
				fake_node.next = m_head;
				fake_node.prev = m_tail;
			}

			else
			{
				m_tail->next = new_node;
				new_node->prev = m_tail;
				new_node->next = &fake_node;
				fake_node.prev = new_node;
				m_tail = new_node;
			}
		}
	}

	template< std::input_iterator InputIt >
	void fill_list(InputIt first, InputIt last)
	{
		for (; first != last; ++first)
		{
			node* new_node = create_node(*first);

			if (m_head == nullptr)
			{
				m_head = new_node;
				m_tail = new_node;
				m_head->next = &fake_node;
				m_head->prev = &fake_node;
				fake_node.next = m_head;
				fake_node.prev = m_tail;
			}

			else
			{
				m_tail->next = new_node;
				new_node->prev = m_tail;
				new_node->next = &fake_node;
				fake_node.prev = new_node;
				m_tail = new_node;
			}
			
		}
	}

	node* create_node(const T& value)
	{
		node* new_node = node_allocator_traits::allocate(m_alloc, 1);
		node_allocator_traits::construct(m_alloc, new_node, value);

		return new_node;
	}

	void destroy_node(node* node)
	{
		node_allocator_traits::destroy(m_alloc, node);
		node_allocator_traits::deallocate(m_alloc, node, 1);
	}

	using node_allocator = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
	using node_allocator_traits = typename std::allocator_traits<Allocator>::template rebind_traits<node>;
	
	node_allocator m_alloc;
	node* m_head = nullptr;
	node* m_tail = nullptr;
	base_node fake_node{ &fake_node, &fake_node };
};

template <class T, class Allocator>
inline list<T, Allocator>::list()
{
	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
}

template <class T, class Allocator>
inline list<T, Allocator>::list(const Allocator& alloc) : m_alloc(alloc)
{
	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
}

template <class T, class Allocator>
inline list<T, Allocator>::list(size_type count, const T& value, const Allocator& alloc) : m_alloc(alloc)
{
	fill_list(count, value);
}

template <class T, class Allocator> 
inline list<T, Allocator>::list(size_type count, const Allocator& alloc) : m_alloc(alloc)
{
	fill_list(count, T());
}

template <class T, class Allocator>
template <std::input_iterator InputIt>
inline list<T, Allocator>::list(InputIt first, InputIt last, const Allocator& alloc) : m_alloc(alloc)
{
	fill_list(first, last);
}

template<class T, class Allocator>
inline list<T, Allocator>::list(const list& other)
{
	m_alloc = node_allocator_traits::select_on_container_copy_construction(other.m_alloc);
	fill_list(other.begin(), other.end());
}

template <class T, class Allocator>
inline list<T, Allocator>::list(const list& other, const Allocator& alloc) : m_alloc(alloc)
{
	fill_list(other.begin(), other.end());
}

template <class T, class Allocator>
inline list<T, Allocator>::list(list&& other)
{
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_alloc = node_allocator_traits::select_on_container_copy_construction(std::move(other.m_alloc));

	fake_node.next = m_head;
	fake_node.prev = m_tail;

	m_head->prev = &fake_node;
	m_tail->next = &fake_node;

	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;
}

template <class T, class Allocator>
inline list<T, Allocator>::list(list&& other, const Allocator& alloc) : m_alloc(alloc)
{
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_alloc = std::move(other.m_alloc);

	fake_node.next = m_head;
	fake_node.prev = m_tail;

	m_head->prev = &fake_node;
	m_tail->next = &fake_node;

	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;
}

template <class T, class Allocator>
inline list<T, Allocator>::list(std::initializer_list<T> init, const Allocator& alloc) : m_alloc(alloc)
{
	fill_list(init.begin(), init.end());
}

template <class T, class Allocator>
inline list<T, Allocator>::~list()
{
	clear();
}

template <class T, class Allocator>
inline list<T, Allocator>& list<T, Allocator>::operator=( const list& other )
{
	if (this == &other) return *this;
	else
	{
		clear();
		if (node_allocator_traits::propagate_on_container_copy_assignment)
			m_alloc = other.m_alloc;
		
		fill_list(other.begin(), other.end());
		return *this;
	}
}

template <class T, class Allocator>
inline list<T, Allocator>& list<T, Allocator>::operator=( list&& other ) noexcept
{
	if (this == &other) return *this;
	else
	{
		clear();
		if (node_allocator_traits::propagate_on_container_move_assignment)
			m_alloc = std::move(other.m_alloc);
		
		m_head = other.m_head;
		m_tail = other.m_tail;

		fake_node.next = m_head;
		other.m_tail = static_cast<node*>(&other.fake_node);
		other.m_head->prev = &other.fake_node;
		other.m_tail->next = &other.fake_node;

		return *this;
	}
}

template <class T, class Allocator>
inline list<T, Allocator>& list<T, Allocator>::operator=( std::initializer_list<value_type> ilist )
{
	clear();
	fill_list(ilist.begin(), ilist.end());
	return *this;
}

template <class T, class Allocator>
inline void list<T, Allocator>::assign(size_type count, const T& value)
{
	fill_list(count, value);
}

template <class T, class Allocator>
template <std::input_iterator InputIt>
inline void list<T, Allocator>::assign(InputIt first, InputIt last)
{
	fill_list(first, last);
}

template <class T, class Allocator>
inline void list<T, Allocator>::assign(std::initializer_list<T> ilist)
{
	fill_list(ilist.begin(), ilist.end());
}

template<class T, class Allocator>
void list<T, Allocator>::clear()
{
	node* current = m_head;
	while (current != &fake_node)
	{
		node* next = static_cast<node*>(current->next);
		destroy_node(current);
		current = next;
	}

	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
	fake_node.next = &fake_node;
	fake_node.prev = &fake_node;
}














#endif // !STL_HEADER_CXX20
#endif // !_LIST_HPP_
