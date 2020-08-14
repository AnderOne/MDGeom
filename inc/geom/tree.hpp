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
#include <algorithm>
#include <vector>
#include <array>
#include <memory>

namespace GEOM {

using namespace BASE;

//Содержит реализацию сортирующего двоичного дерева (KD-Tree)
namespace TREE {

template<typename T, unsigned N> struct t_tree {

	typedef BASE::t_vector<T, N> t_vert;
	typedef BASE::t_rect<T, N> t_rect;

	std::vector<const t_vert *> find(const t_rect &_rect) const { std::vector<const t_vert *> LIST; find(LIST, ROOT.get(), _rect, 0); return LIST; }

	explicit t_tree(const t_vert *_vert, size_t _num): VERT(_vert), ROOT(build(const_cast<t_vert *> (_vert), _num)) {}

private:
	struct t_node {
		explicit t_node(t_node *p1, t_node *p2, const t_vert *v): NODE{p1, p2}, VERT{v} {}

		~t_node() { for (auto &p: NODE) delete p; }

		std::array<t_node*, 2> NODE;
		const t_vert *VERT;
	};

	void find(std::vector<const t_vert *> &list, t_node *node, const t_rect &rect, int step) const {

		if (node) {
			const auto *vert = (node->VERT); size_t next = ((step < N - 1)? (step + 1): 0);
			std::array<unsigned, N> test;
			for (int i = 0; i < N; ++ i) {
				test[i] = ((*vert)[i] >= rect.min[i]) | (((*vert)[i] <= rect.max[i]) << 1);
			}
			if (std::count(test.begin(), test.end(), 3) == N) { list.push_back(vert); }
			if (test[step] & 2) {
				find(list, node->NODE[1], rect, next);
			}
			if (test[step] & 1) {
				find(list, node->NODE[0], rect, next);
			}
		}
	}

	t_node *build(t_vert **start, t_vert **end, int step) {

		if (start >= end) { return nullptr; }

		t_vert **pivot = start + (end - start) / 2;
		std::nth_element(
		start, pivot, end, [step](auto *a, auto *b) { return (*a)[step] < (*b)[step]; });

		int next = (step < N - 1)? (step + 1): 0;

		return new t_node(
		build(start, pivot, next), build(pivot + 1, end, next), *pivot
		);
	}

	t_node *build(t_vert *start, size_t num) {
		std::vector<t_vert *> TEMP(num);
		for (int i = 0; i < num; ++ i) {
			TEMP[i] = start + i;
		}
		return build(
		TEMP.data(), TEMP.data() + num, 0
		);
	}

	t_tree(const t_tree &) = delete;

	std::unique_ptr<t_node> ROOT;
	const t_vert *VERT;
};

//...

}//TREE

}//GEOM
