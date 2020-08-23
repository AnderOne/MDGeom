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
#include <set>

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace TEST {

template <unsigned N> bool checkDuplicate(const t_grid<N> &grid);

template <> bool checkDuplicate(const t_grid<1> &grid);


template <typename T, unsigned N, unsigned M>
bool checkDuplicate(const t_mesh<T, N, M> &mesh, double eps = MATH_EPSILON) {

	const auto &tree = mesh.tree();
	for (auto &vert: mesh.vert())
	if (tree.find(t_rect<T, N>{vert - eps,vert + eps}).size() > 1) {
	    return false;
	}
	return checkDuplicate(
	mesh.grid()
	);
}

template <unsigned N>
bool checkDuplicate(const std::vector<t_cell<N>> &cell_list) {

	std::set<std::set<int>> cell_set;
	for (auto &cell: cell_list) {
		std::set<int> item_set;
		for (int item: cell) {
		    if (item_set.count(item)) { return false; }
		    item_set.insert(item);
		}
		if (cell_set.count(item_set)) { return false; }
		cell_set.emplace(
		std::move(item_set)
		);
	}
	return true;
}

template <unsigned N>
bool checkDuplicate(const t_grid<N> &grid) {
	return checkDuplicate<N>(grid.template cell<N>()) &&
	       checkDuplicate(grid.template grid<N-1>());
}

template <>
bool checkDuplicate(const t_grid<1> &grid) {
	return checkDuplicate<1>(
	grid.template cell<1>()
	);
}

//...

}//TEST

}//GEOM
