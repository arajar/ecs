#pragma once

#include <cstdint>
#include <functional>

///////////////////////////////////////////////////////////////////////////////

template<class C, typename T = int32_t>
struct Identifier
{
	inline Identifier() : m_value() {}
	inline explicit Identifier(const T& i) : m_value(i) {}
	inline Identifier(const Identifier<C, T>& i) : m_value(i.m_value) {}

	inline const T& value() const
	{
		return m_value;
	}

	inline bool operator==(const Identifier<C, T>& id) const { return m_value == id.m_value; }
	inline bool operator!=(const Identifier<C, T>& id) const { return m_value != id.m_value; }
	inline T operator++() { return m_value++; }

private:
	T m_value;
};

///////////////////////////////////////////////////////////////////////////////

namespace std
{
	template <class C, typename T>
	struct less<Identifier<C, T> > : binary_function < Identifier<C, T>, Identifier<C, T>, bool >
	{
		bool operator() (const Identifier<C, T>& x, const Identifier<C, T>& y) const
		{
			return x.value() < y.value();
		}
	};
};

///////////////////////////////////////////////////////////////////////////////

#define ID(x) public: typedef Identifier<x> Id; public: Id m_id;
#define IDs(x) public: typedef Identifier<x, std::string> Id; public: Id m_id;

///////////////////////////////////////////////////////////////////////////////

// From Google's cityhash
inline uint64_t hash_128_to_64(const uint64_t upper, const uint64_t lower)
{
	// Murmur-inspired hashing.
	const uint64_t kMul = 0x9ddfea08eb382d69ULL;
	uint64_t a = (lower ^ upper) * kMul;
	a ^= (a >> 47);
	uint64_t b = (upper ^ a) * kMul;
	b ^= (b >> 47);
	b *= kMul;
	return b;
}

///////////////////////////////////////////////////////////////////////////////

