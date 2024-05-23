#ifndef _LIST_HPP_
#define _LIST_HPP_


#ifndef STL_HEADER_CXX20
#define STL_HEADER_CXX20

#include <iterator>
#include <concepts>
#include <memory>
#include <initializer_list>
#include <limits>

template< class T >
class list
{
private:
	struct base_node;
	struct node;
	class twindiriter;

	// type declarations
public:
	using value_type = T;
	using allocator_type = std::allocator<node>;
	using alloc_traits = std::allocator_traits<allocator_type>;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = twindiriter;
	using const_iterator = const twindiriter;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;


	// constructors and destructor
public:
	list();
	explicit list(size_type count);
	list(size_type count, const T& value);
	template< std::input_iterator InputIt >
	list(InputIt first, InputIt last);
	list(const list& other);
	list(list&& other) noexcept;
	list(std::initializer_list<T> ilist);
	~list();

	// assignment operators
	list& operator=(const list& other);
	list& operator=(list&& other) noexcept;
	list& operator=(std::initializer_list<T> ilist);

	// assign methods
	void assign(size_type count, const T& value);
	template< std::input_iterator InputIt >
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<T> ilist);

	// get_allocator
	allocator_type get_allocator() const noexcept { return m_alloc; }

	// element access
	reference front() { return m_head->value; }
	const_reference front() const noexcept { return m_head->value; }

	reference back() { return m_tail->value; }
	const_reference back() const noexcept { return m_tail->value; }

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
	bool empty() const noexcept { return m_size == 0; }
	size_type size() const noexcept { return std::distance(begin(), end()); }
	size_type max_size() const noexcept { return alloc_traits::max_size(m_alloc); }

	// modifiers
	void clear() noexcept;

	iterator insert(const_iterator pos, const T& value);
	iterator insert(const_iterator pos, T&& value);
	iterator insert(const_iterator pos, size_type count, const T& value);
	template< std::input_iterator InputIt >
	iterator insert(const_iterator pos, InputIt first, InputIt last);
	iterator insert(const_iterator pos, std::initializer_list<T> ilist);

	template< class... Args >
	iterator emplace(const_iterator pos, Args&&... args);

	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);

	void push_back(const T& value);
	void push_back(T&& value);

	template< class... Args >
	reference emplace_back(Args&&... args);

	void pop_back();

	void push_front(const T& value);
	void push_front(T&& value);

	template< class... Args >
	reference emplace_front(Args&&... args);

	void pop_front();

	void resize(size_type count);
	void resize(size_type count, const value_type& value);

	void swap(list& other) noexcept;

	// operations
	void merge(list& other);
	void merge(list&& other);
	template< class Compare >
	void merge(list& other, Compare comp);
	template< class Compare >
	void merge(list&& other, Compare comp);

	void splice(const_iterator pos, list& other);
	void splice(const_iterator pos, list&& other);
	void splice(const_iterator pos, list& other, const_iterator it);
	void splice(const_iterator pos, list&& other, const_iterator it);
	void splice(const_iterator pos, list& other,
		const_iterator first, const_iterator last);
	void splice(const_iterator pos, list&& other,
		const_iterator first, const_iterator last);

	size_type remove(const T& value);
	template< class UnaryPredicate >
	size_type remove_if(UnaryPredicate p);

	void reverse() noexcept;

	size_type unique();
	template< class BinaryPredicate >
	size_type unique(BinaryPredicate p);

	void sort();
	template< class Compare >
	void sort(Compare comp);

	// members
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

	base_node fake_node{ &fake_node, &fake_node };
	node* m_head = nullptr;
	node* m_tail = nullptr;
	allocator_type m_alloc;
	size_type m_size = 0;

	template< class Compare >
	void merge_sort(Compare comp, const_iterator start, const_iterator end);


};

// spaceship operator;
// template< class T >
// auto operator <=> (const list<T>& lhs, const list<T>& rhs)
// {
// 	return lhs <=> rhs;
// }

template< class T >
inline list<T>::list()
{
	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
	m_size = 0;
}

template< class T >
inline list<T>::list(size_type count)
{
	for (size_t i = 0; i < count; ++i)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, T());

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
		++m_size;
	}
}

template< class T >
inline list<T>::list(size_type count, const T& value)
{

	for (size_t i = 0; i < count; ++i)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, value);

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
		++m_size;
	}
}

template< class T >
template< std::input_iterator InputIt >
inline list<T>::list(InputIt first, InputIt last)
{
	for (; first != last; ++first)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, *first);

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
		++m_size;
	}
}

template< class T >
inline list<T>::list(const list& other)
{
	for (auto it = other.begin(); it != other.end(); ++it)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, *it);

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
		++m_size;
	}
}

template< class T >
inline list<T>::list(list&& other) noexcept
{
	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	m_alloc = std::move(other.m_alloc);

	fake_node.next = m_head;
	fake_node.prev = m_tail;

	m_head->prev = &fake_node;
	m_tail->next = &fake_node;

	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;

	other.m_size = 0;
}

template< class T >
inline list<T>::list(std::initializer_list<T> ilist)
{
	for (auto it = ilist.begin(); it != ilist.end(); ++it)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, *it);

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
		++m_size;
	}
}

template< class T >
inline list<T>::~list()
{
	node* current = m_head;
	while (current != &fake_node)
	{
		node* next = static_cast<node*>(current->next);
		alloc_traits::destroy(m_alloc, current);
		alloc_traits::deallocate(m_alloc, current, 1);
		current = next;
	}

	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
	fake_node.next = &fake_node;
	fake_node.prev = &fake_node;
	m_size = 0;
}

template< class T >
inline list<T>& list<T>::operator=(const list& other)
{
	if (this != &other)
	{
		this->clear();

		for (auto it = other.begin(); it != other.end(); ++it)
		{
			node* new_node = alloc_traits::allocate(m_alloc, 1);
			alloc_traits::construct(m_alloc, new_node, *it);

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
			++m_size;
		}

	}

	/**/

	return *this;
}

template< class T >
inline list<T>& list<T>::operator=(list&& other) noexcept 
{
	if (this == &other)
	{
		return *this;
	}

	clear();

	m_head = other.m_head;
	m_tail = other.m_tail;
	m_size = other.m_size;
	m_alloc = std::move(other.m_alloc);

	fake_node.next = m_head;
	fake_node.prev = m_tail;

	m_head->prev = &fake_node;
	m_tail->next = &fake_node;

	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;

	other.m_size = 0;

	return *this;
}

template< class T >
inline list<T>& list<T>::operator=(std::initializer_list<T> ilist)
{
	this->clear();

	for (auto it = ilist.begin(); it != ilist.end(); ++it)
	{
		node* new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, *it);

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
		++m_size;
	}

	return *this;
}

template< class T >
inline void list<T>::assign(size_type count, const T& value)
{
	clear();

	for (size_type i = 0; i < count; ++i)
	{
		push_back(value);
	}
}

template< class T >
template< std::input_iterator InputIt >
inline void list<T>::assign(InputIt first, InputIt last)
{
	clear();

	for (; first != last; ++first)
	{
		push_back(*first);
	}

}

template< class T >
inline void list<T>::assign(std::initializer_list<T> ilist)
{
	clear();

	for (const T& value : ilist)
	{
		push_back(value);
	}
}

template< class T >
inline void list<T>::clear() noexcept
{
	node* current = m_head;
	while (current != &fake_node)
	{
		node* next = static_cast<node*>(current->next);
		alloc_traits::destroy(m_alloc, current);
		alloc_traits::deallocate(m_alloc, current, 1);
		current = next;
	}

	m_head = static_cast<node*>(&fake_node);
	m_tail = static_cast<node*>(&fake_node);
	m_size = 0;
}

template< class T >
inline list<T>::iterator list<T>::insert(const_iterator pos, const T& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, value);

	if (m_size == 0)
	{
		m_head = new_node;
		m_tail = new_node;
		m_head->next = &fake_node;
		m_head->prev = &fake_node;
		fake_node.next = m_head;
		fake_node.prev = m_tail;
	}

	else if (pos == cend())
	{
		m_tail->next = new_node;
		new_node->prev = m_tail;
		new_node->next = &fake_node;
		fake_node.prev = new_node;
		m_tail = new_node;

	}

	else if (pos == cbegin())
	{
		m_head->prev = new_node;
		new_node->next = m_head;
		new_node->prev = &fake_node;
		fake_node.next = new_node;
		m_head = new_node;
	}

	else
	{
		node* current = static_cast<node*>(pos.m_node);
		node* prev = static_cast<node*>(current->prev);

		new_node->next = current;
		new_node->prev = prev;
		prev->next = new_node;
		current->prev = new_node;
	}
	++m_size;

	return iterator(new_node);
}

template< class T >
inline list<T>::iterator list<T>::insert(const_iterator pos, T&& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::move(value));

	if (m_size == 0)
	{
		m_head = new_node;
		m_tail = new_node;
		m_head->next = &fake_node;
		m_head->prev = &fake_node;
		fake_node.next = m_head;
		fake_node.prev = m_tail;
	}

	else if (pos == cend())
	{
		m_tail->next = new_node;
		new_node->prev = m_tail;
		new_node->next = &fake_node;
		fake_node.prev = new_node;
		m_tail = new_node;

	}

	else if (pos == cbegin())
	{
		m_head->prev = new_node;
		new_node->next = m_head;
		new_node->prev = &fake_node;
		fake_node.next = new_node;
		m_head = new_node;
	}

	else
	{
		node* current = static_cast<node*>(pos.m_node);
		node* prev = static_cast<node*>(current->prev);

		new_node->next = current;
		new_node->prev = prev;
		prev->next = new_node;
		current->prev = new_node;
	}

	++m_size;

	return iterator(new_node);
}

template< class T >
inline list<T>::iterator list<T>::insert(const_iterator pos, size_type count, const T& value)
{

	node* new_node = nullptr;

	for (size_type i = 0; i < count; i++)
	{
		new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, value);

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

		++m_size;
	}
	return iterator(new_node);
}

template< class T >
template< std::input_iterator InputIt >
inline list<T>::iterator list<T>::insert(const_iterator pos, InputIt first, InputIt last)
{
	node* new_node = nullptr;

	for (; first != last; ++first)
	{
		new_node = alloc_traits::allocate(m_alloc, 1);
		alloc_traits::construct(m_alloc, new_node, *first);

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

		++m_size;
	}
	return iterator(new_node);
}

template< class T >
inline list<T>::iterator list<T>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
	return insert(pos, ilist.begin(), ilist.end());

}

template< class T >
template< class... Args >
inline list<T>::iterator list<T>::emplace(const_iterator pos, Args&&... args)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::forward<Args>(args)...);

	if (m_size == 0)
	{
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
			m_tail->prev = new_node;
			new_node->next = m_tail;
			new_node->prev = &fake_node;
			fake_node.next = new_node;
			m_tail = new_node;
		}

		++m_size;

		return iterator(new_node);

	}

	node* prev = static_cast<node*>(pos.m_node->prev);

	new_node->prev = prev;
	new_node->next = pos.m_node;
	prev->next = new_node;
	pos.m_node->prev = new_node;

	++m_size;

	return iterator(new_node);
}

template< class T >
inline list<T>::iterator list<T>::erase(const_iterator pos)
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

		alloc_traits::destroy(m_alloc, current);
		alloc_traits::deallocate(m_alloc, current, 1);

		--m_size;

		return iterator(next);
	}

	node* current = static_cast<node*>(pos.m_node);
	node* next = static_cast<node*>(pos.m_node->next);
	node* prev = static_cast<node*>(pos.m_node->prev);

	prev->next = next;
	next->prev = prev;

	alloc_traits::destroy(m_alloc, current);
	alloc_traits::deallocate(m_alloc, current, 1);

	--m_size;

	return iterator(next);
}

template< class T >
inline list<T>::iterator list<T>::erase(const_iterator first, const_iterator last)
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

		alloc_traits::destroy(m_alloc, current);
		alloc_traits::deallocate(m_alloc, current, 1);

		--m_size;
	}

	return iterator(last_node);
}

template< class T >
inline void list<T>::push_back(const T& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, value);

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
	++m_size;
}

template< class T >
inline void list<T>::push_back(T&& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::move(value));

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
	++m_size;
}

template< class T >
template< class... Args >
inline list<T>::reference list<T>::emplace_back(Args&&... args)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::forward<Args>(args)...);

	if (m_head == nullptr)
	{
		m_head = new_node;
		m_tail = new_node;
		m_head->next = &fake_node;
		m_head->prev = &fake_node;
		fake_node.next = m_head;
		fake_node.prev = m_tail;

		++m_size;

		return m_head->value;
	}

	m_tail->next = new_node;
	new_node->prev = m_tail;
	new_node->next = &fake_node;
	fake_node.prev = new_node;
	m_tail = new_node;

	++m_size;

	return m_tail->value;

}

template< class T >
inline void list<T>::pop_back()
{
	if (m_size == 0)
	{
		return;
	}

	node* prev = static_cast<node*>(m_tail->prev);
	node* next = static_cast<node*>(m_tail->next);

	alloc_traits::destroy(m_alloc, m_tail);
	alloc_traits::deallocate(m_alloc, m_tail, 1);

	prev->next = next;
	next->prev = prev;

	--m_size;
}

template< class T >
inline void list<T>::push_front(const T& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, value);

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
		m_head->prev = new_node;
		new_node->next = m_head;
		new_node->prev = &fake_node;
		fake_node.next = new_node;
		m_head = new_node;
	}

	++m_size;
}

template< class T >
inline void list<T>::push_front(T&& value)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::move(value));

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
		m_head->prev = new_node;
		new_node->next = m_head;
		new_node->prev = &fake_node;
		fake_node.next = new_node;
		m_head = new_node;
	}

	++m_size;
}

template< class T >
template< class... Args >
inline list<T>::reference list<T>::emplace_front(Args&&... args)
{
	node* new_node = alloc_traits::allocate(m_alloc, 1);
	alloc_traits::construct(m_alloc, new_node, std::forward<Args>(args)...);

	if (m_head == nullptr)
	{
		m_head = new_node;
		m_tail = new_node;
		m_head->next = &fake_node;
		m_head->prev = &fake_node;
		fake_node.next = m_head;
		fake_node.prev = m_tail;

		++m_size;

		return m_head->value;
	}

	m_head->prev = new_node;
	new_node->next = m_head;
	new_node->prev = &fake_node;
	fake_node.next = new_node;
	m_head = new_node;

	++m_size;

	return m_head->value;
}

template< class T >
inline void list<T>::pop_front()
{
	if (m_size == 0)
	{
		return;
	}

	node* current = m_head;
	node* next = static_cast<node*>(current->next);

	fake_node.next = next;
	next->prev = &fake_node;

	m_head = next;

	alloc_traits::destroy(m_alloc, current);
	alloc_traits::deallocate(m_alloc, current, 1);

	--m_size;

}

template< class T >
inline void list<T>::resize(size_type count)
{
	if (count == m_size)
	{
		return;
	}

	else if (count < m_size)
	{
		for (size_t i = count; i < m_size; ++i)
		{
			node* current = m_tail;
			node* prev = static_cast<node*>(m_tail->prev);

			prev->next = &fake_node;
			fake_node.prev = prev;

			alloc_traits::destroy(m_alloc, current);
			alloc_traits::deallocate(m_alloc, current, 1);

			m_tail = prev;
		}
		m_size = count;
	}
	else if (count > m_size)
	{
		for (size_t i = m_size; i < count; ++i)
		{
			node* new_node = alloc_traits::allocate(m_alloc, 1);
			alloc_traits::construct(m_alloc, new_node, T());

			m_tail->next = new_node;
			new_node->prev = m_tail;
			new_node->next = &fake_node;
			fake_node.prev = new_node;
			m_tail = new_node;

			++m_size;
		}
	}
}

template< class T >
inline void list<T>::resize(size_type count, const value_type& value)
{
	if (count == m_size)
	{
		return;
	}

	else if (count < m_size)
	{
		for (size_t i = count; i < m_size; ++i)
		{
			node* current = m_tail;
			node* prev = static_cast<node*>(m_tail->prev);

			prev->next = &fake_node;
			fake_node.prev = prev;

			alloc_traits::destroy(m_alloc, current);
			alloc_traits::deallocate(m_alloc, current, 1);

			m_tail = prev;
		}
		m_size = count;
	}
	else if (count > m_size)
	{
		for (size_t i = m_size; i < count; ++i)
		{
			node* new_node = alloc_traits::allocate(m_alloc, 1);
			alloc_traits::construct(m_alloc, new_node, value);

			m_tail->next = new_node;
			new_node->prev = m_tail;
			new_node->next = &fake_node;
			fake_node.prev = new_node;
			m_tail = new_node;

			++m_size;
		}
	}
}

template< class T >
inline void list<T>::swap(list<T>& other) noexcept
{
	std::swap(fake_node, other.fake_node);

	std::swap(m_head->prev, other.m_head->prev);
	std::swap(m_tail->next, other.m_tail->next);

	std::swap(m_size, other.m_size);
	std::swap(m_alloc, other.m_alloc);
}

template< class T >
inline void list<T>::merge(list<T>& other)
{
	merge(other, std::less<T>());
}

template< class T >
inline void list<T>::merge(list<T>&& other)
{
	merge(std::move(other), std::less<T>());
}

template< class T >
template< class Compare >
inline void list<T>::merge(list<T>& other, Compare comp)
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
		else
		{
			++it;
		}
	}

}

template< class T >
template< class Compare >
inline void list<T>::merge(list<T>&& other, Compare comp)
{
	merge(std::move(other), comp);
}

template< class T >
inline void list<T>::splice(const_iterator pos, list& other)
{
	if (other.empty())
	{
		return;
	}

	node* pos_node = static_cast<node*>(pos.m_node);
	node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

	node* first_node = other.m_head;
	node* last_node = other.m_tail;

	pos_node->prev = last_node;
	last_node->next = pos_node;

	prev_pos_node->next = first_node;
	first_node->prev = prev_pos_node;

	m_size += other.m_size;

	other.m_size = 0;
	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;

}

template< class T >
inline void list<T>::splice(const_iterator pos, list&& other)
{
	if (other.empty())
	{
		return;
	}

	node* pos_node = static_cast<node*>(pos.m_node);
	node* prev_pos_node = static_cast<node*>(pos.m_node->prev);

	node* first_node = std::move(other.m_head);
	node* last_node = std::move(other.m_tail);

	pos_node->prev = std::move(last_node);
	last_node->next = std::move(pos_node);

	prev_pos_node->next = std::move(first_node);
	first_node->prev = std::move(prev_pos_node);

	m_size += other.m_size;

	other.m_size = 0;
	other.m_head = static_cast<node*>(&other.fake_node);
	other.m_tail = static_cast<node*>(&other.fake_node);
	other.m_head->prev = &other.fake_node;
	other.m_tail->next = &other.fake_node;
}

template< class T >
inline void list<T>::splice(const_iterator pos, list& other, const_iterator it)
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

		--other.m_size;
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

		--other.m_size;
	}
}

template< class T >
inline void list<T>::splice(const_iterator pos, list&& other, const_iterator it)
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

		--other.m_size;
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

		--other.m_size;
	}
}

template< class T >
inline void list<T>::splice(const_iterator pos, list& other, const_iterator first, const_iterator last)
{
	if (first == last)
	{
		return;
	}

	if (first == other.cbegin())
	{
		node* prev_pos_node = static_cast<node*>(pos.m_node->prev);
		node* pos_node = static_cast<node*>(pos.m_node);

		// ����������� ���������
		node* prev_first_node = static_cast<node*>(&other.fake_node);
		node* first_node = static_cast<node*>(other.m_head);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		// �������� ��������
		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		// ��������� �������� 

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

		// ����������� ���������
		node* prev_first_node = static_cast<node*>(first.m_node->prev);
		node* first_node = static_cast<node*>(first.m_node);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		// �������� ��������
		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		// ��������� �������� 

		prev_last_node->next = pos_node;
		first_node->prev = prev_pos_node;
		prev_pos_node->next = first_node;
		pos_node->prev = prev_last_node;
	}

}

template< class T >
inline void list<T>::splice(const_iterator pos, list&& other, const_iterator first, const_iterator last)
{
	if (first == last)
	{
		return;
	}

	if (first == last) {
		return;
	}

	if (first == other.cbegin())
	{
		node* prev_pos_node = static_cast<node*>(pos.m_node->prev);
		node* pos_node = static_cast<node*>(pos.m_node);

		// ����������� ���������
		node* prev_first_node = static_cast<node*>(&other.fake_node);
		node* first_node = static_cast<node*>(other.m_head);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		// �������� ��������
		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		// ��������� �������� 

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

		// ����������� ���������
		node* prev_first_node = static_cast<node*>(first.m_node->prev);
		node* first_node = static_cast<node*>(first.m_node);

		node* prev_last_node = static_cast<node*>(last.m_node->prev);
		node* last_node = static_cast<node*>(last.m_node);

		// �������� ��������
		prev_first_node->next = last_node;
		last_node->prev = prev_first_node;

		// ��������� �������� 
		prev_last_node->next = pos_node;
		first_node->prev = prev_pos_node;
		prev_pos_node->next = first_node;
		pos_node->prev = prev_last_node;
	}
}

template< class T >
inline list<T>::size_type list<T>::remove(const T& value)
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

template< class T >
template< class UnaryPredicate >
inline list<T>::size_type list<T>::remove_if(UnaryPredicate p)
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

template< class T >
inline void list<T>::reverse() noexcept
{
	if (m_head == &fake_node || m_head->next == &fake_node)
	{
		return;
	}

	// ����� ���� ���� ����� ������� ���� �������� ������������� 
	base_node* prev = nullptr;
	base_node* current = m_head;
	base_node* next = nullptr;

	// ����� ��������� �����
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

template< class T >
inline list<T>::size_type list<T>::unique()
{
	return unique(
		[](const auto& lhs, const auto& rhs)
		{
			return lhs == rhs;
		}
	);
}

template< class T >
template< class BinaryPredicate >
inline list<T>::size_type list<T>::unique(BinaryPredicate p)
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

template< class T >
inline void list<T>::sort()
{
	sort(std::less<T>());
}

template< class T >
template< class Compare >
inline void list<T>::sort(Compare comp)
{
	if (size() < 2) return;

	merge_sort(comp, begin(), end());

}


template< class T >
template< class Compare >
inline void list<T>::merge_sort(Compare comp, const_iterator start, const_iterator end)
{
	// для выбора сортировки нам нужно понимать, 
	// что у нас двунаправленный итератор
	// поэтому буду использовать merge sort
	// сложность такой сортировки n*log(n)
	// как в стандарте

}


#endif // !STL_HEADER_CXX20


#endif // !_LIST_HPP_
