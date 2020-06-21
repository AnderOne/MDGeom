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
#include <memory>
#include <array>
#include <vector>
#include <map>

namespace GEOM {

using namespace BASE;

namespace MESH {

template <typename T, unsigned N> using t_vert = t_vector<T, N>;

//NOTE: Obtaining simplicial cells after such operations as slicing and etc. is very difficult for N-d case!
//Therefore, we need to use arbitrary convex polytopes.
template <unsigned N> struct t_item { typedef std::vector<int> t_type; };
template <> struct t_item<1> { typedef std::array<int, 2> t_type; };

template <unsigned N> using t_cell = typename t_item<N>::t_type;

template <unsigned N> struct t_push {

	void add(int val) { item.push_back(val); ++ pos; }

	MESH::t_cell<N> item;
	int pos = 0;
};

template <> struct t_push<1> {

	void add(int val) { item[pos] = val; ++ pos; }

	MESH::t_cell<1> item;
	int pos = 0;
};

typedef t_cell<3> t_body;
typedef t_cell<2> t_face;
typedef t_cell<1> t_edge;

//Forward declarations:
template <typename T, unsigned N, unsigned M, unsigned K>
struct t_iter;
template <typename T, unsigned N, unsigned M, unsigned K>
struct t_link;
template <typename T, unsigned N, unsigned M, unsigned K>
struct t_part;
template <typename T, unsigned N, unsigned M = N>
struct t_mesh;
template <unsigned N>
struct t_grid;

typedef t_mesh<MATH_TYPE, 4>
t_mesh_4d;
typedef t_mesh<MATH_TYPE, 3>
t_mesh_3d;
typedef t_mesh<MATH_TYPE, 2>
t_mesh_2d;

template <unsigned N>
using t_surf =
t_mesh<MATH_TYPE, N, N - 1>;

typedef t_surf<4>
t_surf_4d;
typedef t_surf<3>
t_surf_3d;


//Handler classes:
template <unsigned N,
          unsigned M>
struct t_hand {

	static_assert((M >= 0) && (M < N));

	template <typename ... TT> static t_grid<N> make(const std::vector<t_cell<M>> &cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          make(std::forward<TT>(args) ...));
		fill(grid, cell);
		t_hand<N, M>::cell(grid) = cell;
		return grid;
	}
	template <typename ... TT> static t_grid<N> make(std::vector<t_cell<M>> &&cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          make(std::forward<TT>(args) ...));
		fill(grid, cell);
		t_hand<N, M>::cell(grid) =
		          std::move(cell);
		return grid;
	}

	template <unsigned K, typename = typename std::enable_if<(K > N)>::type>
	static t_grid<N> make(const t_grid<K> &other) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::make(other));
		fill(grid, t_hand<K, M>::cell(other));
		t_hand<N, M>::cell(grid) = t_hand<K, M>::cell(other);
		return grid;
	}
	static t_grid<N> make(const t_grid<N> &grid) {
		return grid;
	}
	static t_grid<N> &&make(t_grid<N> &&grid) {
		return std::move(grid);
	}

	static void fill(t_grid<N> &grid, const std::vector<t_cell<M>> &cell) {
		auto &link = grid.template link<M - 1>();
		for (int c = 0; c < cell.size(); ++ c)
		for (int l: cell[c]) {
			if (l >= link.size()) {
				link.resize(l + 1);
			}
			link[l].push_back(c);
		}
	}

	static const std::vector<std::vector<int>> &link(const t_grid<N> &grid) {
		return t_hand<N-1, M>::link(grid.GRID);
	}
	static std::vector<std::vector<int>> &link(t_grid<N> &grid) {
		return t_hand<N-1, M>::link(grid.GRID);
	}

	static const std::vector<t_cell<M>> &cell(const t_grid<N> &grid) {
		return t_hand<N-1, M>::cell(grid.GRID);
	}
	static std::vector<t_cell<M>> &cell(t_grid<N> &grid) {
		return t_hand<N-1, M>::cell(grid.GRID);
	}

	static const t_grid<M> &grid(const t_grid<N> &grid) {
		return t_hand<N-1, M>::grid(grid.GRID);
	}
	static t_grid<M> &grid(t_grid<N> &grid) {
		return t_hand<N-1, M>::grid(grid.GRID);
	}
};

template <unsigned N>
struct t_hand<N, N> {

	static const std::vector<std::vector<int>> &link(const t_grid<N> &grid) { return grid.LINK; }
	static std::vector<std::vector<int>> &link(t_grid<N> &grid) { return grid.LINK; }

	static const std::vector<t_cell<N>> &cell(const t_grid<N> &grid) { return grid.CELL; }
	static std::vector<t_cell<N>> &cell(t_grid<N> &grid) { return grid.CELL; }

	static const t_grid<N> &grid(const t_grid<N> &grid) { return grid; }
	static t_grid<N> &grid(t_grid<N> &grid) { return grid; }

	static t_grid<N> make(const std::vector<t_cell<N>> &cell) {
		t_grid<N> grid; fill(grid, cell); grid.CELL = cell;
		return grid;
	}
	static t_grid<N> make(std::vector<t_cell<N>> &&cell) {
		t_grid<N> grid; fill(grid, cell);
		grid.CELL = std::move(cell);
		return grid;
	}
	template <unsigned K, typename = typename std::enable_if<(K > N)>::type>
	static t_grid<N> make(const t_grid<K> &other) {
		t_grid<N> grid; fill(grid, t_hand<K, N>::cell(other));
		grid.CELL = t_hand<K, N>::cell(other);
		return grid;
	}
	static t_grid<N> make(const t_grid<N> &grid) {
		return grid;
	}
	static t_grid<N> &&make(t_grid<N> &&grid) {
		return std::move(grid);
	}

	static void fill(t_grid<N> &grid, const std::vector<t_cell<N>> &cell) {
		auto &link = grid.template link<N - 1>();
		for (int c = 0; c < cell.size(); ++ c)
		for (int l: cell[c]) {
			if (l >= link.size()) {
				link.resize(l + 1);
			}
			link[l].push_back(c);
		}
	}

};

//Grid structures:
template <unsigned N>
struct t_grid {

	template <unsigned M> const std::vector<std::vector<int>> &link() const {
		return t_hand<N, M>::link(*this);
	}
	template <unsigned M> std::vector<std::vector<int>> &link() {
		return t_hand<N, M>::link(*this);
	}

	template <unsigned M, typename = typename std::enable_if<(M > 0)>::type>
	const std::vector<t_cell<M>> &cell() const {
		return t_hand<N, M>::cell(*this);
	}
	template <unsigned M, typename = typename std::enable_if<(M > 0)>::type>
	std::vector<t_cell<M>> &cell() {
		return t_hand<N, M>::cell(*this);
	}

	template <unsigned M> const t_grid<M> &grid() const {
		return t_hand<N, M>::grid(*this);
	}
	template <unsigned M> t_grid<M> &grid() {
		return t_hand<N, M>::grid(*this);
	}

private:
	template <unsigned _N, unsigned _M>
	friend struct t_hand;

	std::vector<std::vector<int>> LINK;
	std::vector<t_cell<N>> CELL;
	t_grid<N-1> GRID;
};

template <> struct
t_grid<0> {
	std::vector<std::vector<int>> LINK;
};

//Iterator:
template <typename T, unsigned N, unsigned M, unsigned K> struct t_iter {

	typedef MESH::t_part<T, N, M, K> t_part;
	typedef MESH::t_mesh<T, N, M> t_mesh;

	bool operator==(const t_iter &other) const { return ITEM == other.ITEM; }
	bool operator!=(const t_iter &other) const { return ITEM != other.ITEM; }

	const t_part *operator->() const { return &PART; }
	const t_part &operator*() const { return PART; }

	t_iter  operator++(int) { t_iter iter(*this); next(); return iter; }
	t_iter  operator--(int) { t_iter iter(*this); prev(); return iter; }
	t_iter &operator++() { next(); return *this; }
	t_iter &operator--() { prev(); return *this; }

private:
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_link;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_part;
	template <typename _T, unsigned _N, unsigned _K>
	friend struct MESH::t_mesh;

	void next() { PART.ind = *(++ ITEM); }
	void prev() { PART.ind = *(-- ITEM); }

	t_iter(const t_mesh &_mesh, const int *_item, int _ind, bool _val = true):
	    ITEM(_item + _ind),
	    PART(_mesh) {
	    if (_val) PART.ind = ITEM[_ind];
	}

	const int *ITEM;
	t_part PART;
};

//Links:
template <typename T, unsigned N, unsigned M, unsigned K> struct t_link {

	typedef MESH::t_mesh<T, N, M> t_mesh;

	t_iter<T, N, M, K+1> begin() const {
		return t_iter<T, N, M, K+1>(MESH, MESH.template link<K>()[ind].data(), 0);
	}
	t_iter<T, N, M, K+1> end() const {
		return t_iter<T, N, M, K+1>(MESH, MESH.template link<K>()[ind].data(),
		MESH.template link<K>()[ind].size(),
		false
		);
	}

	const std::vector<int> &item() const { return MESH.template link<K>()[ind]; }
	int item(int i) const { return MESH.template link<K>()[ind][i]; }
	size_t size() const { MESH.template link<K>()[ind].size(); }

private:
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_link;
	template <typename _T, unsigned _N, unsigned _K>
	friend struct MESH::t_mesh;

	t_link(const t_mesh &_mesh, int _ind):
	       MESH(_mesh), ind(_ind) {}
	t_link(const t_mesh &_mesh):
	       MESH(_mesh) {}

	const t_mesh &MESH;
	int ind;
};

template <typename T, unsigned N, unsigned M> struct t_link<T, N, M, M> {

	typedef MESH::t_mesh<T, N, M> t_mesh;

	size_t size() const { return 0; }

private:
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_link;
	template <typename _T, unsigned _N, unsigned _K>
	friend struct MESH::t_mesh;

	t_link(const t_mesh &_mesh, int _ind):
	       MESH(_mesh), ind(_ind) {}
	t_link(const t_mesh &_mesh):
	       MESH(_mesh) {}

	const t_mesh &MESH;
	int ind;
};

//Proxy:
template <typename T, unsigned N, unsigned M, unsigned K> struct t_part {

	typedef MESH::t_link<T, N, M, K> t_link;
	typedef MESH::t_mesh<T, N, M> t_mesh;

	t_iter<T, N, M, K-1> begin() const {
		return t_iter<T, N, M, K-1>(MESH, MESH.template cell<K>()[ind].data(), 0);
	}
	t_iter<T, N, M, K-1> end() const {
		return t_iter<T, N, M, K-1>(MESH, MESH.template cell<K>()[ind].data(),
		MESH.template cell<K>()[ind].size(),
		false
		);
	}

	const t_cell<K> &item() const { return MESH.template cell<K>()[ind]; }
	int item(int i) const { return MESH.template cell<K>()[ind][i]; }
	t_link link() const { return MESH.template link<K>(ind); }

	int id() const { return ind; }

private:
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_part;
	template <typename _T, unsigned _N, unsigned _K>
	friend struct MESH::t_mesh;

	t_part(const t_mesh &_mesh, int _ind):
	       MESH(_mesh), ind(_ind) {}
	t_part(const t_mesh &_mesh):
	       MESH(_mesh) {}

	const t_mesh &MESH;
	int ind;
};

template <typename T, unsigned N, unsigned M> struct t_part<T, N, M, 0> {

	typedef MESH::t_link<T, N, M, 0> t_link;
	typedef MESH::t_mesh<T, N, M> t_mesh;

	const t_vert<T, N> &data() const { return MESH.vert()[ind]; }
	t_link link() const { return MESH.template link<0>(ind); }

	int id() const { return ind; }

private:
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M, unsigned _K>
	friend struct MESH::t_part;
	template <typename _T, unsigned _N, unsigned _K>
	friend struct MESH::t_mesh;

	t_part(const t_mesh &_mesh, int _ind):
	      MESH(_mesh), ind(_ind) {}
	t_part(const t_mesh &_mesh):
	      MESH(_mesh) {}

	const t_mesh &MESH;
	int ind;
};

//Mesh structures:
template <typename T, unsigned N, unsigned M> struct t_mesh {

	template<unsigned K> using t_iter = MESH::t_iter<T, N, M, K>;
	template<unsigned K> using t_link = MESH::t_link<T, N, M, K>;
	template<unsigned K> using t_part = MESH::t_part<T, N, M, K>;
	typedef MESH::t_vert<T, N> t_vert;

	static_assert(N >= M, "Mesh dimension must not be more than space dimension!");

	template <typename ... TT> t_mesh(const std::vector<t_vert> &vert, TT && ... args) {
		DATA.VERT = std::make_shared<std::vector<t_vert>>(vert);
		DATA.GRID = std::make_shared<t_grid>();
		DATA.GRID->GRID = t_hand<M, 1>::make(
			std::forward<TT>(args) ... );
		init();
	}

	template <typename ... TT> t_mesh(std::vector<t_vert> &&vert, TT && ... args) {
		DATA.VERT = std::make_shared<std::vector<t_vert>>(
			std::move(vert));
		DATA.GRID = std::make_shared<t_grid>();
		DATA.GRID->GRID = t_hand<M, 1>::make(
			std::forward<TT>(args) ... );
		init();
	}

	t_mesh() {}

	//Data transform:
	template <typename ... TT> t_mesh ref(TT ... args) const { return t_mesh(DATA, [&](const auto &v) { return v.ref(args ...); }); }
	template <typename ... TT> t_mesh rot(TT ... args) const { return t_mesh(DATA, [&](const auto &v) { return v.rot(args ...); }); }
	t_mesh mov(const t_vector<T, N> &dir) const { return t_mesh(DATA, [&](const auto &v) { return v.mov(dir); }); }

	//Data access:
	template <unsigned I> const std::vector<std::vector<int>> &link() const { return DATA.GRID->GRID.template link<I>(); }
	template <unsigned I> t_link<I> link(int i) const { return t_link<I>(*this, i); }

	template <unsigned I> const std::vector<t_cell<I>> &cell() const { return DATA.GRID->GRID.template cell<I>(); }
	const std::vector<t_body> &body() const { return cell<3>(); }
	const std::vector<t_face> &face() const { return cell<2>(); }
	const std::vector<t_edge> &edge() const { return cell<1>(); }
	const std::vector<t_vert> &vert() const { return *DATA.VERT; }

	template <unsigned I> t_part<I> cell(int i) const { return t_part<I>(*this, i); }
	t_part<3> body(int i) const { return cell<3>(i); }
	t_part<2> face(int i) const { return cell<2>(i); }
	t_part<1> edge(int i) const { return cell<1>(i); }
	t_part<0> vert(int i) const { return cell<0>(i); }

	const MESH::t_grid<M> &grid() const { return DATA.GRID->GRID; }

	t_iter<M> begin() const {
		return t_iter<M>(*this, DATA.GRID->ITEM.data(), 0);
	}
	t_iter<M> end() const {
		return t_iter<M>(*this, DATA.GRID->ITEM.data(),
		DATA.GRID->ITEM.size(), false);
	}

private:
	template <typename _T, unsigned _N, unsigned _M>
	friend struct MESH::t_iter;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct MESH::t_link;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct MESH::t_part;

	struct t_grid { MESH::t_grid<M> GRID; std::vector<int> ITEM; };
	struct t_data {
		std::shared_ptr<std::vector<t_vert>> VERT;
		std::shared_ptr<t_grid> GRID;
	};

	t_mesh(const t_data &data, auto &&func) {
		DATA.VERT = std::make_shared<
		    std::vector<t_vert>>(data.VERT->size());
		DATA.GRID = data.GRID;
		std::transform(
		data.VERT->begin(), data.VERT->end(),
		DATA.VERT->begin(), func
		);
	}

	void init() {
		DATA.GRID->ITEM.resize(
		DATA.GRID->GRID.template cell<M>().size());
		std::iota(
		DATA.GRID->ITEM.begin(),
		DATA.GRID->ITEM.end(),
		0);
	}

	t_data DATA;
};

//...

}//MESH

}//GEOM
