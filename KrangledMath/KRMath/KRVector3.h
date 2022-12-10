#pragma once
#include "KRVector.h"

namespace KRM
{
	template<typename T>
	class Vector<T, 3>
	{
	public:
		union
		{
			T m_Data[3];
			struct
			{
				T x;
				T y;
				T z;
			};
			struct
			{
				T r;
				T g;
				T b;
			};
		};

		_NODISCARD Vector Cross(const Vector& rhs) const;
	};

	template<typename T>
	inline Vector<T, 3> Vector<T, 3>::Cross(const Vector& rhs) const
	{
		static_assert(std::is_floating_point<T>::value);
		Vector outVec{};
		outVec.x = y * rhs.z - z * rhs.y;
		outVec.y = z * rhs.x - x * rhs.z;
		outVec.z = x * rhs.y - y - rhs.x;
		return outVec;
	}

	template<typename T>
	_NODISCARD inline Vector<T, 3> Cross(const Vector<T, 3>& lhs, const Vector<T, 3>& rhs)
	{
		static_assert(std::is_floating_point<T>::value);
		return lhs.Cross(rhs);
	}
}