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
	lhs.data(), lhs.data() + lhs.size(),\
	rhs.data(), ans.data(),\
	[](const auto &a, const auto &b) {\
	return a op b;\
	}\
);

#define __DEF_TRANSFORM_1(ans, lhs, val, op)\
std::transform(\
	lhs.data(), lhs.data() + lhs.size(),\
	ans.data(),\
	[v = val](const auto &a) {\
	return a op v;\
	}\
);

template <typename T, unsigned N>
struct t_matrix;

template <typename T, unsigned N>
struct t_vector;

template <typename T, unsigned N, unsigned M = N>
struct t_basis;

#define MATH_EPSILON (1.e-14)

//Matrix of linear transformation:
template <typename T, unsigned N>
struct t_matrix {

	__CHECK_TEMPLATE_POINT_TYPE(T)
	__CHECK_TEMPLATE_POINT_DIM(N)

	template <typename... TT, typename = typename std::enable_if<(sizeof...(TT) > 1)>::type>
	inline t_matrix(TT... arg): dat{static_cast<T> (arg) ...} {
		__CHECK_PARAMETER_PACK_SIZE(arg, N * N)
	}
	inline t_matrix(const T &val) { std::fill(dat, dat + N * N, val); }
	inline t_matrix() {}

	inline t_matrix<T, N> mul(const t_matrix<T, N> &rhs) const {
		t_matrix<T, N> ans;
		for (int i = 0; i < N; ++ i) for (int j = 0; j < N; ++ j) {
			ans[i][j] = row(i).dot(rhs.col(j));
		}
		return ans;
	}

	inline t_vector<T, N> mul(const t_vector<T, N> &rhs) const {
		t_vector<T, N> ans;
		for (int i = 0; i < N; ++ i) {
			ans[i] = row(i).dot(rhs);
		}
		return ans;
	}

	inline t_matrix<T, N> sub(const t_matrix<T, N> &rhs) const {
		t_matrix<T, N> ans;
		__DEF_TRANSFORM_2(ans, (*this), rhs, -)
		return ans;
	}
	inline t_matrix<T, N> add(const t_matrix<T, N> &rhs) const {
		t_matrix<T, N> ans;
		__DEF_TRANSFORM_2(ans, (*this), rhs, +)
		return ans;
	}
	inline t_matrix<T, N> div(const T &val) const {
		t_matrix<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), val, /)
		return ans;
	}
	inline t_matrix<T, N> mul(const T &val) const {
		t_matrix<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), val, *)
		return ans;
	}
	inline t_matrix<T, N> neg() const {
		t_matrix<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), -1, *)
		return ans;
	}

	inline t_vector<T, N> row(int i) const {
		t_vector<T, N> ans;
		for (int k = 0; k < N; ++ k) {
			ans[k] = (*this)[i][k];
		}
		return ans;
	}

	inline t_vector<T, N> col(int i) const {
		t_vector<T, N> ans;
		for (int k = 0; k < N; ++ k) {
			ans[k] = (*this)[k][i];
		}
		return ans;
	}

	//Get rotation matrix:
	inline static t_matrix<T, N> getRotation(int i, int j, const T &angle) {

		t_matrix mat = t_matrix::getIdentity();
		mat[i][j] = - sin(angle);
		mat[j][j] = cos(angle);
		mat[j][i] = sin(angle);
		mat[i][i] = cos(angle);
		return mat;
	}

	//Get identity matrix:
	inline static t_matrix<T, N> getIdentity() {
		t_matrix<T, N> mat = T(0);
		for (int i = 0; i < N; ++ i) {
			mat[i][i] = T(1);
		}
		return mat;
	}

	//Inverse matrix:
	inline t_matrix<T, N> inv() const;

	//Determinant:
	inline const T det() const;

	inline t_matrix<T, N> &operator+=(const t_matrix<T, N> &rhs) {
		*this = this->add(rhs);
		return *this;
	}
	inline t_matrix<T, N> &operator-=(const t_matrix<T, N> &rhs) {
		*this = this->sub(rhs);
		return *this;
	}
	inline t_matrix<T, N> &operator*=(const T &val) {
		*this = this->mul(val);
		return *this;
	}
	inline t_matrix<T, N> &operator/=(const T &val) {
		*this = this->div(val);
		return *this;
	}

	//Comparison:
	inline bool operator != (const t_matrix &rhs) const {
		return !std::equal(dat, dat + N * N, rhs.dat);
	}
	inline bool operator == (const t_matrix &rhs) const {
		return std::equal(dat, dat + N * N, rhs.dat);
	}

	//Data access:
	inline const T *operator[] (int i) const {
		return dat + i * N;
	}
	inline T *operator[] (int i) {
		return dat + i * N;
	}
	constexpr unsigned size() const {
		return N * N;
	}
	inline const T *data() const {
		return dat;
	}
	inline T *data() {
		return dat;
	}

private:
	T dat[N * N];
};

//N-dimensional vector:
template <typename T, unsigned N>
struct t_vector {

	__CHECK_TEMPLATE_POINT_TYPE(T)
	__CHECK_TEMPLATE_POINT_DIM(N)

	template <typename... TT, typename = typename std::enable_if<(sizeof...(TT) > 1)>::type>
	inline t_vector(TT... arg): dat{static_cast<T> (arg) ...} {
		__CHECK_PARAMETER_PACK_SIZE(arg, N)
	}
	inline t_vector(const T &val) { std::fill(dat, dat + N, val); }
	inline t_vector() {}

	inline t_vector<T, N> rot(const t_vector<T, N> &center, int i, int j, T angle) const {
		return this->sub(center).rot(i, j, angle).add(center);
	}

	inline t_vector<T, N> rot(const t_basis<T, N> &basis, int i, int j, T angle) const {
		return basis.get(
		basis.put(*this).rot(i, j, angle)
		);
	}

	inline t_vector<T, N> rot(int i, int j, T angle) const {
		t_vector<T, N> ans(*this);
		ans.dat[i] = cos(angle) * dat[i] -
		sin(angle) * dat[j];
		ans.dat[j] = sin(angle) * dat[i] +
		cos(angle) * dat[j];
		return ans;
	}

	inline t_vector<T, N> mov(const t_vector<T, N> &dir) const {
		return this->add(dir);
	}

	inline t_vector<T, N> sub(const t_vector<T, N> &rhs) const {
		t_vector<T, N> ans;
		__DEF_TRANSFORM_2(ans, (*this), rhs, -)
		return ans;
	}
	inline t_vector<T, N> add(const t_vector<T, N> &rhs) const {
		t_vector<T, N> ans;
		__DEF_TRANSFORM_2(ans, (*this), rhs, +)
		return ans;
	}
	inline t_vector<T, N> div(const T &val) const {
		t_vector<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), val, /)
		return ans;
	}
	inline t_vector<T, N> mul(const T &val) const {
		t_vector<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), val, *)
		return ans;
	}
	inline t_vector<T, N> neg() const {
		t_vector<T, N> ans;
		__DEF_TRANSFORM_1(ans, (*this), -1, *)
		return ans;
	}

	inline const T dot(const t_vector &rhs) const {
		return std::inner_product(
		dat, dat + N, rhs.dat, T(0)
		);
	}
	inline const T len() const {
		return std::sqrt(len2());
	}
	inline const T len2() const {
		return dot(*this);
	}

	inline t_vector<T, N> &operator+=(const t_vector<T, N> &rhs) {
		*this = this->add(rhs);
		return *this;
	}
	inline t_vector<T, N> &operator-=(const t_vector<T, N> &rhs) {
		*this = this->sub(rhs);
		return *this;
	}
	inline t_vector<T, N> &operator*=(const T &val) {
		*this = this->mul(val);
		return *this;
	}
	inline t_vector<T, N> &operator/=(const T &val) {
		*this = this->div(val);
		return *this;
	}

	//Comparison:
	inline bool operator != (const t_vector &rhs) const {
		return !std::equal(dat, dat + N, rhs.dat);
	}
	inline bool operator == (const t_vector &rhs) const {
		return std::equal(dat, dat + N, rhs.dat);
	}

	inline const T &operator[] (int i) const {
		return dat[i];
	}
	inline T &operator[] (int i) {
		return dat[i];
	}
	constexpr unsigned size() const {
		return N;
	}
	inline const T *data() const {
		return dat;
	}
	inline T *data() {
		return dat;
	}

private:
	T dat[N];
};

//Orthonormal basis of Affine space:
template <typename T, unsigned N, unsigned M>
struct t_basis {

	typedef BASE::t_vector<T, N> t_vector;

	__CHECK_TEMPLATE_POINT_TYPE(T)
	__CHECK_TEMPLATE_POINT_DIM(N)
	__CHECK_TEMPLATE_POINT_DIM(M)

	static_assert(N >= M, "Subspace dimension must not be more owner dimension!");

	template <typename... TT, typename = typename std::enable_if<(sizeof...(TT) == M)>::type>
	inline t_basis(TT && ... arg): vec{static_cast<t_vector> (arg) ...} {
		this->template ort<false>(0);
	}

	//Construct uniform basis:
	inline t_basis(): top(0) {
		std::fill(vec, vec + M, T(0)); for (int i = 0; i < M; ++ i) vec[i][i] = T(1);
	}

	//Basis transform:
	template <typename ... TT> inline t_basis rot(TT ... arg) const {
		t_basis ans; ans.top = top.rot(arg ...);
		for (int i = 0; i < M; ++ i) {
			ans.vec[i] = top.add(vec[i]).rot(arg ...).sub(ans.top);
		}
		return ans;
	}
	template <typename ... TT> inline t_basis mov(TT ... arg) const {
		t_basis ans;
		std::copy(vec, vec + M, ans.vec);
		ans.top = top.mov(arg ...);
		return ans;
	}

	//Orthogonal complement:
	template <unsigned K, typename = typename std::enable_if<(K <= N) && (K > M)>::type>
	t_basis<T, N, K> ext() const {

		t_basis<T, N, K> basis; for (int i = 0; i < M; ++ i) basis.vec[i] = vec[i];
		basis.top = top;

		for (int k = M, i = 0; (k < N + K) && (i < N); ++ i) {
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

	const t_vector &operator[](int i) const { return vec[i]; }
	const t_vector &center() const { return top; }

	const auto *begin() const { return vec; }
	const auto *end() const {
		return vec + M;
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

	t_vector vec[M];
	t_vector top;
};

//...

#define __DEF_BINARY_2(TYPE, L_TYPE, R_TYPE, S, C)\
template <typename T, unsigned N>\
TYPE<T, N> operator C(const L_TYPE<T, N> &L, const R_TYPE<T, N> &R) {\
	return L.S(R);\
}

#define __DEF_BINARY_0(L_TYPE, R_TYPE, S, C)\
template <typename T, unsigned N>\
const T operator C(const L_TYPE<T, N> &L, const R_TYPE<T, N> &R) {\
	return L.S(R);\
}

#define __DEF_BINARY_R(TYPE, L_TYPE, S, C)\
template <typename T, unsigned N>\
TYPE<T, N> operator C(const L_TYPE<T, N> &L, const T &R) {\
	return L.S(R);\
}

#define __DEF_BINARY_L(TYPE, R_TYPE, S, C)\
template <typename T, unsigned N>\
TYPE<T, N> operator C(const T &L, const R_TYPE<T, N> &R) {\
	return R.S(L);\
}

#define __DEF_UNARY(TYPE, S, C)\
template <typename T, unsigned N>\
TYPE<T, N> operator C(const TYPE<T, N> &A) {\
	return A.S();\
}

__DEF_BINARY_2(t_matrix, t_matrix, t_matrix, mul, *)
__DEF_BINARY_2(t_vector, t_matrix, t_vector, mul, *)

__DEF_BINARY_2(t_matrix, t_matrix, t_matrix, sub, -)
__DEF_BINARY_2(t_matrix, t_matrix, t_matrix, add, +)
__DEF_BINARY_2(t_vector, t_vector, t_vector, sub, -)
__DEF_BINARY_2(t_vector, t_vector, t_vector, add, +)

__DEF_BINARY_0(t_vector, t_vector, dot, *)

__DEF_BINARY_R(t_matrix, t_matrix, div, /)
__DEF_BINARY_R(t_matrix, t_matrix, mul, *)
__DEF_BINARY_R(t_vector, t_vector, div, /)
__DEF_BINARY_R(t_vector, t_vector, mul, *)

__DEF_BINARY_L(t_matrix, t_matrix, mul, *)
__DEF_BINARY_L(t_vector, t_vector, mul, *)

__DEF_UNARY(t_matrix, neg, -)
__DEF_UNARY(t_vector, neg, -)

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

template <typename T, unsigned N>
std::ostream &operator<<(std::ostream &out, const t_matrix<T, N> &mat) {
	out << mat.row(0);
	for (int i = 1; i < N; ++ i) out << "\n" << mat.row(i);
	return out;
}

template <typename T, unsigned N>
std::ostream &operator<<(std::ostream &out, const t_vector<T, N> &vec) {
	out << vec[0];
	for (int i = 1; i < N; ++ i) out << "\t" << vec[i];
	return out;
}

//...

typedef BASE::t_matrix<double, 4> t_matrix_4d;
typedef BASE::t_matrix<double, 3> t_matrix_3d;
typedef BASE::t_vector<double, 4> t_vector_4d;
typedef BASE::t_vector<double, 3> t_vector_3d;

template <unsigned N>
using t_space = t_basis<double, N, 3>;

typedef t_space<4>
t_space_4d;

template <unsigned N>
using t_plane = t_basis<double, N, 2>;

typedef t_plane<4>
t_plane_4d;
typedef t_plane<3>
t_plane_3d;

template <unsigned N>
using t_line = t_basis<double, N, 1>;

typedef t_line<4>
t_line_4d;
typedef t_line<3>
t_line_3d;
typedef t_line<2>
t_line_2d;

//...

}//BASE

}//GEOM
