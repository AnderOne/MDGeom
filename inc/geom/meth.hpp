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

template <unsigned N, unsigned M, unsigned K, bool IS_SLICE = false> struct t_sect_builder {

	t_sect_builder(const t_grid<N> &_old_grid, t_grid<M> &_new_grid):
	               old_grid(_old_grid), new_grid(_new_grid) {}

	void make_new_cell(const std::vector<int> &old_item_index,
	                   const std::vector<int> &new_cell_child,
	                   std::vector<int> &old_cell_index) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_cell = new_grid.template cell<K + 1>();

		for (int i = 0; i < old_cell.size(); ++ i) {
			t_cell<K + 1> cell;
			for (int c: old_cell[i]) {
				int k = old_item_index[c]; if (k != nullind) cell.push_back(k);
			}
			bool comp = (cell.size() == old_cell[i].size());
			int k = new_cell_child[i];
			if (!IS_SLICE && (k != nullind)) {
				cell.push_back(k);
				comp = true;
			}
			if (comp) {
				old_cell_index[i] =
				new_cell.size();
				new_cell.push_back(
				cell
				);
			}
		}
	}

	void make_new_item(const std::vector<int> &new_item_child, std::vector<int> &new_cell_child) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_item = new_grid.template cell<K>();

		for (int i = 0; i < old_cell.size(); ++ i) {
			t_push<K> push;
			for (int c: old_cell[i]) {
				int k = new_item_child[c]; if (k != nullind) push.add(k);
			}
			if (push.pos > K) {
				new_cell_child[i] =
				new_item.size();
				new_item.push_back(
				push.item
				);
			}
		}
	}

	void make(const std::vector<int> &old_item_index, const std::vector<int> &new_item_child) {

		const auto &old_cell = old_grid.template cell<K + 1>();
		auto &new_item = new_grid.template cell<K>();

		std::vector<int> old_cell_index(
			old_cell.size(), nullind
		);
		std::vector<int> new_cell_child(
			old_cell.size(), nullind
		);

		//Заполняем новые подъячейки меньшей размерности:
		make_new_item(
			new_item_child, new_cell_child
		);
		//Заполняем новые ячейки из подъячеек:
		t_sect_builder<N, M, (K < M)? (K): (N), IS_SLICE>(old_grid, new_grid).make_new_cell(
			old_item_index, new_cell_child,
			old_cell_index
		);
		//Вызываемся рекурсивно вверх:
		t_sect_builder<N, M, K + 1, IS_SLICE>(old_grid, new_grid).make(
			old_cell_index,
			new_cell_child
		);

		//Заполняем обратные ссылки:
		t_hand<M, K>::fill(
			new_grid,
			new_item
		);
	}

private:
	const t_grid<N> &old_grid;
	t_grid<M> &new_grid;
};

template <unsigned N, unsigned M, bool IS_SLICE> struct t_sect_builder<N, M, N, IS_SLICE> {

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

	std::vector<int> old_vert_index(old_vert.size(), nullind);
	std::vector<int> old_vert_state(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {

		const T p = (old_vert[i] - center) * normal;

		if (p == 0) {
			old_vert_index[i] = new_vert.size();
			new_vert.push_back(old_vert[i]);
			old_vert_state[i] = +0;
		}
		else
		if (p > 0) {
			old_vert_index[i] = new_vert.size();
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

	std::vector<int> old_edge_index(old_edge.size(), nullind);
	std::vector<int> new_edge_child(old_edge.size(), nullind);

	for (int i = 0; i < old_edge.size(); ++ i) {

		const auto &edge = old_edge[i];
		const int a = edge[0];
		const int b = edge[1];

		if (old_vert_state[a] * old_vert_state[b] < 0) {
			const auto &pa = old_vert[a];
			const auto &pb = old_vert[b];
			T p = - ((pa - center) * normal) /
			        ((pb - pa) * normal);
			//Add new vert:
			new_edge_child[i] = new_vert.size();
			new_vert.push_back(
			pa + p * (pb - pa)
			);
			//Add new edge:
			old_edge_index[i] = new_edge.size();
			int va = (old_vert_state[a] >= 0)?
			          old_vert_index[a]:
			          new_edge_child[i];
			int vb = (old_vert_state[b] >= 0)?
			          old_vert_index[b]:
			          new_edge_child[i];
			new_edge.push_back({
				va, vb
			});
		}
		else
		if ((old_vert_state[a] >= 0) &&
		    (old_vert_state[b] >= 0)) {
			//Add new edge:
			old_edge_index[i] = new_edge.size();
			new_edge.push_back({
			old_vert_index[a],
			old_vert_index[b]
			});
		}
	}

	//Вызываемся рекурсивно вверх:
	t_sect_builder<M, M, 1>(old_grid, new_grid
	).make(
	old_edge_index, new_edge_child
	);

	return t_mesh<T, N, M>(
	std::move(new_vert), new_grid
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

	std::vector<int> old_vert_index(old_vert.size(), nullind);
	std::vector<int> old_vert_state(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {

		const T p = (old_vert[i] - center) * normal;

		if (p == 0) {
			old_vert_index[i] = new_vert.size();
			new_vert.push_back(
			basis.put(old_vert[i])
			);
			old_vert_state[i] = +0;
		}
		else
		if (p > 0) {
			old_vert_state[i] = +1;
		}
		else
		if (p < 0) {
			old_vert_state[i] = -1;
		}
	}

	//Разрезаем ребра подпространством:
	const auto &old_grid = mesh.grid(); t_sliced_grid new_grid;
	const auto &old_edge = old_grid.template cell<1>();
	auto &new_edge = new_grid.template cell<1>();

	std::vector<int> old_edge_index(old_edge.size(), nullind);
	std::vector<int> new_edge_child(old_edge.size(), nullind);

	for (int i = 0; i < old_edge.size(); ++ i) {

		const auto &edge = old_edge[i];
		const int a = edge[0];
		const int b = edge[1];

		if (old_vert_state[a] * old_vert_state[b] < 0) {
			const auto &pa = old_vert[a];
			const auto &pb = old_vert[b];
			T p = - ((pa - center) * normal) /
			        ((pb - pa) * normal);
			//Add new vert:
			new_edge_child[i] = new_vert.size();
			new_vert.push_back(
			basis.put(
			pa + p * (pb - pa))
			);
		}
		else
		if ((old_vert_state[a] == 0) &&
		    (old_vert_state[b] == 0)) {
			//Add new edge:
			old_edge_index[i] = new_edge.size();
			new_edge.push_back({
			old_vert_index[a],
			old_vert_index[b]
			});
		}
	}

	//Вызываемся рекурсивно вверх:
	constexpr unsigned L = t_sect_reduce<N, M>::dim;
	t_sect_builder<M, L, 1, true>(old_grid, new_grid
	).make(
	old_edge_index, new_edge_child
	);

	return t_sliced_mesh(
	std::move(new_vert), new_grid
	);
}

//...

}//METH

}//GEOM
