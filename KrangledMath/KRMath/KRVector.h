#pragma once
#include <type_traits>
#include <cmath>

namespace KRM
{
	template<typename T, const int size>
	class Vector;
	template<typename VecType, unsigned int... Indexes>
	class Swizzle
	{
		using Type = VecType::Type;
		const static unsigned int VSize = VecType::Size;


		static_assert(std::conjunction<std::bool_constant<Indexes < VSize>...>::value, "Index is out of range");
	public:
		operator Vector<Type, sizeof...(Indexes)>() const
		{
			Vector<Type, sizeof...(Indexes)> vec{ m_Data[Indexes]... };
			return vec;
		}

		_NODISCARD Type operator[](unsigned int idx) const;
	private:
		Type m_Data[VSize];

	};


	// Base Vector class only contains members that need to be specialized 
	template<typename _T, int _Size>
	class VectorBase
	{
	public:
		const static unsigned int Size = _Size;
		using Type = _T;


		_T m_Data[_Size];
	protected:
		~VectorBase() = default;
	};



	template<typename _T>
	class VectorBase<_T, 2>
	{
	public:
		using Type = _T;

		const static unsigned int Size = 2;
		union
		{
			_T m_Data[2];
			struct { _T x, y; };
#include "Swizzle2.inc.h"

		};
	protected:
		~VectorBase() = default;
	};

	template<typename _T>
	class VectorBase<_T, 3>
	{
	public:
		using Type = _T;

		const static unsigned int Size = 3;

		union
		{
			_T m_Data[3];
			struct { _T x, y, z; };
#include "Swizzle3.inc.h"
		};
	protected:
		~VectorBase() = default;
	};

	template<typename _T>
	class VectorBase<_T, 4>
	{
	public:
		using Type = _T;

		const static unsigned int Size = 3;

		union
		{
			_T m_Data[4];
			struct { _T x, y, z, w; };
#include "Swizzle4.inc.h"
		};
	protected:
		~VectorBase() = default;
	};


	// Vector will contain all the members that don't need to be specialized
	template<typename T, const int size>
	class Vector final : public VectorBase<T, size>
	{
	public:
		static_assert(std::is_arithmetic<T>::value);
		Vector();


		Vector(T x, T y) requires (size >= 2);
		Vector(T x, T y, T z) requires (size >= 3);
		Vector(const Vector& rhs);
		Vector(Vector&& rhs);
		Vector& operator=(const Vector& rhs);
		Vector& operator=(Vector&& rhs);
		
		_NODISCARD Vector GetNormalized() const;
		Vector& Normalize();
		_NODISCARD T Dot(const Vector& rhs) const;
		_NODISCARD T AngleBetween(const Vector& rhs) const;
		_NODISCARD T Magnitude() const;
		_NODISCARD T SqrMagnitude() const;
		_NODISCARD Vector Reflect(const Vector& normal) const;
		_NODISCARD Vector Reject(const Vector& v) const;
		_NODISCARD Vector Project(const Vector& v) const;

		_NODISCARD auto Cross(const Vector& rhs) const requires (size == 3 || size == 2);

		/// <summary>
		/// No Range checks
		/// </summary>
		_NODISCARD T& operator[](uint32_t index);

		// Member operator overloads
		_NODISCARD Vector operator*(T scalar) const;
		Vector& operator*=(T scalar);
		_NODISCARD Vector operator-(const Vector& rhs) const;
		Vector& operator-=(const Vector& rhs);
		_NODISCARD Vector operator+(const Vector& rhs) const;
		Vector& operator+=(const Vector& rhs);
	private:
	};
	
	// Member functions

	template<typename T, int size>
	inline Vector<T, size>::Vector()
		: VectorBase<T, size>::VectorBase{}
	{
	}

	template<typename T, int size>
	inline Vector<T, size>::Vector(T x, T y) requires (size >= 2)
	{
		this->m_Data[0] = x;
		this->m_Data[1] = y;
	}

	template<typename T, int size>
	inline Vector<T, size>::Vector(T x, T y, T z) requires (size >= 3)
	{
		this->m_Data[0] = x;
		this->m_Data[1] = y;
		this->m_Data[2] = z;
	}

	template<typename T, int size>
	inline Vector<T, size>::Vector(const Vector& rhs)
		: VectorBase<T, size>{}
	{
		std::memcpy(this->m_Data, rhs.m_Data, size * sizeof(T));
	}

	template<typename T, int size>
	inline Vector<T, size>::Vector(Vector&& rhs)
		: VectorBase<T, size>{}
	{
		std::memcpy(this->m_Data, rhs.m_Data, size * sizeof(T));
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::operator=(const Vector& rhs)
	{
		std::memcpy(this->m_Data, rhs.m_Data, size * sizeof(T));
		return *this;
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::operator=(Vector&& rhs)
	{
		std::memcpy(this->m_Data, rhs.m_Data, size * sizeof(T));
		return *this;
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::GetNormalized() const
	{
		static_assert(std::is_floating_point<T>::value);
		Vector output = *this;
		return (output * (1 / Magnitude()));
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::Normalize()
	{
		static_assert(std::is_floating_point<T>::value);
		return operator*=(1 / Magnitude());
	}

	template<typename T, int size>
	inline T Vector<T, size>::Dot(const Vector& rhs) const
	{
		static_assert(std::is_floating_point<T>::value);
		T dot{};
		for (int i{}; i < size; ++i)
		{
			dot += this->m_Data[i] * rhs.m_Data[i];
		}
		return dot;
	}

	template<typename T, int size>
	inline T Vector<T, size>::AngleBetween(const Vector& rhs) const
	{
		return acos(Dot(rhs) / (Magnitude() * rhs.Magnitude()));
	}

	template<typename T, int size>
	inline T Vector<T, size>::Magnitude() const
	{
		static_assert(std::is_floating_point<T>::value);
		return (T)sqrt(SqrMagnitude());
	}

	template<typename T, int size>
	inline T Vector<T, size>::SqrMagnitude() const
	{
		static_assert(std::is_floating_point<T>::value);
		T sqrMagnitude{};
		for (int i{}; i < size; ++i)
		{
			sqrMagnitude += this->m_Data[i] * this->m_Data[i];
		}
		return sqrMagnitude;
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::Reflect(const Vector& normal) const
	{
		static_assert(std::is_floating_point<T>::value);
		return *this - 2 * Dot(normal) * normal;
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::Reject(const Vector& v) const
	{
		static_assert(std::is_floating_point<T>::value);
		return operator-(Project(v));
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::Project(const Vector& v) const
	{
		static_assert(std::is_floating_point<T>::value);
		return (Dot(v) / v.Dot(v)) * v;
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::operator*(T scalar) const
	{
		Vector<T, size> outVec{*this};
		outVec *= scalar;
		return outVec;
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::operator*=(T scalar)
	{
		for (int i{}; i < size; ++i)
		{
			this->m_Data[i] *= scalar;
		}
		return *this;
	}

	template<typename T, int size>
	Vector<T, size> Vector<T,size>::operator-(const Vector& rhs) const
	{
		Vector outVec{*this};
		outVec -= rhs;
		return outVec;
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::operator-=(const Vector& rhs)
	{
		for (int i{}; i < size; ++i)
		{
			this->m_Data[i] -= rhs.m_Data[i];
		}
		return *this;
	}

	template<typename T, int size>
	inline Vector<T, size> Vector<T, size>::operator+(const Vector& rhs) const
	{
		Vector outVec{ *this };
		outVec += rhs;
		return outVec;
	}

	template<typename T, int size>
	inline Vector<T, size>& Vector<T, size>::operator+=(const Vector& rhs)
	{
		for (int i{}; i < size; ++i)
		{
			this->m_Data[i] += rhs.m_Data[i];
		}
		return *this;
	}

	// Non-member functions
	template<typename T, int size>
	_NODISCARD T Dot(const Vector<T, size>& lhs, const Vector<T, size>& rhs)
	{
		return lhs.Dot(rhs);
	}
	
	template<typename T, int size>
	_NODISCARD T AngleBetween(const Vector<T, size>& lhs, const Vector<T, size>& rhs)
	{
		return lhs.AngleBetween(lhs);
	}

	template<typename T, int size>
	_NODISCARD Vector<T, size> Reflect(const Vector<T, size>& incoming, const Vector<T, size>& normal)
	{
		return incoming.Reflect(normal);
	}

	template<typename T, int size>
	_NODISCARD Vector<T, size> Reject(const Vector<T, size>& u, const Vector<T, size>& v)
	{
		return u.Reject(v);
	}

	template<typename T, int size>
	_NODISCARD Vector<T, size> Project(const Vector<T, size>& u, const Vector<T, size>& v)
	{
		return u.Project(v);
	}

	template<typename T, int size>
	_NODISCARD auto Cross(const Vector<T, size>& lhs, const Vector<T, size>& rhs) requires (size == 2 || size == 3)
	{}

	template<typename T>
	_NODISCARD auto Cross(const Vector<T, 2>& lhs, const Vector<T, 2>& rhs)
	{
		return lhs.m_Data[0] * rhs.m_Data[1] - lhs.m_Data[1] * rhs.m_Data[0];
	}

	template<typename T>
	_NODISCARD auto Cross(const Vector<T, 3>& lhs, const Vector<T, 3>& rhs)
	{
		Vector<T, 3> output = Vector<T, 3>{};

		output.m_Data[0] = lhs.m_Data[1] * rhs.m_Data[2] - lhs.m_Data[2] * rhs.m_Data[1];
		output.m_Data[1] = -lhs.m_Data[2] * rhs.m_Data[0] + lhs.m_Data[0] * rhs.m_Data[2];
		output.m_Data[2] = lhs.m_Data[0] * rhs.m_Data[1] - lhs.m_Data[1] * rhs.m_Data[0];

		return output;
	}

	template<typename T, int size>
	_NODISCARD auto Vector<T, size>::Cross(const Vector& rhs) const requires (size == 3 || size == 2)
	{
		return KRM::Cross(*this, rhs);
	}
	template<typename T, int size>
	inline T& Vector<T, size>::operator[](uint32_t index)
	{
		return this->m_Data[index];
	}
	template<typename VecType, unsigned int... Indexes>
	inline VecType::Type Swizzle<VecType, Indexes...>::operator[](unsigned int idx) const
	{
		return m_Data[Indexes[idx]];
	}
}

template<typename T, int size>
KRM::Vector<T, size> operator*(T scalar, const KRM::Vector<T, size>& rhs)
{
	return rhs * scalar;
}
