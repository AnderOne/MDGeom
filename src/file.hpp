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

	out << N << "\t" << M << "\n";
	out << "\n";

	out << mesh.vert().size();
	out << "\n";
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

//...

}//FILE

}//GEOM
