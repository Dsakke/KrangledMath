#pragma once
#include "KRVector.h"

namespace KRM
{
	template<typename T>
	class Vector<T, 2>
	{
	public:
		union
		{
			T m_Data[2];
			struct 
			{
				T x;
				T y;
			};
			struct 
			{
				T r;
				T g;
			};
		};

		Vector(T x, T y);

		_NODISCARD T Cross(const Vector& rhs) const;
	private:
	};

	template<typename T>
	Vector<T, 2>::Vector(T x, T y)
		: x{ x }
		, y{ y }
	{ }

	template<typename T>
	_NODISCARD inline T Vector<T, 2>::Cross(const Vector& rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}

	template<typename T>
	_NODISCARD inline T Cross(const Vector<T, 2>& lhs, const Vector<T, 2>& rhs)
	{
		return lhs.Cross(rhs);
	}
}