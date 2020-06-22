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
#include "base.hpp"

namespace GEOM {

using namespace BASE;

namespace EXPR {

template <typename T, unsigned N> struct t_expr;

template <typename T, unsigned N,
typename E = t_expr<T, N>>
struct t_expr_rot_by_center;
template <typename T, unsigned N,
typename E = t_expr<T, N>>
struct t_expr_rot_in_basis;
template <typename T, unsigned N,
typename E = t_expr<T, N>>
struct t_expr_rot;
template <typename T, unsigned N,
typename E = t_expr<T, N>>
struct t_expr_ref;
template <typename T, unsigned N,
typename E = t_expr<T, N>>
struct t_expr_mov;

#define __DEF_TRANSFORM(T, N, E) \
template <typename ... TT>\
auto ref(TT && ... args) const { return t_expr_ref<T, N, E> (*this, std::forward<TT>(args) ... ); }\
template <typename ... TT>\
auto mov(TT && ... args) const { return t_expr_mov<T, N, E> (*this, std::forward<TT>(args) ... ); }\
\
auto rot(const t_vector<T, N> &center, unsigned i1, unsigned i2, T angle) const {\
	return t_expr_rot_by_center<T, N, E>(*this, center, i1, i2, angle);\
}\
auto rot(const t_basis<T, N> &basis, unsigned i1, unsigned i2, T angle) const {\
	return t_expr_rot_in_basis<T, N, E>(*this, basis, i1, i2, angle);\
}\
auto rot(unsigned i1, unsigned i2, T angle) const {\
	return t_expr_rot<T, N, E>\
	(*this, i1, i2, angle);\
}

template <typename T, unsigned N, typename E> struct t_expr_ref {

	explicit t_expr_ref(const E &expr, const t_basis<T, N, N - 1> &basis):
	_expr(expr), _center(basis.center()), _normal(basis.template ext<N>()[N - 1]) {}

	explicit t_expr_ref(const t_basis<T, N, N - 1> &basis):
	_center(basis.center()), _normal(basis.template ext<N>()[N - 1]) {}

	explicit t_expr_ref(const E &expr, const t_vector<T, N> &center,
	                    const t_vector<T, N> &normal):
	_expr(expr), _center(center), _normal(normal) {}

	explicit t_expr_ref(const t_vector<T, N> &center,
	                    const t_vector<T, N> &normal):
	_center(center), _normal(normal) {}

	__DEF_TRANSFORM(T, N, t_expr_ref)

	inline t_vector<T, N>
	operator()(const t_vector<T, N> &vec) const {
	return _expr(vec).ref(_center, _normal);
	}

private:
	const t_vector<T, N> _center;
	const t_vector<T, N> _normal;
	const E _expr;
};

template <typename T, unsigned N, typename E> struct t_expr_rot_by_center {

	explicit t_expr_rot_by_center(const E &expr, const t_vector<T, N> &center, unsigned i1, unsigned i2, T angle):
	_expr(expr), _center(center), _i1(i1), _i2(i2), _angle(angle) {}

	explicit t_expr_rot_by_center(const t_vector<T, N> &center, unsigned i1, unsigned i2, T angle):
	_center(center), _i1(i1), _i2(i2), _angle(angle) {}

	__DEF_TRANSFORM(T, N, t_expr_rot_by_center)

	inline t_vector<T, N>
	operator()(const t_vector<T, N> &vec) const {
	return _expr(vec).rot(
	_center, _i1, _i2, _angle
	);
	}

private:
	const t_vector<T, N> _center;
	const unsigned _i1;
	const unsigned _i2;
	const T _angle;
	const E _expr;
};

template <typename T, unsigned N, typename E> struct t_expr_rot_in_basis {

	explicit t_expr_rot_in_basis(const E &expr, const t_basis<T, N> &basis, unsigned i1, unsigned i2, T angle):
	_expr(expr), _basis(basis), _i1(i1), _i2(i2), _angle(angle) {}

	explicit t_expr_rot_in_basis(const t_basis<T, N> &basis, unsigned i1, unsigned i2, T angle):
	_basis(basis), _i1(i1), _i2(i2), _angle(angle) {}

	__DEF_TRANSFORM(T, N, t_expr_rot_in_basis)

	t_vector<T, N> operator()(const t_vector<T, N> &vec) const {
	return _expr(vec).rot(
	_basis, _i1, _i2, _angle
	);
	}

private:
	const t_basis<T, N> _basis;
	const unsigned _i1;
	const unsigned _i2;
	const T _angle;
	const E _expr;
};

template <typename T, unsigned N, typename E> struct t_expr_rot {

	explicit t_expr_rot(const E &expr, unsigned i1, unsigned i2, T angle):
	_expr(expr), _i1(i1), _i2(i2), _angle(angle) {}

	explicit t_expr_rot(unsigned i1, unsigned i2, T angle):
	_i1(i1), _i2(i2), _angle(angle) {}

	__DEF_TRANSFORM(T, N, t_expr_rot)

	t_vector<T, N> operator()(const t_vector<T, N> &vec) const {
	return _expr(vec).rot(
	_i1, _i2, _angle
	);
	}

private:
	const unsigned _i1;
	const unsigned _i2;
	const T _angle;
	const E _expr;
};

template <typename T, unsigned N, typename E> struct t_expr_mov {

	explicit t_expr_mov(const E &expr, const t_vector<T, N> &offset):
	_expr(expr), _offset(offset) {}

	explicit t_expr_mov(const t_vector<T, N> &offset):
	_offset(offset) {}

	__DEF_TRANSFORM(T, N, t_expr_mov)

	inline t_vector<T, N>
	operator()(const t_vector<T, N> &vec) const {
	return _expr(vec).mov(_offset);
	}

private:
	const t_vector<T, N> _offset;
	const E _expr;
};

template <typename T, unsigned N> struct t_expr {

	__DEF_TRANSFORM(T, N, t_expr)

	inline t_vector<T, N>
	operator()(const t_vector<T, N> &vec) const {
	return vec;
	}
};

//...

#undef __DEF_TRANSFORM

}//EXPR

}//GEOM
