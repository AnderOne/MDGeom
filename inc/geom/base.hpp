/**
 * Copyright (c) 2019-2020 Andrey Baranov <armath123@gmail.com>
 *
 * This file is part of MDGeom (Multi-Dimensional Geometry).
 *
 * MDGeom is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * MDGeom is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with MDGeom;
 * if not, see <http://www.gnu.org/licenses/>
**/

#pragma once
#include <type_traits>
#include <algorithm>
#include <array>
#include <numeric>
#include <cmath>

#include <iostream>
#include <cassert>

namespace GEOM {

namespace BASE {

#define __CHECK_PARAMETER_PACK_SIZE(arg, N)\
static_assert(sizeof...(arg) <= N, "Too many arguments!");\
static_assert(sizeof...(arg) >= N, "Too few arguments!");

#define __CHECK_TEMPLATE_POINT_TYPE(T)\
static_assert(std::is_floating_point<T>::value);

#define __CHECK_TEMPLATE_POINT_DIM(N)\
static_assert(N > 0, "Dimension must not be lesser one!");

#define __DEF_TRANSFORM_2(ans, lhs, rhs, op)\
std::transform(\
	lhs.begin(), lhs.end(), rhs.begin(), ans.begin(),\
	[](const auto &a, const auto &b) {\
	return a op b;\
	}\
);

#define __DEF_TRANSFORM_1(ans, lhs, val, op)\
std::transform(\
	lhs.begin(), lhs.end(), ans.begin(),\
	[v = val](const auto &a) {\
	return a op v;\
	}\
);

#define MATH_EPSILON (1.e-14)

#ifndef MATH_TYPE
#define MATH_TYPE double
#endif

//Forward declarations:

template <typename T, unsigned N>
struct t_vector;

template <typename T, unsigned N,
unsigned M = N>
struct t_basis;

typedef t_vector<MATH_TYPE, 4>
t_vector_4d;
typedef t_vector<MATH_TYPE, 3>
t_vector_3d;
typedef t_vector<MATH_TYPE, 2>
t_vector_2d;

template <unsigned N, unsigned M = N> using t_space = t_basis<MATH_TYPE, N, M>;

template <unsigned N> using t_plane = t_basis<MATH_TYPE, N, N - 1>;

typedef t_plane<4>
t_plane_4d;
typedef t_plane<3>
t_plane_3d;

template <unsigned N> using t_line = t_basis<MATH_TYPE, N, 1>;

typedef t_line<4>
t_line_4d;
typedef t_line<3>
t_line_3d;
typedef t_line<2>
t_line_2d;


//N-dimensional vector:
template <typename T, unsigned N> struct t_vector {

	__CHECK_TEMPLATE_POINT_TYPE(T)
	__CHECK_TEMPLATE_POINT_DIM(N)

	template <typename... TT, typename = typename std::enable_if<(sizeof...(TT) > 1)>::type>
	inline t_vector(TT... arg): dat{static_cast<T> (arg) ...} {
		__CHECK_PARAMETER_PACK_SIZE(arg, N)
	}
	inline t_vector(const T &val) { std::fill(dat.begin(), dat.end(), val); }
	inline t_vector() {}

	template <typename ... TT,
	          typename = typename std::enable_if<(sizeof ... (TT) == 0) && (N == 1)>::type>
	inline operator T() const {
		return dat[0];
	}

	//Vector transformations:

	inline t_vector ref(const t_vector<T, N> &center, const t_vector<T, N> &normal) const {
		return this->sub(normal.mul(this->sub(center).dot(normal) * 2));
	}

	inline t_vector ref(const t_basis<T, N, N - 1> &basis) const {
		t_basis<T, N, N> extBasis = basis.template ext<N>();
		const auto &normal = extBasis[N - 1];
		return ref(basis.center(), normal);
	}

	inline t_vector rot(const t_vector<T, N> &center, int i, int j, T angle) const {
		return this->sub(center).rot(i, j, angle).add(center);
	}

	inline t_vector rot(const t_basis<T, N> &basis, int i, int j, T angle) const {
		return basis.get(
		basis.put(*this).rot(i, j, angle)
		);
	}

	inline t_vector rot(int i, int j, T angle) const {
		t_vector ans(*this);
		ans.dat[i] = cos(angle) * dat[i] -
		sin(angle) * dat[j];
		ans.dat[j] = sin(angle) * dat[i] +
		cos(angle) * dat[j];
		return ans;
	}

	inline t_vector mov(const t_vector &dir) const {
		return this->add(dir);
	}

	//Vector operations:
	inline t_vector sub(const t_vector &rhs) const { t_vector ans; __DEF_TRANSFORM_2(ans, (*this), rhs, -); return ans; }
	inline t_vector add(const t_vector &rhs) const { t_vector ans; __DEF_TRANSFORM_2(ans, (*this), rhs, +); return ans; }

	//Mixed operations:
	inline t_vector div(const T &val) const { t_vector ans; __DEF_TRANSFORM_1(ans, (*this), val, /); return ans; }
	inline t_vector mul(const T &val) const { t_vector ans; __DEF_TRANSFORM_1(ans, (*this), val, *); return ans; }
	inline t_vector sub(const T &val) const { t_vector ans; __DEF_TRANSFORM_1(ans, (*this), val, -); return ans; }
	inline t_vector add(const T &val) const { t_vector ans; __DEF_TRANSFORM_1(ans, (*this), val, +); return ans; }
	inline t_vector neg() const { t_vector ans; __DEF_TRANSFORM_1(ans, (*this), -1, *); return ans; }

	inline const T dot(const t_vector &rhs) const { return std::inner_product(dat.begin(), dat.end(), rhs.dat.begin(), T(0)); }
	inline const T len()  const { return std::sqrt(len2()); }
	inline const T len2() const { return dot(*this); }

	//Comparison:
	inline bool operator!=(const t_vector &rhs) const { return !std::equal(dat.begin(), dat.end(), rhs.dat.begin()); }
	inline bool operator==(const t_vector &rhs) const { return std::equal(dat.begin(), dat.end(), rhs.dat.begin()); }

	//Assignment:
	inline t_vector &operator+=(const t_vector &rhs) { *this = this->add(rhs); return *this; }
	inline t_vector &operator-=(const t_vector &rhs) { *this = this->sub(rhs); return *this; }
	inline t_vector &operator*=(const T &val) { *this = this->mul(val); return *this; }
	inline t_vector &operator/=(const T &val) { *this = this->div(val); return *this; }

	//Data access:
	inline const T &operator[] (int i) const { return dat[i]; }
	inline T &operator[] (int i) { return dat[i]; }

	const auto begin() const { return dat.begin(); }
	const auto end() const { return dat.end(); }
	auto begin() { return dat.begin(); }
	auto end() { return dat.end(); }

private:
	std::array<T, N> dat;
};

//Orthonormal basis of Affine space:
template <typename T, unsigned N, unsigned M>
struct t_basis {

	typedef BASE::t_vector<T, N> t_vector;

	__CHECK_TEMPLATE_POINT_TYPE(T)
	__CHECK_TEMPLATE_POINT_DIM(N)
	__CHECK_TEMPLATE_POINT_DIM(M)

	static_assert(N >= M, "Subspace dimension must not be more owner dimension!");

	template <typename ... TT,
	          typename = typename std::enable_if<(sizeof...(TT) == M)>::type>
	inline t_basis(const t_vector &_top, TT && ... arg):
	               top(_top), vec{t_vector{arg} ...} {
		this->template ort<false>(0);
	}
	//Construct uniform basis:
	inline t_basis(const t_vector &_top): top(_top) {
		std::fill(vec.begin(), vec.end(), 0); for (int i = 0; i < M; ++ i) vec[i][i] = T(1);
	}
	inline t_basis(): top(0) {
		std::fill(vec.begin(), vec.end(), 0); for (int i = 0; i < M; ++ i) vec[i][i] = T(1);
	}

	//Basis transform:
	template <typename ... TT> inline t_basis rot(TT ... arg) const {
		t_basis ans; ans.top = top.rot(arg ...);
		for (int i = 0; i < M; ++ i) ans.vec[i] = top.add(vec[i]).rot(arg ...).sub(ans.top);
		return ans;
	}
	template <typename ... TT> inline t_basis mov(TT ... arg) const {
		t_basis ans;
		std::copy(vec.begin(), vec.end(), ans.vec.begin());
		ans.top = top.mov(arg ...);
		return ans;
	}

	//Orthogonal complement:
	template <unsigned K,
	          typename = typename std::enable_if<(K <= N) && (K > M)>::type>
	t_basis<T, N, K> ext() const {
		t_basis<T, N, K> basis;
		for (int i = 0; i < M; ++ i) { basis.vec[i] = vec[i]; }
		basis.top = top;
		for (int k = M, i = 0; (k < K) && (i < N); ++ i) {
			basis.vec[k] = 0; basis.vec[k][i] = T(1);
			if (basis.template ort<true>(k - 1)) {
				++ k;
			}
		}
		return basis;
	}

	//Project N-d point into basis of M-d subspace:
	inline BASE::t_vector<T, M> put(const t_vector &arg) const {
		BASE::t_vector<T, N> rad = arg.sub(top);
		BASE::t_vector<T, M> ans;
		for (int i = 0; i < M; ++ i) ans[i] = rad.dot(vec[i]);
		return ans;
	}
	//Expand point from basis to world coordinates:
	inline t_vector get(const BASE::t_vector<T, M> &arg) const {
		t_vector ans = top;
		for (int i = 0; i < M; ++ i) {
			ans = ans.add(vec[i].mul(arg[i]));
		}
		return ans;
	}

	const t_vector &operator[](int i) const {
		return vec[i];
	}
	const t_vector &center() const {
		return top;
	}
	const auto begin() const {
		return vec.begin();
	}
	const auto end() const {
		return vec.end();
	}

private:
	template <typename _T, unsigned _N, unsigned _M> friend struct t_basis;

	template <bool CHECK_DIV0 = false> inline bool ort(int _start) {

		std::array<T, M> L2;
		for (int i = 0; i <= _start; ++ i) L2[i] = vec[i].len2();

		//Orthogonalization:
		for (int i = _start + 1; i < M; ++ i) {
			BASE::t_vector<T, N> res = vec[i];
			for (int k = 0; k < i; ++ k) {
				res -= (vec[k].dot(vec[i]) / L2[k]) * vec[k];
			}
			if (CHECK_DIV0 && (res.len2() < MATH_EPSILON)) {
				return false;
			}
			vec[i] = std::move(res);
			L2[i] = res.len2();
		}

		//Normalization:
		for (int i = _start; i < M; ++ i) {
			vec[i] /= std::sqrt(L2[i]);
		}

		return true;
	}

	std::array<t_vector, M> vec;
	t_vector top;
};

//...

#define __DEF_BINARY_2(Q, LQ, RQ, S, C)\
template <typename T, unsigned N>\
Q<T, N> operator C(const LQ<T, N> &lhs, const RQ<T, N> &rhs) { return lhs.S(rhs); }

#define __DEF_BINARY_0(LQ, RQ, S, C)\
template <typename T, unsigned N>\
const T operator C(const LQ<T, N> &lhs, const RQ<T, N> &rhs) { return lhs.S(rhs); }

#define __DEF_BINARY_R(Q, LQ, S, C)\
template <typename L, typename R,\
                      unsigned N>\
Q<L, N> operator C(const LQ<L, N> &lhs, const R &rhs) { return lhs.S(rhs); }

#define __DEF_BINARY_L(Q, RQ, S, C)\
template <typename L, typename R,\
                      unsigned N>\
Q<R, N> operator C(const L &lhs, const RQ<R, N> &rhs) { return rhs.S(lhs); }

#define __DEF_UNARY(Q, S, C)\
template <typename T, unsigned N>\
Q<T, N> operator C(const Q<T, N> &arg) { return arg.S(); }

//...

__DEF_BINARY_2(t_vector, t_vector, t_vector, sub, -)
__DEF_BINARY_2(t_vector, t_vector, t_vector, add, +)

__DEF_BINARY_0(t_vector, t_vector, dot, *)

__DEF_BINARY_R(t_vector, t_vector, div, /)
__DEF_BINARY_R(t_vector, t_vector, mul, *)
__DEF_BINARY_R(t_vector, t_vector, sub, -)
__DEF_BINARY_R(t_vector, t_vector, add, +)

__DEF_BINARY_L(t_vector, t_vector, mul, *)
__DEF_BINARY_L(t_vector, t_vector, add, +)

__DEF_UNARY(t_vector, neg, -)

//...

template <typename T, unsigned N>
std::ostream &operator<<(std::ostream &out, const t_vector<T, N> &vec) {
	out << vec[0];
	for (int i = 1; i < N; ++ i) out << "\t" << vec[i];
	return out;
}

//...

#undef __CHECK_PARAMETER_PACK_SIZE
#undef __CHECK_TEMPLATE_POINT_TYPE
#undef __CHECK_TEMPLATE_POINT_DIM
#undef __DEF_TRANSFORM_2
#undef __DEF_TRANSFORM_1
#undef __DEF_BINARY_2
#undef __DEF_BINARY_0
#undef __DEF_BINARY_R
#undef __DEF_BINARY_L
#undef __DEF_UNARY

//...

}//BASE

}//GEOM
