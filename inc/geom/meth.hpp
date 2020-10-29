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

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace METH {

template <unsigned N, unsigned M> struct t_sect_reduce { constexpr static int dim = ((M == N)? (N - 1): (M)); };

constexpr int nullind = -1;

//Метод проецирования сетки на подпространство:
template <typename T, unsigned N,
                      unsigned M>
auto getProject(const t_mesh<T, N, M> &mesh, const t_basis<T, N, N - 1> &basis) {

	typedef t_mesh<T, N - 1, t_sect_reduce<N, M>::dim> t_projected_mesh;
	typedef typename t_projected_mesh::t_vert t_projected_vert;

	const std::vector<t_vector<T, N>> &old_vert = mesh.vert();
	std::vector<t_projected_vert> new_vert(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {
		new_vert[i] = basis.put(old_vert[i]);
	}

	return t_projected_mesh{
	std::move(new_vert),
	mesh.grid()
	};
}


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

enum class t_state { CROSS, LOWER, UPPER, INNER };

typedef std::vector<int> t_child;

template <unsigned N, unsigned M, unsigned K, bool SLICE_ONLY = false>
struct t_sect_builder {

	t_sect_builder(const t_grid<N> &_old_grid, t_grid<M> &_new_grid):
	               old_grid(_old_grid), new_grid(_new_grid) {}

	void make_new_item(const std::vector<t_state> &old_item_state,
	                   const std::vector<t_child> &new_item_child,
	                   const std::vector<int> &new_item_index,
	                   std::vector<t_state> &old_cell_state,
	                   std::vector<t_child> &new_cell_child,
	                   std::vector<int> &new_cell_index) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_item = new_grid.template cell<K>();

		for (int i = 0; i < old_cell.size(); ++ i) {

			//Проверяем положение ячейки относительно подпространства:
			size_t cross = 0, lower = 0, upper = 0, inner = 0;
			for (int c: old_cell[i]) {
				if (old_item_state[c] == t_state::CROSS) { ++ cross; }
				if (old_item_state[c] == t_state::LOWER) { ++ lower; }
				if (old_item_state[c] == t_state::UPPER) { ++ upper; }
				if (old_item_state[c] == t_state::INNER) {
					assert(new_item_index[c] != nullind);
					new_cell_child[i].
					push_back(new_item_index[c]);
					++ inner;
				}
			}
			if (inner == old_cell[i].size()) old_cell_state[i] = t_state::INNER;
			else
			if (cross || (upper && lower)) old_cell_state[i] = t_state::CROSS;
			else {
				old_cell_state[i] = lower? t_state::LOWER: t_state::UPPER;
			}

			if (old_cell_state[i] != t_state::CROSS) continue;

			//Добавляем новую подъячейку:
			std::set<int> item;
			for (int c: old_cell[i]) for (int k: new_item_child[c]) item.insert(k);
			t_push<K> push;
			for (int k: item) push.add(k);

			assert(push.pos > K);

			new_cell_child[i].push_back(
				new_item.size()
			);
			new_item.push_back(
			push.item
			);
		}
	}

	void make_new_cell(const std::vector<t_state> &old_item_state,
	                   const std::vector<t_child> &new_item_child,
	                   const std::vector<int> &new_item_index,
	                   std::vector<t_state> &old_cell_state,
	                   std::vector<t_child> &new_cell_child,
	                   std::vector<int> &new_cell_index) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_cell = new_grid.template cell<K + 1>();

		for (int i = 0; i < old_cell.size(); ++ i) {

			if (!SLICE_ONLY) {
				if ((old_cell_state[i] != t_state::INNER) && (old_cell_state[i] != t_state::UPPER) &&
				    (old_cell_state[i] != t_state::CROSS)) continue;
			}
			else {
				if (old_cell_state[i] != t_state::INNER) continue;
			}

			std::set<int> item;
			if (!SLICE_ONLY) for (int k: new_cell_child[i]) item.insert(k);
			for (int c: old_cell[i]) {
			int k = new_item_index[c]; if (k != nullind) item.insert(k);
			}
			t_cell<K + 1> cell;
			for (int k: item) cell.push_back(k);

			//Добавляем новую ячейку:
			new_cell_index[i] =
			new_cell.size();
			new_cell.push_back(
			cell
			);
		}
	}

	void make(const std::vector<t_state> &old_item_state,
	          const std::vector<t_child> &new_item_child,
	          const std::vector<int> &new_item_index) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_item = new_grid.template cell<K>();

		std::vector<t_state> old_cell_state(old_cell.size());
		std::vector<t_child> new_cell_child(old_cell.size());
		std::vector<int> new_cell_index(
			old_cell.size(), nullind
		);

		//Заполняем новые подъячейки меньшей размерности:
		make_new_item(
			old_item_state, new_item_child, new_item_index,
			old_cell_state, new_cell_child, new_cell_index
		);
		//Заполняем новые ячейки из подъячеек:
		t_sect_builder<N, M, (K < M)? (K): (N), SLICE_ONLY>
		(old_grid, new_grid).make_new_cell(
			old_item_state, new_item_child, new_item_index,
			old_cell_state, new_cell_child, new_cell_index
		);
		//Вызываемся рекурсивно вверх:
		t_sect_builder<N, M, K + 1, SLICE_ONLY>
		(old_grid, new_grid).make(
			old_cell_state, new_cell_child, new_cell_index
		);

		//Заполняем обратные ссылки:
		t_hand<M, K>::fill(
		new_grid, new_item
		);
	}

private:
	const t_grid<N> &old_grid;
	t_grid<M> &new_grid;
};

template <unsigned N, unsigned M, bool SLICE_ONLY>
struct t_sect_builder<N, M, N, SLICE_ONLY> {

	template <typename ... TT> t_sect_builder(TT ... args) {}
	template <typename ... TT>
	void make_new_cell(TT ... args) {}
	template <typename ... TT>
	void make(TT ... args) {}

};

//Метод отсечения гиперплоскостью:
template <typename T, unsigned N,
                      unsigned M>
auto getClipped(const t_mesh<T, N, M> &mesh, const t_vector<T, N> &center,
                                             const t_vector<T, N> &normal) {

	//Разделяем вершины относительно подпространства:
	const auto &old_vert = mesh.vert(); std::vector<t_vert<T, N>> new_vert;

	std::vector<int> new_vert_index(old_vert.size(), nullind);
	std::vector<int> old_vert_state(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {

		const T p = (old_vert[i] - center) * normal;

		if (p == 0) {
			new_vert_index[i] = new_vert.size();
			new_vert.push_back(old_vert[i]);
			old_vert_state[i] = +0;
		}
		else
		if (p > 0) {
			new_vert_index[i] = new_vert.size();
			new_vert.push_back(old_vert[i]);
			old_vert_state[i] = +1;
		}
		else
		if (p < 0) {
			old_vert_state[i] = -1;
		}
	}

	//Разрезаем ребра подпространством:
	const auto &old_grid = mesh.grid(); t_grid<M> new_grid;
	const auto &old_edge = old_grid.template cell<1>();
	auto &new_edge = new_grid.template cell<1>();

	std::vector<t_state> old_edge_state(old_edge.size());
	std::vector<t_child> new_edge_child(old_edge.size());
	std::vector<int> new_edge_index(
		old_edge.size(), nullind
	);

	for (int i = 0; i < old_edge.size(); ++ i) {

		const auto &edge = old_edge[i]; const int a = edge[0], b = edge[1];

		if (old_vert_state[a] == 0) {
			new_edge_child[i].push_back(new_vert_index[a]);
		}
		if (old_vert_state[b] == 0) {
			new_edge_child[i].push_back(new_vert_index[b]);
		}
		if (old_vert_state[a] * old_vert_state[b] < 0)
			old_edge_state[i] = t_state::CROSS;
		else
		if (old_vert_state[a] + old_vert_state[b] < 0)
			old_edge_state[i] = t_state::LOWER;
		else
		if (old_vert_state[a] + old_vert_state[b] > 0)
			old_edge_state[i] = t_state::UPPER;
		else {
			old_edge_state[i] = t_state::INNER;
		}

		if (old_edge_state[i] == t_state::CROSS) {
			const auto &pa = old_vert[a], &pb = old_vert[b];
			T p = - ((pa - center) * normal) /
			        ((pb - pa) * normal);
			//Add new vert:
			new_edge_child[i].push_back(new_vert.size());
			new_vert.push_back(
			pa + p * (pb - pa)
			);
			//Add new edge (short of edge):
			new_edge_index[i] = new_edge.size();
			int va = (old_vert_state[a] > 0)?
			          new_vert_index[a]:
			          new_vert.size() - 1;
			int vb = (old_vert_state[b] > 0)?
			          new_vert_index[b]:
			          new_vert.size() - 1;
			new_edge.push_back({va, vb});
		}
		if (old_edge_state[i] == t_state::INNER ||
		    old_edge_state[i] == t_state::UPPER) {
			//Add new edge:
			new_edge_index[i] = new_edge.size();
			new_edge.push_back({
			new_vert_index[a],
			new_vert_index[b]
			});
		}
	}

	//Вызываемся рекурсивно вверх:
	t_sect_builder<M, M, 1, false>(old_grid, new_grid).make(
	old_edge_state, new_edge_child, new_edge_index
	);

	return t_mesh<T, N, M>(
	std::move(new_vert),
	new_grid
	);
}

//Метод сечения гиперплоскостью:
template <typename T, unsigned N,
                      unsigned M>
auto getSection(const t_mesh<T, N, M> &mesh, const t_basis<T, N, N - 1> &basis) {

	typedef t_mesh<T, N - 1, t_sect_reduce<N, M>::dim> t_sliced_mesh;
	typedef typename t_sliced_mesh::t_vert t_sliced_vert;
	typedef t_grid<t_sect_reduce<N, M>::dim> t_sliced_grid;

	t_basis<T, N, N> extBasis = basis.template ext<N>();
	const auto &center = extBasis.center();
	const auto &normal = extBasis[N - 1];

	//Разделяем вершины относительно подпространства:
	const auto &old_vert = mesh.vert();
	std::vector<t_sliced_vert> new_vert;

	std::vector<int> new_vert_index(old_vert.size(), nullind);
	std::vector<int> old_vert_state(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {

		const T p = (old_vert[i] - center) * normal;

		if (p == 0) {
			new_vert_index[i] = new_vert.size();
			new_vert.push_back(
				basis.put(old_vert[i])
			);
			old_vert_state[i] = +0;
		}
		if (p > 0) {
			old_vert_state[i] = +1;
		}
		if (p < 0) {
			old_vert_state[i] = -1;
		}
	}

	//Разрезаем ребра подпространством:
	const auto &old_grid = mesh.grid(); t_sliced_grid new_grid;
	const auto &old_edge = old_grid.template cell<1>();
	auto &new_edge = new_grid.template cell<1>();

	std::vector<t_state> old_edge_state(old_edge.size());
	std::vector<t_child> new_edge_child(old_edge.size());
	std::vector<int> new_edge_index(
		old_edge.size(), nullind
	);

	for (int i = 0; i < old_edge.size(); ++ i) {

		const auto &edge = old_edge[i]; const int a = edge[0], b = edge[1];

		if (old_vert_state[a] == 0) {
			new_edge_child[i].push_back(new_vert_index[a]);
		}
		if (old_vert_state[b] == 0) {
			new_edge_child[i].push_back(new_vert_index[b]);
		}
		if (old_vert_state[a] * old_vert_state[b] < 0)
			old_edge_state[i] = t_state::CROSS;
		else
		if (old_vert_state[a] + old_vert_state[b] < 0)
			old_edge_state[i] = t_state::LOWER;
		else
		if (old_vert_state[a] + old_vert_state[b] > 0)
			old_edge_state[i] = t_state::UPPER;
		else {
			old_edge_state[i] = t_state::INNER;
		}

		if (old_edge_state[i] == t_state::CROSS) {
			const auto &pa = old_vert[a], &pb = old_vert[b];
			T p = - ((pa - center) * normal) /
			        ((pb - pa) * normal);
			//Add new vert:
			new_edge_child[i].push_back(new_vert.size());
			new_vert.push_back(
			basis.put(pa + p * (pb - pa))
			);
		}
		if (old_edge_state[i] == t_state::INNER) {
			//Add new edge:
			new_edge_index[i] = new_edge.size();
			new_edge.push_back({
			new_vert_index[a],
			new_vert_index[b]
			});
		}
	}

	//Вызываемся рекурсивно вверх:
	constexpr unsigned L = t_sect_reduce<N, M>::dim;
	t_sect_builder<M, L, 1, true>(old_grid, new_grid
	).make(
	old_edge_state, new_edge_child, new_edge_index
	);

	return t_sliced_mesh(
	std::move(new_vert),
	new_grid
	);
}

//...

}//METH

}//GEOM
