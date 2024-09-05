#ifndef _LIST_HPP_
#define _LIST_HPP_


#ifndef STL_HEADER_CXX20
#define STL_HEADER_CXX20

#include <iterator>
#include <iostream>
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

	iterator insert( const_iterator pos, const T& value ) { return insert_impl(pos, create_node(value)); }
	iterator insert( const_iterator pos, T&& value ) { return insert_impl(pos, create_node(std::move(value))); }
	iterator insert( const_iterator pos, size_type count, const T& value );
	template< std::input_iterator InputIt >
	iterator insert( const_iterator pos, InputIt first, InputIt last );
	iterator insert( const_iterator pos, std::initializer_list<T> ilist ) { return insert(pos, ilist.begin(), ilist.end()); }

	template< class... Args >
	iterator emplace( const_iterator pos, Args&&... args ) { return insert_impl(pos, create_node(std::forward<Args>(args)...)); }

	iterator erase( const_iterator pos );
	iterator erase( const_iterator first, const_iterator last );

	void push_back( const T& value ) { insert_impl(end(), create_node(value)); }
	void push_back( T&& value ) { insert_impl(end(), create_node(std::move(value))); }

	template< class... Args >
	reference emplace_back( Args&&... args );

	void pop_back();

	void push_front( const T& value ) { insert_impl(begin(), create_node(value)); }
	void push_front( T&& value ) { insert_impl(begin(), create_node(std::move(value))); }

	template< class... Args >
	reference emplace_front( Args&&... args );

	void pop_front();

	void resize( size_type count ) { resize(count, T()); }
	void resize( size_type count, const value_type& value );

	void swap( list& other ) noexcept;

	// operations
	void merge( list& other ) { merge(other, std::less<T>()); }
	void merge( list&& other ) { merge(std::move(other), std::less<T>()); }
	template< class Compare >
	void merge( list& other, Compare comp );
	template< class Compare >
	void merge( list&& other, Compare comp ) { merge(std::move(other), comp); }

	void splice( const_iterator pos, list& other );
	void splice( const_iterator pos, list&& other ) { splice(pos, std::move(other)); }
	void splice( const_iterator pos, list& other, const_iterator it );
	void splice( const_iterator pos, list&& other, const_iterator it ) { splice(pos, std::move(other), it); }
	void splice( const_iterator pos, list& other,
        const_iterator first, const_iterator last);
	void splice( const_iterator pos, list&& other,
        const_iterator first, const_iterator last) { splice(pos, std::move(other), first, last); }

	size_type remove( const T& value );
	template< class UnaryPredicate >
	size_type remove_if( UnaryPredicate p );

	void reverse() noexcept;

	size_type unique();
	template< class BinaryPredicate >
	size_type unique( BinaryPredicate p );	

	void sort() { sort(std::less<T>()); }
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

		friend void swap(base_node* rhs, base_node* lhs)
		{
			base_node* temp;
			rhs = temp;
			rhs = lhs;
			lhs = temp;
		}
	};

	struct node : base_node
	{
		T value;
		node() = default;
		node(const node&) = delete;
		node(const T& _val) : value(_val) { }

		template< class... Args>
		node(Args&& ...args) : value(std::forward<Args>(args)...) {}

		bool operator < (const node& other) const
		{
			return this->value < other.value;
		}

		friend void swap(node* lhs, node* rhs) 
		{
			node* temp;
			rhs = temp;
			rhs = lhs;
			lhs = temp;
		}
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

	iterator insert_impl(const_iterator pos, node* new_node);

	node* create_node(const T& value)
	{
		node* new_node = node_allocator_traits::allocate(m_alloc, 1);
		node_allocator_traits::construct(m_alloc, new_node, value);

		return new_node;
	}

	template< class... Args >
	node* create_node(Args&& ...args)
	{
		node* new_node = node_allocator_traits::allocate(m_alloc, 1);
		node_allocator_traits::construct(m_alloc, new_node, std::forward<Args>(args)...);

		return new_node;
	}

	void destroy_node(node* node)
	{
		node_allocator_traits::destroy(m_alloc, node);
		node_allocator_traits::deallocate(m_alloc, node, 1);
	}

	// node* split_list()
	// {
	// 	if (m_head == nullptr || m_head->next == &fake_node) return m_head;

	// 	node* slow = m_head;
	// 	node* fast = m_head;
	// 	while (fast->next != &fake_node || fast->next->next != &fake_node) 
	// 	{
	// 		slow = static_cast<node*>(slow->next);
	// 		fast = static_cast<node*>(fast->next->next);
	// 	}

	// 	node* middle = static_cast<node*>(slow->next);
	// 	slow->next = nullptr;

	// 	return middle; 
	// }
	
	list split_before(const_iterator here)
	{
		list second_list;

		return second_list.splice( second_list.begin(), *this, here, this->end() ) ;
	}

	list split_after(const_iterator here)
	{
		if( here == end() ) return {} ; // return empty list
    	else return split_before( ++here ) ;
	}



	template< class Compare >
	void merge_sort(Compare comp, iterator first, iterator last)
	{
		if (size() < 2) return;
		if (size() == 2)
		{
			if (*first, *last)
			{
				using std::swap;
				swap(*first, *last);
			}
		}

		auto middle_diff = std::distance(first, last) / 2;
		auto middle = first;
		std::advance(middle, middle_diff);		

		list second_list;
		second_list.splice(middle, *this);

		merge_sort(comp, first, last);
		second_list.merge_sort(comp, second_list.begin(), second_list.end());

		merge(second_list);
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
inline void list<T, Allocator>::clear()
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

template <class T, class Allocator>
inline list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, size_type count, const T& value)
{
	node* new_node = nullptr;

	for (size_type i = 0; i < count; i++)
	{
		new_node = create_node(value);

		if (pos == cend())
		{
			new_node->prev = m_tail;
			new_node->next = &fake_node;
			m_tail->next = new_node;
			m_tail = new_node;
		}
		else
		{
			node* pos_node = static_cast<node*>(pos.m_node);
			node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

			pos_node->prev = new_node;
			new_node->next = pos_node;

			prev_pos_node->next = new_node;
			new_node->prev = prev_pos_node;
		}
	}
	return iterator(new_node);
}

template <class T, class Allocator>
template <std::input_iterator InputIt>
inline list<T, Allocator>::iterator list<T, Allocator>::insert(const_iterator pos, InputIt first, InputIt last)
{
    node* new_node = nullptr;

	for (; first != last; ++first)
	{
		new_node = create_node(*first);

		if (pos == cend())
		{
			new_node->prev = m_tail;
			new_node->next = &fake_node;
			m_tail->next = new_node;
			m_tail = new_node;
		}
		else
		{
			node* pos_node = static_cast<node*>(pos.m_node);
			node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

			pos_node->prev = new_node;
			new_node->next = pos_node;

			prev_pos_node->next = new_node;
			new_node->prev = prev_pos_node;
		}
	}
	return iterator(new_node);
}

template <class T, class Allocator>
inline list<T, Allocator>::iterator list<T, Allocator>::insert_impl(const_iterator pos, node* new_node)
{
	node* prev = (pos == end()) ? m_tail : static_cast<node*>(pos.m_node->prev);
	node* next = (pos == end()) ? static_cast<node*>(&fake_node) : static_cast<node*>(pos.m_node);

	new_node->prev = prev;
	if (prev) prev->next = new_node;

	new_node->next = next;
	if (next) next->prev = new_node;

	if (prev == m_tail) m_tail = new_node;
	if (next == m_head) m_head = new_node;

	return iterator(new_node);
}

template <class T, class Allocator>
inline list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator pos)
{
    if (pos == cend())
	{
		return end();
	}

	else if (pos == cbegin())
	{
		node* current = static_cast<node*>(pos.m_node);
		node* next = static_cast<node*>(pos.m_node->next);

		fake_node.next = next;
		next->prev = &fake_node;

		m_head = static_cast<node*>(next->prev);

		destroy_node(current);

		return iterator(next);
	}

	node* current = static_cast<node*>(pos.m_node);
	node* next = static_cast<node*>(pos.m_node->next);
	node* prev = static_cast<node*>(pos.m_node->prev);

	prev->next = next;
	next->prev = prev;

	destroy_node(current);

	return iterator(next);
}

template <class T, class Allocator>
inline list<T, Allocator>::iterator list<T, Allocator>::erase(const_iterator first, const_iterator last)
{
    node* first_node = static_cast<node*>(first.m_node);
	node* last_node = static_cast<node*>(last.m_node);

	while (first_node != last_node)
	{
		node* next_node = static_cast<node*>(first_node->next);
		node* prev_node = static_cast<node*>(first_node->prev);

		prev_node->next = next_node;
		next_node->prev = prev_node;

		node* current = first_node;
		first_node = next_node;

		destroy_node(current);
	}

	return iterator(last_node);
}

template <class T, class Allocator>
template <class... Args>
inline list<T, Allocator>::reference list<T, Allocator>::emplace_back(Args&&... args)
{
	insert_impl(end(), create_node(std::forward<Args>(args)...));
	return m_tail->value;
}

template <class T, class Allocator>
template <class... Args>
inline list<T, Allocator>::reference list<T, Allocator>::emplace_front(Args&&... args)
{
    insert_impl(begin(), create_node(std::forward<Args>(args)...));
	return m_head->value;
}

template <class T, class Allocator>
inline void list<T, Allocator>::pop_back()
{
	if (empty()) return;

	node* prev = static_cast<node*>(m_tail->prev);
	node* next = static_cast<node*>(m_tail->next);

	destroy_node(m_tail);

	prev->next = next;
	next->prev = prev;

}

template <class T, class Allocator>
inline void list<T, Allocator>::pop_front()
{
	if (empty()) return;

	node* current = m_head;
	node* next = static_cast<node*>(current->next);

	fake_node.next = next;
	next->prev = &fake_node;

	m_head = next;

	destroy_node(current);
}

template <class T, class Allocator>
inline void list<T, Allocator>::resize(size_type count, const value_type& value)
{	
	if (empty()) fill_list(count, value);
	if (count == size()) return;

	if (size() < count) insert_impl(cend(), create_node(value));
	if (size() > count) 
	{
		for (size_t i = 0; i < abs(size() - count); i++)
			pop_back();
	}
}

template< class T, class Allocator >
inline void list<T, Allocator>::swap(list& other) noexcept
{
	if (node_allocator_traits::propagate_on_container_swap::value)
		std::swap(m_alloc, other.m_alloc);

	using std::swap;
	swap(&fake_node, &other.fake_node);
	swap(m_head->prev, other.m_head->prev);
	swap(m_tail->next, other.m_tail->next);
}

template<class T, class Allocator>
template<class Compare>
inline void list<T, Allocator>::merge(list& other, Compare comp)
{
	if (this == &other) return;
	if (other.empty()) return;

	auto it = begin();
	auto other_it = other.begin();

	while (other_it != other.end())
	{
		if (it == end() || comp(*other_it, *it))
		{
			auto transfer = other_it;
			++other_it;
			splice(it, other, transfer);
		}
		else ++it;
	}
}

template< class T, class Allocator >
inline void list<T, Allocator>::splice(const_iterator pos, list& other)
{
	if (other.empty()) return;
	
	node* pos_node = static_cast<node*>(pos.m_node);
	node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

	node* first_node = other.m_head;
	node* last_node = other.m_tail;

	pos_node->prev = last_node;
	last_node->next = pos_node;

	prev_pos_node->next = first_node;
	first_node->prev = prev_pos_node;

	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;
}

template< class T, class Allocator >
inline void list<T, Allocator>::splice(const_iterator pos, list& other, const_iterator it)
{
	node* pos_node = static_cast<node*>(pos.m_node);
	node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

	node* other_it_node = static_cast<node*>(it.m_node);

	if (it == other.cbegin())
	{
		node* next_other_it_node = static_cast<node*>(other.m_head->next);
		node* prev_other_it_node = static_cast<node*>(&other.fake_node);

		pos_node->prev = other_it_node;
		other_it_node->next = pos_node;

		prev_pos_node->next = other_it_node;
		other_it_node->prev = prev_pos_node;

		next_other_it_node->prev = &other.fake_node;
		other.fake_node.next = next_other_it_node;

		other.m_head = next_other_it_node;
	}

	else
	{
		node* next_other_it_node = static_cast<node*>(it.m_node->next);
		node* prev_other_it_node = static_cast<node*>(it.m_node->prev);

		pos_node->prev = other_it_node;
		other_it_node->next = pos_node;

		prev_pos_node->next = other_it_node;
		other_it_node->prev = prev_pos_node;

		next_other_it_node->prev = prev_other_it_node;
		prev_other_it_node->next = next_other_it_node;
	}
}

template< class T, class Allocator >
inline void list<T, Allocator>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
	if (first == last) return;

	if (first == other.cbegin())
	{
		node* prev_pos_node = static_cast<node*>(pos.m_node->prev);
		node* pos_node = static_cast<node*>(pos.m_node);

		node* prev_first_node = static_cast<node*>(&other.fake_node);
		node* first_node = static_cast<node*>(other.m_head);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		prev_last_node->next = pos_node;
		first_node->prev = prev_pos_node;
		prev_pos_node->next = first_node;
		pos_node->prev = prev_last_node;

		last_node->prev = &other.fake_node;
		other.fake_node.next = last_node;
		other.m_head = last_node;
	}

	else
	{
		node* prev_pos_node = static_cast<node*>(pos.m_node->prev);
		node* pos_node = static_cast<node*>(pos.m_node);

		node* prev_first_node = static_cast<node*>(first.m_node->prev);
		node* first_node = static_cast<node*>(first.m_node);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		prev_last_node->next = pos_node;
		first_node->prev = prev_pos_node;
		prev_pos_node->next = first_node;
		pos_node->prev = prev_last_node;
	}

}

template< class T, class Allocator >
inline list<T, Allocator>::size_type list<T, Allocator>::remove(const T& value)
{
	size_type counter = 0;

	for (auto it = begin(); it != end();)
	{
		if (*it == value)
		{
			it = erase(it);
			++counter;
		}
		else ++it;
	}
	return counter;
}

template< class T, class Allocator >
template< class UnaryPredicate >
inline list<T, Allocator>::size_type list<T, Allocator>::remove_if(UnaryPredicate p)
{
	size_type counter = 0;

	for (auto it = begin(); it != end();)
	{
		if (p(*it))
		{
			it = erase(it);
			++counter;
		}
		else ++it;
	}
	return counter;
}

template< class T, class Allocator >
inline void list<T, Allocator>::reverse() noexcept
{
	if (m_head == &fake_node || m_head->next == &fake_node) return;

	base_node* prev = nullptr;
	base_node* current = m_head;
	base_node* next = nullptr;

	m_tail = m_head;

	while (current != nullptr)
	{
		next = current->next;
		current->next = prev;
		current->prev = next;
		prev = current;
		current = next;
	}

	m_head = static_cast<node*>(prev);
}

template< class T, class Allocator >
inline list<T, Allocator>::size_type list<T, Allocator>::unique()
{
	return unique(
		[](const auto& lhs, const auto& rhs)
		{
			return lhs == rhs;
		}
	);
}

template< class T, class Allocator >
template< class BinaryPredicate >
inline list<T, Allocator>::size_type list<T, Allocator>::unique(BinaryPredicate p)
{
	if (empty()) return 0;

	size_type counter = 0;

	for (auto it = std::next(begin()); it != end();)
	{
		if (p(*std::prev(it), *it))
		{
			it = erase(it);
			++counter;
		}
		else
		{
			++it;
		}
	}
	return counter;
}

template< class T, class Allocator >
template< class Compare >
inline void list<T, Allocator>::sort(Compare comp) 
{
	merge_sort(comp, begin(), --end());
}




#endif // !STL_HEADER_CXX20
#endif // !_LIST_HPP_
