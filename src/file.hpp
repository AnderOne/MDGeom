#pragma once
#include <geom/base.hpp>
#include <geom/mesh.hpp>
#include <ostream>

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace FILE {

template <typename T, unsigned N, unsigned M>
std::ostream &operator << (std::ostream &out, const t_mesh<T, N, M> &mesh) {

	out << N << "\t" << M << "\n" << "\n" << mesh.vert().size() << "\n";
	for (auto v: mesh.vert()) out << v << "\n";
	out << "\n";

	out << mesh.grid();

	return out;
}

template <unsigned N>
std::ostream &operator << (std::ostream &out, const t_grid<N> &grid) {

	out << grid.template grid<N - 1>() << "\n";

	const auto &cell = grid.template cell<N>();
	out << cell.size() << "\n";
	for (auto c: cell) {
		out << c.size();
		for (int i: c) out << "\t" << i;
		out << "\n";
	}
	return out;
}	

std::ostream &operator << (std::ostream &out, const t_grid<1> &grid) {

	const auto &edge = grid.cell<1>();
	out << edge.size() << "\n";
	for (auto e: edge) {
		out << e[0] << "\t";
		out << e[1] << "\n";
	}
	return out;
}

template <typename T, unsigned N, unsigned M>
std::istream &operator >> (std::istream &src, t_mesh<T, N, M> &mesh) {

	size_t n, m; src >> n >> m;
	if ((n != N) || (m != M)) {
		src.setstate(std::ios_base::badbit);
		return src;
	}

	src >> n; std::vector<typename t_mesh<T, N, M>::t_vert> vert(n);
	for (int i = 0; i < n; ++ i)
	for (int k = 0; k < N; ++ k) {
		src >> vert[i][k];
	}

	t_grid<M> grid;
	src >> grid;

	mesh = t_mesh<T, N, M>(
		std::move(vert),
		std::move(grid)
	);

	return src;
}

template <unsigned N>
std::istream &operator >> (std::istream &src, t_grid<N> &grid) {

	src >> grid.template grid<N - 1>();

	auto &cell = grid.template cell<N>();
	size_t n; src >> n; cell.resize(n);
	for (auto &c: cell) {
	size_t m; src >> m; c.resize(m);
	for (int &i: c) src >> i;
	}

	return src;
}	

std::istream &operator >> (std::istream &src, t_grid<1> &grid) {

	auto &edge = grid.cell<1>();
	size_t n; src >> n;
	edge.resize(n);
	for (auto &e: edge) {
	src >> e[0];
	src >> e[1];
	}

	return src;
}

//...

}//FILE

}//GEOM
