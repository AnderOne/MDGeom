#pragma once
#include "base.hpp"
#include <valarray>
#include <vector>
#include <array>
#include <map>

namespace GEOM {

using namespace BASE;

namespace MESH {

template <typename T, unsigned N> using t_vert = t_vector<T, N>;

template <unsigned N> using t_cell = std::array<int, N + 1>;

typedef t_cell<3> t_body;
typedef t_cell<2> t_face;
typedef t_cell<1> t_edge;

//Forward declarations:
template <typename T, unsigned N, unsigned M>
struct t_mesh;
template <unsigned N>
struct t_grid;

//Handler classes:
template <unsigned N,
          unsigned M>
struct t_hand {

	static_assert((M > 0) && (M < N));

	template <typename ... TT>
	static t_grid<N> gen(const std::vector<t_cell<M>> &cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          gen(std::forward<TT>(args) ...));
		t_hand<N, M>::get(grid) = cell;
		return grid;
	}
	template <typename ... TT>
	static t_grid<N> gen(std::vector<t_cell<M>> &&cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          gen(std::forward<TT>(args) ...));
		t_hand<N, M>::get(grid) =
		          std::move(cell);
		return grid;
	}

	static const std::vector<t_cell<M>> &get(const t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
	static std::vector<t_cell<M>> &get(t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
};

template <unsigned N>
struct t_hand<N, N> {

	static const std::vector<t_cell<N>> &get(const t_grid<N> &grid) {
		return grid.CELL;
	}
	static std::vector<t_cell<N>> &get(t_grid<N> &grid) {
		return grid.CELL;
	}
	static t_grid<N> gen(const std::vector<t_cell<N>> &cell) {
		t_grid<N> grid; grid.CELL = cell; return grid;
	}
	static t_grid<N> gen(std::vector<t_cell<N>> &&cell) {
		t_grid<N> grid; grid.CELL = std::move(cell);
		return grid;
	}

};

//Grid structures:
template <unsigned N>
struct t_grid {

	template <unsigned M> const std::vector<t_cell<M>> &get() const {
		return t_hand<N, M>::get(*this);
	}
	template <unsigned M> std::vector<t_cell<M>> &get() {
		return t_hand<N, M>::get(*this);
	}

	std::vector<t_cell<N>> CELL;
	t_grid<N-1> GRID;
};

template <> struct t_grid<1> {

	std::vector<t_edge> CELL;
};

//Mesh structures:
template <typename T, unsigned N, unsigned M = N - 1>
struct t_mesh {

	static_assert(N >= M, "Mesh dimension must not be more than space dimension!");

	typedef MESH::t_vert<T, N>
	t_vert;

	template <typename ... TT> t_mesh(const std::vector<t_vert> &vert, TT && ... args):
	                           VERT(vert),
	                           GRID(std::move(t_hand<M, 1>::gen(
	                           std::forward<TT>(args) ...
	                           ))) {}

	template <typename ... TT> t_mesh(std::vector<t_vert> &&vert, TT && ... args):
	                           VERT(std::move(vert)),
	                           GRID(std::move(t_hand<M, 1>::gen(
	                           std::forward<TT>(args) ...
	                           ))) {}

	t_mesh() {}

	//Data transform:
	void rot(const t_vector<T, N> &center, int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(center, xaxis, yaxis, angle);
	}
	void rot(const t_basis<T, N> &basis, int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(basis, xaxis, yaxis, angle);
	}
	void rot(int xaxis, int yaxis, T angle) {
		for (auto &v : VERT) v = v.rot(xaxis, yaxis, angle);
	}
	void mov(const t_vector<T, N> &dir) {
		for (auto &v : VERT) v = v.mov(dir);
	}

	//Data access:
	template <unsigned I> const std::vector<t_cell<I>> &cell() const {
		return GRID.template get<I>();
	}
	const std::vector<t_body> &body() const { return cell<3>(); }
	const std::vector<t_face> &face() const { return cell<2>(); }
	const std::vector<t_edge> &edge() const { return cell<1>(); }
	const std::vector<t_vert> &vert() const { return VERT; }

	template <unsigned I> const t_cell<I> &cell(int i) const {
		return GRID.template get<I>()[i];
	}
	const t_body &body(int i) const { return cell<3>(i); }
	const t_face &face(int i) const { return cell<2>(i); }
	const t_edge &edge(int i) const { return cell<1>(i); }
	const t_vert &vert(int i) const { return VERT[i]; }

protected:
	template <unsigned I> void set(const std::vector<t_cell<I>> &it) {
		GRID.template get<I>() = it;
	}
	template <unsigned I> void set(std::vector<t_cell<I>> &&it) {
		GRID.template get<I>() = std::move(it);
	}
	void set(const std::vector<t_vert> &vt) {
		VERT = vt;
	}
	void set(std::vector<t_vert> &&vt) {
		VERT = std::move(vt);
	}

private:
	std::vector<t_vert> VERT;
	t_grid<M> GRID;
};

//...

typedef MESH::t_mesh<double, 4>
t_mesh_4d;
typedef MESH::t_mesh<double, 3>
t_mesh_3d;

//...

}//MESH

}//GEOM
