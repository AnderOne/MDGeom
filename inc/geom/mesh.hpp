#pragma once

#include "base.hpp"
#include <valarray>
#include <vector>
#include <array>

namespace GEOM {

namespace MESH {

typedef BASE::t_basis<double, 4, 3>
t_slice_3d;
typedef BASE::t_basis<double, 4>
t_space_4d;
typedef BASE::t_basis<double, 3>
t_space_3d;

using namespace BASE;

template <unsigned N> using t_vert = t_vector<double, N>;

template <unsigned N>
//using t_item = std::array<int, N + 1>;
struct t_item {
	template <typename ... TT> t_item(TT ... args):
	                           ind{args ...} {}

	std::ostream &print(std::ostream &out) const {
		out << ind.size();
		for (int i: ind) out << "\t" << i;
		return out;
	}
	const auto begin() const {
		return std::begin(ind);
	}
	const auto end() const {
		return std::end(ind);
	}
	size_t size() const {
		return ind.size();
	}
	const auto &operator[](int i) const {
		return ind[i];
	}
	auto &operator[](int i) {
		return ind[i];
	}
private:
	std::valarray<int> ind;
};

template <> struct t_item<1> {
	template <typename ... TT> t_item(TT ... args):
	                           ind{args ...} {}

	std::ostream &print(std::ostream &out) const {
		out << ind[0] << "\t" << ind[1];
		return out;
	}
	const auto begin() const {
		return std::begin(ind);
	}
	const auto end() const {
		return std::end(ind);
	}
	size_t size() const {
		return ind.size();
	}
	const auto &operator[](int i) const {
		return ind[i];
	}
	auto &operator[](int i) {
		return ind[i];
	}
private:
	std::array<int, 2> ind;
};

typedef t_item<1>
t_edge;
typedef t_item<2>
t_face;
typedef t_item<3>
t_cell;

template <unsigned N> struct t_grid;

template <unsigned N, unsigned M>
struct t_hand {

	static_assert((N > 0) && (M > 0) && (M < N));

	static const std::vector<t_item<M>> &get(const t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
	static std::vector<t_item<M>> &get(t_grid<N> &grid) {
		return t_hand<N-1, M>::get(grid.GRID);
	}
};

template <unsigned N>
struct t_hand<N, N> {

	static const std::vector<t_item<N>> &get(const t_grid<N> &grid) {
		return grid.ITEM;
	}
	static std::vector<t_item<N>> &get(t_grid<N> &grid) {
		return grid.ITEM;
	}
};

template <unsigned N>
struct t_grid {

	template <typename ... TT>
	t_grid(const std::vector<t_item<N>> &item, TT && ... args): 
	       ITEM(item), GRID(std::forward<TT>(args) ...) {}

	template <typename ... TT>
	t_grid(std::vector<t_item<N>> &&item, TT && ... args): 
	       ITEM(std::move(item)),
	       GRID(
	       std::forward<TT>(args)
	       ...
	       ) {}

	t_grid() {}

	std::ostream &print(std::ostream &out) const {
		GRID.print(out);
		out << ITEM.size() << "\n";
		for (const auto &i : ITEM) {
			i.print(out);
			out << "\n";
		}
		out << "\n";
		return out;
	}

	template <unsigned M>
	const std::vector<t_item<M>> &get() const {
		return t_hand<N, M>::get(*this);
	}
	template <unsigned M>
	std::vector<t_item<M>> &get() {
		return t_hand<N, M>::get(*this);
	}

	std::vector<t_item<N>> ITEM;
	t_grid<N-1> GRID;
};

template <> struct t_grid<1> {

	t_grid(const std::vector<t_item<1>> &item): ITEM(item) {}

	t_grid(std::vector<t_item<1>> &&item): 
	       ITEM(std::move(item)) {}

	t_grid() {}

	std::ostream &print(std::ostream &out) const {
		out << ITEM.size() << "\n";
		for (const auto &i : ITEM) {
			i.print(out);
			out << "\n";
		}
		out << "\n";
		return out;
	}

	std::vector<t_item<1>> ITEM;
};

template <unsigned N>
struct t_mesh {

	template <typename ... TT> t_mesh(const std::vector<t_vert<N>> &vert, TT && ... args):
	                           VERT(vert), GRID(std::forward<TT>(args) ...) {}

	template <typename ... TT> t_mesh(std::vector<t_vert<N>> &&vert, TT && ... args):
	                           VERT(std::move(vert)),
	                           GRID(std::forward<TT>(args) ...) {}

	t_mesh() {}

	//Data transform:
	void rot(const t_vector<double, N> &center, int xaxis, int yaxis, double angle) {
		for (auto &v : VERT) v = v.rot(center, xaxis, yaxis, angle);
	}
	void rot(const t_basis<double, N> &basis, int xaxis, int yaxis, double angle) {
		for (auto &v : VERT) v = v.rot(basis, xaxis, yaxis, angle);
	}
	void rot(int xaxis, int yaxis, double angle) {
		for (auto &v : VERT) v = v.rot(xaxis, yaxis, angle);
	}
	void mov(const t_vector<double, N> &dir) {
		for (auto &v : VERT) v = v.mov(dir);
	}

	std::ostream &print(std::ostream &out) const {
		out << VERT.size() << "\n";
		for (const auto &v : VERT) {
			out << v << "\n";
		}
		out << "\n";
		GRID.print(out);
		return out;
	}

	//Data access:
	template <unsigned M>
	const std::vector<t_item<M>> &item() const { return GRID.template get<M>(); }
	const std::vector<t_vert<N>> &vert() const { return VERT; }
	const std::vector<t_edge> &edge() const { return item<1>(); }
	const std::vector<t_face> &face() const { return item<2>(); }
	const std::vector<t_cell> &cell() const { return item<3>(); }

	template <unsigned M>
	const t_item<M> &item(int i) const {
		return GRID.template get<M>()[i];
	}
	const t_vert<N> &vert(int i) const {
		return VERT[i];
	}
	const t_edge &edge(int i) const {
		return item<1>(i);
	}
	const t_face &face(int i) const {
		return item<2>(i);
	}
	const t_cell &cell(int i) const {
		return item<3>(i);
	}

protected:
	template <unsigned M> void set(const std::vector<t_item<M>> &it) {
		GRID.template get<M>() = it;
	}
	template <unsigned M> void set(std::vector<t_item<M>> &&it) {
		GRID.template get<M>() = std::move(it);
	}
	void set(const std::vector<t_vert<N>> &vt) {
		VERT = vt;
	}
	void set(std::vector<t_vert<N>> &&vt) {
		VERT = std::move(vt);
	}

private:
	std::vector<t_vert<N>> VERT;
	t_grid<N-1> GRID;
};

typedef t_mesh<3>
t_mesh_3d;
typedef t_mesh<4>
t_mesh_4d;

//...

template <unsigned N>
std::ostream &operator<<(std::ostream &out, const t_mesh<N> &mesh) {
	return
	mesh.print(out);
}

//...

}//MESH
	
}//GEOM
