#include <geom/geom.hpp>

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace RECT {

t_rect_4d::t_rect_4d(const t_vector_4d &min, const t_vector_4d &max) {

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

	set(std::move(VERT));
	set(std::move(EDGE));
	set(std::move(FACE));
	set(std::move(CELL));
}

t_rect_3d::t_rect_3d(const t_vector_3d &min, const t_vector_3d &max) {

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

	set(std::move(VERT));
	set(std::move(EDGE));
	set(std::move(FACE));
}

t_mesh_3d t_rect_4d::cross(const t_slice_3d &slice) const {

	//Check for vertices positions:
	std::vector<double> proj(vert().size());
	t_vector_4d norm = slice.normal();
	t_vector_4d top = slice.center();
	for (int vi = 0; vi < vert().size(); ++ vi) {
		proj[vi] = (vert(vi) - top) * norm;
	}

	std::vector<MESH::t_vert<3>> VERT;
	std::vector<t_edge> EDGE;
	std::vector<t_face> FACE;

	constexpr int null = -1;
	std::vector<int> VMAP(vert().size(), null);
	std::vector<int> EMAP(edge().size(), null);
	std::vector<int> NODE(edge().size(), null);
	std::vector<int> USED(face().size(), 0);

	//Check for cells:
	for (int ci = 0; ci < cell().size(); ++ ci) {

		const auto &_cell = cell(ci);
		std::vector<int> PATH;

		//Check for faces:
		for (int fi : _cell) {

			const auto &_face = face(fi);
			int line[2];
			int fp = 0;
			int fc = 0;

			//Check for edges:
			for (int ei : _face) {

				const auto &_edge = edge(ei);
				int a = _edge[0];
				int b = _edge[1];
				const auto &va = vert(a);
				const auto &vb = vert(b);
				int ec = 0;

				//Check for vertices:
				if (proj[a] * proj[b] < 0) {
					if (NODE[ei] == null) {
						const auto &ab = vb - va;
						const auto &t = - proj[a] / ab.dot(norm);
						const auto &c = slice.put(va + t * ab);
						//Add new point:
						NODE[ei] = VERT.size();
						VERT.push_back(c);
					}
					line[fp ++] = NODE[ei];
				}
				if (proj[a] == 0) {
					if (VMAP[a] == null) {
						VMAP[a] = VERT.size();
						VERT.push_back(slice.put(va));
					}
					++ ec;
				}
				if (proj[b] == 0) {
					if (VMAP[b] == null) {
						VMAP[b] = VERT.size();
						VERT.push_back(slice.put(vb));
					}
					++ ec;
				}

				//Add new edge:
				if (ec == 2) {
					if (EMAP[ei] == null) {
						EMAP[ei] = EDGE.size();
						EDGE.push_back({VMAP[a], VMAP[b]});
					}
					++ fc;
				}
				//...
			}

			if ((fc > 1) && !USED[fi]) {
				std::valarray<int> val(_face.size());
				for (int i = 0; i < _face.size(); ++ i) {
					val[i] = EMAP[_face[i]];
				}
				FACE.push_back(val);
				USED[fi] = 1;
				continue;
			}
			if (fp > 0) {
				assert(fp == 2);
				PATH.push_back(EDGE.size());
				EDGE.push_back({line[0], line[1]});
			}
		}
		//...
		if (PATH.size() != 0) {
			std::valarray<int> val(PATH.size());
			for (int i = 0; i < PATH.size(); ++ i) {
				val[i] = PATH[i];
			}
			FACE.push_back(val);
		}
	}

	return t_mesh_3d(
	std::move(VERT),
	std::move(FACE),
	std::move(EDGE)
	);
}

//...
	
}
	
}

//...
