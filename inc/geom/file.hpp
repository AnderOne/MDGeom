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
#include "mesh.hpp"
#include <ostream>

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace FILE {

template <typename T, unsigned N, unsigned M>
std::ostream &operator << (std::ostream &out, const t_mesh<T, N, M> &mesh) {

	out << mesh.vert().size() << "\n";
	for (auto v: mesh.vert()) {
		out << v << "\n";
	}
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
