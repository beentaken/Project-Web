#ifndef AUTO_LISTER
#define AUTO_LISTER

template <typename T>
class AutoLister
{
public:
	AutoLister()
	{
		m_next = Head();
		Head() = static_cast<T*>(this);
	}

	T* Next() { return m_next; }
	const T* Next() const { return m_next; }

	static T*& Head() { static T* p = 0; return p; }

private:
	T* m_next;
};

#endif