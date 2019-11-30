#pragma once

#include "mesh.hpp"

namespace GEOM {

using namespace MESH;

namespace RECT {

template <typename T, unsigned N> struct t_rect: public t_mesh<T, N> {

	typedef typename t_mesh<T, N>::t_vert t_vert;

	template <typename ... F, typename = typename std::enable_if<
	         ((sizeof ... (F) == 0) && (N == 4))
	         >::type>
	t_rect(const t_vector<T, 4> &min, const t_vector<T, 4> &max) {

		const std::array<int, 4> zero{0, 0, 0, 0};

		const t_vector_4d lim[2] = {min, max};

		std::vector<t_vert> VERT;
		std::vector<t_edge> EDGE;
		std::vector<t_face> FACE;
		std::vector<t_cell> CELL;

		//Формируем массив вершин:
		for (int i = 0; i < 16; ++ i) {
			int i4 = (i >> 3) & 1;
			int i3 = (i >> 2) & 1;
			int i2 = (i >> 1) & 1;
			int i1 = (i) & 1;
			VERT.push_back(
			{lim[i1][0], lim[i2][1], lim[i3][2], lim[i4][3]}
			);
		}

		//Формируем массив ребер:
		std::vector<std::array<int, 4>> emap(16, zero);
		for (int l = 0, i = 0; i < 16; ++ i)
		for (int k = 0; k < 4; ++ k) {
			if ((i >> k) & 1 != 0) continue;
			int b = i | (1 << k);
			int a = i;
			emap[a][k] = l;
			EDGE.push_back(
			{a, b}
			);
			++ l;
		}

		//Формируем массив граней:
		std::vector<std::array<std::array<int, 4>, 4>>
		fmap(16, {zero, zero, zero, zero});
		for (int l = 0, i = 0; i < 16; ++ i)
		for (int k1 = 0; k1 <  4; ++ k1)
		for (int k2 = 0; k2 < k1; ++ k2) {
			if (((i >> k1) & 1) ||
				((i >> k2) & 1)) {
				continue;
			}
			int e1, e2, e3, e4;
			e1 = emap[i][k1];
			e2 = emap[i][k2];
			int j = i | (1 << k1);
			e3 = emap[j][k2];
			j = i | (1 << k2);
			e4 = emap[j][k1];
			fmap[i][k1][k2] = l;
			fmap[i][k2][k1] = l;
			FACE.push_back(
			{e1, e2, e3, e4}
			);
			++ l;
		}

		//Формируем массив ячеек:
		for (int l = 0, i = 0; i < 16; ++ i)
		for (int k1 = 0; k1 <  4; ++ k1)
		for (int k2 = 0; k2 < k1; ++ k2)
		for (int k3 = 0; k3 < k2; ++ k3) {
			if (((i >> k1) & 1) ||
				((i >> k2) & 1) ||
				((i >> k3) & 1)) {
				continue;
			}
			int f1, f2, f3, f4, f5, f6;
			f1 = fmap[i][k1][k2];
			f2 = fmap[i][k2][k3];
			f3 = fmap[i][k3][k1];
			int j = i | (1 << k1);
			f4 = fmap[j][k2][k3];
			j = i | (1 << k2);
			f5 = fmap[j][k3][k1];
			j = i | (1 << k3);
			f6 = fmap[j][k1][k2];
			CELL.push_back(
			{f1, f2, f3, f4, f5, f6}
			);
		}

		this->set(std::move(VERT));
		this->set(std::move(EDGE));
		this->set(std::move(FACE));
		this->set(std::move(CELL));
	}

	template <typename ... F, typename = typename std::enable_if<
	         ((sizeof ... (F) == 0) && (N == 3))
	         >::type>
	t_rect(const t_vector<T, 3> &min, const t_vector<T, 3> &max) {

		const std::array<int, 3> zero{0, 0, 0};

		const t_vector_3d lim[2] = {min, max};

		std::vector<t_vert> VERT;
		std::vector<t_edge> EDGE;
		std::vector<t_face> FACE;

		//Формируем массив вершин:
		for (int i = 0; i < 8; ++ i) {
			int i3 = (i >> 2) & 1;
			int i2 = (i >> 1) & 1;
			int i1 = (i) & 1;
			VERT.push_back(
			{lim[i1][0], lim[i2][1], lim[i3][2]}
			);
		}

		//Формируем массив ребер:
		std::vector<std::array<int, 3>> emap(8, zero);
		for (int l = 0, i = 0; i < 8; ++ i)
		for (int k = 0; k < 3; ++ k) {
			if ((i >> k) & 1 != 0) continue;
			int b = i | (1 << k);
			int a = i;
			emap[a][k] = l;
			EDGE.push_back(
			{a, b}
			);
			++ l;
		}

		//Формируем массив граней:
		for (int l = 0, i = 0; i < 8; ++ i)
		for (int k1 = 0; k1 <  3; ++ k1)
		for (int k2 = 0; k2 < k1; ++ k2) {
			if (((i >> k1) & 1) ||
				((i >> k2) & 1)) {
				continue;
			}
			int e1, e2, e3, e4;
			e1 = emap[i][k1];
			e2 = emap[i][k2];
			int j = i | (1 << k1);
			e3 = emap[j][k2];
			j = i | (1 << k2);
			e4 = emap[j][k1];
			FACE.push_back(
			{e1, e2, e3, e4}
			);
			++ l;
		}

		this->set(std::move(VERT));
		this->set(std::move(EDGE));
		this->set(std::move(FACE));
	}

	//...
};

}//RECT

//...

typedef RECT::t_rect<double, 4>
t_rect_4d;
typedef RECT::t_rect<double, 3>
t_rect_3d;

//...

}//GEOM
