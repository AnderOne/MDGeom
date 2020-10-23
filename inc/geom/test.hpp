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
#include <map>
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
	if (tree.find(t_rect<T, N>{vert - eps, vert + eps}).size() > 1) {
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

template <unsigned N> bool checkEqual(const t_grid<N> &grid1, const std::map<size_t, size_t> &vertMap1,
                                      std::map<size_t, size_t> &cellMap1,
                                      const t_grid<N> &grid2, const std::map<size_t, size_t> &vertMap2,
                                      std::map<size_t, size_t> &cellMap2);

template <> bool checkEqual(const t_grid<1> &grid1, const std::map<size_t, size_t> &vertMap1,
                            std::map<size_t, size_t> &cellMap1,
                            const t_grid<1> &grid2, const std::map<size_t, size_t> &vertMap2,
                            std::map<size_t, size_t> &cellMap2);

template <typename T, unsigned N, unsigned M>
bool checkEqual(const t_mesh<T, N, M> &mesh1, const t_mesh<T, N, M> &mesh2, double eps = MATH_EPSILON) {

	std::map<size_t, size_t> vertMap1;
	std::map<size_t, size_t> vertMap2;

	const auto &vert1 = mesh1.vert();
	const auto &vert2 = mesh2.vert();
	const auto &tree1 = mesh1.tree();

	for (int i = 0; i < vert1.size(); ++ i) {
		const auto &iter = vertMap1.find(i); size_t id0 = (iter != vertMap1.end())? (iter->second): (i);
		const auto &list = tree1.find(t_rect<T, N>{vert1[i] - eps, vert1[i] + eps});
		for (const auto &j: list) {
			const auto &it = vertMap1.find(j);
			if ((it != vertMap1.end()) && (it->second != id0)) { return false; }
		    vertMap1[j] = id0;
		}
	}

	for (int i = 0; i < vert2.size(); ++ i) {

		const auto &list = tree1.find(t_rect<T, N>{vert2[i] - eps, vert2[i] + eps});
		if (list.empty()) { return false; }

		auto id0 = vertMap1[list.front()];
		for (const auto &j: list) {
			const auto &it = vertMap1.find(j);
			if ((it != vertMap1.end()) && (it->second != id0)) { return false; }
		}
		vertMap2[i] = id0;
	}

	std::map<size_t, size_t> cellMap1;
	std::map<size_t, size_t> cellMap2;

	return checkEqual<N>(
	    mesh1.grid(), vertMap1,
	    cellMap1,
	    mesh2.grid(), vertMap2,
	    cellMap2
	);
}

template <unsigned N>
bool checkEqual(const std::vector<t_cell<N>> &cellList1, const std::map<size_t, size_t> &itemMap1,
                std::map<size_t, size_t> &cellMap1,
                const std::vector<t_cell<N>> &cellList2, const std::map<size_t, size_t> &itemMap2,
                std::map<size_t, size_t> &cellMap2) {

	std::map<std::set<size_t>, size_t> cellInd;
	size_t maxInd = 0;

	cellMap1.clear(); cellMap2.clear();

	auto getCellMap = [&](const auto &cellList, const auto &itemMap, auto &cellMap) {

		for (int i = 0; i < cellList.size(); ++ i) {
			std::set<size_t> itemSet;
			for (auto item: cellList[i]) itemSet.insert(itemMap.at(item));
			auto it = cellInd.find(itemSet);
			if (it == cellInd.end()) {
				cellInd[itemSet] = maxInd;
				cellMap[i] = maxInd ++;
			}
			else {
				cellMap[i] = it->second;
			}
		}
	};

	auto getCellSet = [&](const auto &cellMap) {

		std::set<size_t> cellSet;
		for (const auto &it: cellMap) cellSet.insert(it.second);
		return cellSet;
	};

	getCellMap(
		cellList1, itemMap1, cellMap1
	);
	getCellMap(
		cellList2, itemMap2, cellMap2
	);
	const auto &set1 = getCellSet(
		cellMap1
	);
	const auto &set2 = getCellSet(
		cellMap2
	);
	return (set1 == set2);
}

template <unsigned N>
bool checkEqual(const t_grid<N> &grid1, const std::map<size_t, size_t> &vertMap1,
                std::map<size_t, size_t> &cellMap1,
                const t_grid<N> &grid2, const std::map<size_t, size_t> &vertMap2,
                std::map<size_t, size_t> &cellMap2) {

	std::map<size_t, size_t> itemMap1, itemMap2;
	if (!checkEqual<N-1>(
	    grid1.template grid<N-1>(), vertMap1,
	    itemMap1,
	    grid2.template grid<N-1>(), vertMap2,
	    itemMap2)) {
	    return false;
	}

	return checkEqual<1>(
	    grid1.template cell<1>(), itemMap1,
	    cellMap1,
	    grid2.template cell<1>(), itemMap2,
	    cellMap2
	);
}

template <>
bool checkEqual(const t_grid<1> &grid1, const std::map<size_t, size_t> &vertMap1,
                std::map<size_t, size_t> &cellMap1,
                const t_grid<1> &grid2, const std::map<size_t, size_t> &vertMap2,
                std::map<size_t, size_t> &cellMap2) {

	return checkEqual<1>(
	    grid1.template cell<1>(), vertMap1,
	    cellMap1,
	    grid2.template cell<1>(), vertMap2,
	    cellMap2
	);
}

//...

}//TEST

}//GEOM
