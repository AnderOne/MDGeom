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
template <typename T, unsigned N, unsigned M>
struct t_part;
template <typename T, unsigned N, unsigned M>
struct t_iter;

template <unsigned N>
struct t_grid;

//Handler classes:
template <unsigned N,
          unsigned M>
struct t_hand {

	static_assert((M >= 0) && (M < N));

	template <typename ... TT> static t_grid<N> make(const std::vector<t_cell<M>> &cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          make(std::forward<TT>(args) ...));
		t_hand<N, M>::cell(grid) = cell;
		return grid;
	}
	template <typename ... TT> static t_grid<N> make(std::vector<t_cell<M>> &&cell, TT && ... args) {
		t_grid<N> grid = std::move(t_hand<N, M + 1>::
		          make(std::forward<TT>(args) ...));
		t_hand<N, M>::cell(grid) =
		          std::move(cell);
		return grid;
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

	static const std::vector<t_cell<N>> &cell(const t_grid<N> &grid) { return grid.CELL; }
	static std::vector<t_cell<N>> &cell(t_grid<N> &grid) { return grid.CELL; }

	static const t_grid<N> &grid(const t_grid<N> &grid) { return grid; }
	static t_grid<N> &grid(t_grid<N> &grid) { return grid; }

	static t_grid<N> make(const std::vector<t_cell<N>> &cell) {
		t_grid<N> grid; grid.CELL = cell; return grid;
	}
	static t_grid<N> make(std::vector<t_cell<N>> &&cell) {
		t_grid<N> grid; grid.CELL = std::move(cell);
		return grid;
	}

};

//Grid structures:
template <unsigned N>
struct t_grid {

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

	std::vector<t_cell<N>> CELL;
	t_grid<N-1> GRID;
};

template <> struct
t_grid<0> {};

//Iterator:
template <typename T, unsigned N, unsigned M>
struct t_iter {

	bool operator==(const t_iter &other) const { return ind == other.ind; }
	bool operator!=(const t_iter &other) const { return ind != other.ind; }

	t_part<T, N, M> operator->() const { return t_part<T, N, M>(VERT, GRID.template grid<M>(), ITEM[ind]); }
	t_part<T, N, M> operator*() const { return t_part<T, N, M>(VERT, GRID.template grid<M>(), ITEM[ind]); }

	t_iter  operator++(int) { t_iter iter(*this); ++ ind; return iter; }
	t_iter  operator--(int) { t_iter iter(*this); -- ind; return iter; }
	t_iter &operator++() { ++ ind; return *this; }
	t_iter &operator--() { -- ind; return *this; }

private:
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_mesh;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_part;

	t_iter(const std::vector<t_vert<T, N>> &_vert, const int *_item, const t_grid<M> &_grid, int _ind): VERT(_vert), ITEM(_item), GRID(_grid), ind(_ind) {}

	const std::vector<t_vert<T, N>> &VERT;
	const int *ITEM;
	const t_grid<M> &GRID;
	int ind;
};

template <typename T, unsigned N>
struct t_iter<T, N, 0> {

	bool operator==(const t_iter &other) const { return ind == other.ind; }
	bool operator!=(const t_iter &other) const { return ind != other.ind; }

	t_part<T, N, 0> operator->() const { return t_part<T, N, 0>(VERT, ITEM[ind]); }
	t_part<T, N, 0> operator*() const { return t_part<T, N, 0>(VERT, ITEM[ind]); }

	t_iter  operator++(int) { t_iter iter(*this); ++ ind; return iter; }
	t_iter  operator--(int) { t_iter iter(*this); -- ind; return iter; }
	t_iter &operator++() { ++ ind; return *this; }
	t_iter &operator--() { -- ind; return *this; }

private:
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_mesh;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_part;

	t_iter(const std::vector<t_vert<T, N>> &_vert, const int *_item, const t_grid<0> &_grid, int _ind): VERT(_vert), ITEM(_item), ind(_ind) {}

	const std::vector<t_vert<T, N>> &VERT;
	const int *ITEM;
	int ind;
};

//Proxy:
template <typename T, unsigned N, unsigned M>
struct t_part {

	t_iter<T, N, M - 1> begin() const {
		return t_iter<T, N, M - 1>(
		VERT, GRID.template cell<M>()[ind].data(), GRID.template grid<M - 1>(), 0
		);
	}
	t_iter<T, N, M - 1> end() const {
		return t_iter<T, N, M - 1>(
		VERT, GRID.template cell<M>()[ind].data(), GRID.template grid<M - 1>(),
		GRID.template cell<M>()[ind].size()
		);
	}

	const t_cell<M> &cell() const { return GRID.template cell<M>()[ind]; }
	int cell(int i) const { return GRID.template cell<M>()[ind][i]; }

	int id() const { return ind; }

private:
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_mesh;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_part;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_iter;

	t_part(const std::vector<t_vert<T, N>> &_vert, const t_grid<M> &_grid, int _ind): VERT(_vert), GRID(_grid), ind(_ind) {}

	const std::vector<t_vert<T, N>> &VERT;
	const t_grid<M> &GRID;
	int ind;
};

template <typename T, unsigned N>
struct t_part<T, N, 0> {

	const t_vert<T, N> &data() const { return VERT[ind]; }
	int id() const { return ind; }

private:
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_mesh;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_part;
	template <typename _T, unsigned _N, unsigned _M>
	friend struct t_iter;

	t_part(const std::vector<t_vert<T, N>> &_vert, int _ind): VERT(_vert), ind(_ind) {}

	const std::vector<t_vert<T, N>> &VERT;
	int ind;
};

//Mesh structures:
template <typename T, unsigned N, unsigned M = N - 1>
struct t_mesh {

	typedef MESH::t_vert<T, N> t_vert;

	static_assert(N >= M, "Mesh dimension must not be more than space dimension!");

	template <typename ... TT> t_mesh(const std::vector<t_vert> &vert, TT && ... args):
	                           VERT(vert), GRID(std::move(t_hand<M, 1>::make(std::forward<TT>(args) ...))) { init(); }

	template <typename ... TT> t_mesh(std::vector<t_vert> &&vert, TT && ... args):
	                           VERT(std::move(vert)),
	                           GRID(std::move(t_hand<M, 1>::make(std::forward<TT>(args) ...))) { init(); }

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
	template <unsigned I> const std::vector<t_cell<I>> &cell() const { return GRID.template cell<I>(); }
	const std::vector<t_body> &body() const { return cell<3>(); }
	const std::vector<t_face> &face() const { return cell<2>(); }
	const std::vector<t_edge> &edge() const { return cell<1>(); }
	const std::vector<t_vert> &vert() const { return VERT; }

	template <unsigned I> t_part<T, N, I> cell(int i) const {
		return t_part<T, N, I>(VERT, GRID.template grid<I>(), i);
	}
	t_part<T, N, 3> body(int i) const { return cell<3>(i); }
	t_part<T, N, 2> face(int i) const { return cell<2>(i); }
	t_part<T, N, 1> edge(int i) const { return cell<1>(i); }
	const t_vert &vert(int i) const { return VERT[i]; }

	t_iter<T, N, M> begin() const {
		return t_iter<T, N, M>(VERT, ITEM.data(), GRID, 0);
	}
	t_iter<T, N, M> end() const {
		return t_iter<T, N, M>(
		VERT, ITEM.data(), GRID, ITEM.size()
		);
	}

protected:
	template <unsigned I> void set(const std::vector<t_cell<I>> &it) {
		GRID.template cell<I>() = it;
	}
	template <unsigned I> void set(std::vector<t_cell<I>> &&it) {
		GRID.template cell<I>() = std::move(it);
	}
	void set(const std::vector<t_vert> &vt) {
		VERT = vt;
	}
	void set(std::vector<t_vert> &&vt) {
		VERT = std::move(vt);
	}

private:
	void init() {
		ITEM.resize(GRID.template cell<M>().size());
		std::iota(ITEM.begin(), ITEM.end(), 0);
	}

	std::vector<t_vert> VERT;
	std::vector<int> ITEM;
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
