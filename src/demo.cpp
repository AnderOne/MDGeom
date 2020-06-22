#include <geom/geom.hpp>
#include "file.hpp"

#include <functional>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>

using namespace GEOM::BASE;
using namespace GEOM::MESH;
using namespace GEOM::METH;
using namespace GEOM::FILE;
using namespace GEOM;

enum t_cell_type {
	SIMPLEX,
	POLYTOP
};
enum t_test {
	PROJECT,
	SLICING,
	CUTTING
};

t_mesh_4d getRectMesh4D(t_cell_type type) {

	std::vector<t_mesh_4d::t_vert> VERT;
	std::vector<t_edge> EDGE;
	std::vector<t_face> FACE;
	std::vector<t_body> BODY;
	typedef MESH::t_cell<4> t_cell;
	std::vector<t_cell> CELL;

	switch (type) {

	case POLYTOP:
		VERT = {
			{-1, -1, -1, -1}, {+1, -1, -1, -1}, {-1, +1, -1, -1}, {+1, +1, -1, -1},
			{-1, -1, +1, -1}, {+1, -1, +1, -1}, {-1, +1, +1, -1}, {+1, +1, +1, -1},
			{-1, -1, -1, +1}, {+1, -1, -1, +1}, {-1, +1, -1, +1}, {+1, +1, -1, +1},
			{-1, -1, +1, +1}, {+1, -1, +1, +1}, {-1, +1, +1, +1}, {+1, +1, +1, +1}
		};
		EDGE = {
			{0, 1}, {0, 2}, {0, 4}, {0, 8}, {1, 3}, {1, 5}, {1, 9}, {2, 3},
			{2, 6}, {2, 10}, {3, 7}, {3, 11}, {4, 5}, {4, 6}, {4, 12}, {5, 7},
			{5, 13}, {6, 7}, {6, 14}, {7, 15}, {8, 9}, {8, 10}, {8, 12}, {9, 11},
			{9, 13}, {10, 11}, {10, 14}, {11, 15}, {12, 13}, {12, 14}, {13, 15}, {14, 15}
		};
		FACE = {
			{1, 0, 7, 4}, {2, 0, 12, 5}, {2, 1, 13, 8}, {3, 0, 20, 6},
			{3, 1, 21, 9}, {3, 2, 22, 14}, {5, 4, 15, 10}, {6, 4, 23, 11},
			{6, 5, 24, 16}, {8, 7, 17, 10}, {9, 7, 25, 11}, {9, 8, 26, 18},
			{11, 10, 27, 19}, {13, 12, 17, 15}, {14, 12, 28, 16}, {14, 13, 29, 18},
			{16, 15, 30, 19}, {18, 17, 31, 19}, {21, 20, 25, 23}, {22, 20, 28, 24},
			{22, 21, 29, 26}, {24, 23, 30, 27}, {26, 25, 31, 27}, {29, 28, 31, 30}
		};
		BODY = {
			{2, 0, 1, 13, 9, 6}, {4, 0, 3, 18, 10, 7},
			{5, 1, 3, 19, 14, 8}, {5, 2, 4, 20, 15, 11},
			{8, 6, 7, 21, 16, 12}, {11, 9, 10, 22, 17, 12},
			{15, 13, 14, 23, 17, 16}, {20, 18, 19, 23, 22, 21}
		};
		CELL = {
			{0, 1, 2, 3, 4, 5, 6, 7}
		};
		break;

	default:
		assert(false);
	}

	return t_mesh_4d(
	std::move(VERT),
	std::move(EDGE),
	std::move(FACE),
	std::move(BODY),
	std::move(CELL)
	);
}

t_mesh_3d getRectMesh3D(t_cell_type type) {

	std::vector<t_mesh_3d::t_vert> VERT;
	std::vector<t_edge> EDGE;
	std::vector<t_face> FACE;
	std::vector<t_body> BODY;

	switch (type) {

	case SIMPLEX:

		VERT = {
			{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
			{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
		};
		EDGE = {
			{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
			{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
			{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7}, {0, 7}
		};
		FACE = {
			{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
			{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
			{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
			{0, 3, 12}, {0, 4, 13}, {1, 5, 12},
			//...
			{0, 15, 18}, {1, 16, 18}, {2, 17, 18},
			{7, 12, 18}, {11, 14, 18}, {10, 13, 18}
		};
		BODY = {
			{0, 6, 13, 16}, {2, 8, 14, 16},
			{1, 7, 14, 17}, {4, 10, 12, 17},
			{3, 9, 12, 15}, {5, 11, 13, 15}
		};
		break;

	case POLYTOP:

		VERT = {
			{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
			{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
		};
		EDGE = {
			{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
			{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}
		};
		FACE = {
			{6, 11, 5, 7}, {8, 10, 9, 11},
			{3, 7, 4, 10}, {1, 6, 2, 9},
			{2, 8, 0, 4}, {0, 3, 1, 5}
		};
		BODY = {
			{0, 1, 2, 3, 4, 5}
		};
		break;

	default:
		assert(false);
	}

	return t_mesh_3d(
	std::move(VERT),
	std::move(EDGE),
	std::move(FACE),
	std::move(BODY)
	);
}

t_surf_3d getRectSurf3D(t_cell_type type) {

	std::vector<t_surf_3d::t_vert> VERT;
	std::vector<t_edge> EDGE;
	std::vector<t_face> FACE;
	std::vector<t_body> BODY;

	switch (type) {

	case SIMPLEX:

		VERT = {
			{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
			{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
		};
		EDGE = {
			{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
			{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
			{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7}
		};
		FACE = {
			{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
			{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
			{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
			{0, 3, 12}, {0, 4, 13}, {1, 5, 12}
		};
		break;

	case POLYTOP:

		VERT = {
			{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
			{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
		};
		EDGE = {
			{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
			{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}
		};
		FACE = {
			{6, 11, 5, 7}, {8, 10, 9, 11},
			{3, 7, 4, 10}, {1, 6, 2, 9},
			{2, 8, 0, 4}, {0, 3, 1, 5}
		};
		break;

	default:
		assert(false);
	}

	return t_surf_3d(
	std::move(VERT),
	std::move(EDGE),
	std::move(FACE)
	);
}


//Iteration over mesh elements:
template <typename T, unsigned N>
void print(const t_mesh<T, N, 3> &mesh, std::ostream &out) {

	for (const auto &body: mesh) {
		out << "START BODY [" << body.id() << "]:\n";
		for (const auto &face: body) {
			out << "START FACE [" << face.id() << "] lying on bodies: {";
			for (const auto &link: face.link()) {
				out << link.id();
				out << ", ";
			}
			out << "}\n";
			for (const auto &edge: face) {
				out << "START EDGE [" << edge.id() << "] lying on faces: {";
				for (const auto &link: edge.link()) {
					out << link.id();
					out << ", ";
				}
				out << "}\n";
				for (const auto &vert: edge) {
					out << "START VERT [" << vert.id() << "] lying on edges: {";
					for (const auto &link: vert.link()) {
						out << link.id();
						out << ", ";
					}
					out << "}\n";
					out << vert.data();
					out << "\n";
					out << "END VERT\n";
				}
				out << "END EDGE\n";
			}
			out << "END FACE\n";
		}
		out << "END BODY\n";
	}
}

//Iteration over mesh elements:
template <typename T, unsigned N>
void print(const t_mesh<T, N, 2> &mesh, std::ostream &out) {

	for (const auto &face: mesh) {
		out << "START FACE [" << face.id() << "]:\n";
		for (const auto &edge: face) {
			out << "START EDGE [" << edge.id() << "] lying on faces: {";
			for (const auto &link: edge.link()) {
				out << link.id();
				out << ", ";
			}
			out << "}\n";
			for (const auto &vert: edge) {
				out << "START VERT [" << vert.id() << "] lying on edges: {";
				for (const auto &link: vert.link()) {
					out << link.id();
					out << ", ";
				}
				out << "}\n";
				out << vert.data();
				out << "\n";
				out << "END VERT\n";
			}
			out << "END EDGE\n";
		}
		out << "END FACE\n";
	}
}


void testForRectSurf3D(std::ostream &fout, t_cell_type type, t_test test) {

	const t_surf_3d mesh = getRectSurf3D(type);
	//print(mesh, std::cout);
	fout << 2 << "\n\n" << mesh << "\n\n";
	switch (test) {
	case CUTTING:
		fout << getClipped(mesh, {0., 0., 0.}, {0., 0., 1.});
		break;
	case SLICING:
		fout << getSection(mesh, t_plane_3d());
		break;
	default:
		assert(false);
	}
}

void testForRectMesh3D(std::ostream &fout, t_cell_type type, t_test test) {

	const t_mesh_3d mesh = getRectMesh3D(type);
	//print(mesh, std::cout);
	fout << 2 << "\n\n" << mesh << "\n\n";
	switch (test) {
	case CUTTING:
		fout << getClipped(mesh, {0., 0., 0.}, {0., 0., 1.});
		break;
	case SLICING:
		fout << getSection(mesh, t_plane_3d());
		break;
	default:
		assert(false);
	}
}

void testForRectMesh4D(std::ostream &fout, t_test test) {

	t_mesh_4d mesh = getRectMesh4D(POLYTOP);

	const int n = 100; fout << n << "\n\n";
	for (int t = 0; t < n; ++ t) {

		std::cout << ":: " << t;
		std::cout << std::endl;

		switch (test) {
		case PROJECT:
			fout << getProject(mesh, t_space<4, 3>()) << "\n\n";
			break;
		case SLICING:
			fout << getSection(mesh, t_space<4, 3>()) << "\n\n";
			break;
		default:
			assert(false);
		}

		const double h = M_PI / (2 * n);
		const double s = 0.1 * sin(t * M_PI / (n / 10));
		t_vector_4d v = {0, s, -s, s};
		mesh = mesh.rot(0, 3, h).
		            rot(1, 3, h).
		            rot(2, 3, h).
		            rot(0, 1, h).
		            rot(0, 2, h).
		            rot(1, 2, h).
		            mov(v);
	}
}

int main() {

	std::ofstream fout("test.txt");
	fout << std::setprecision(5);
	fout << std::scientific;
	//testForRectSurf3D(fout, SIMPLEX, CUTTING);
	//testForRectSurf3D(fout, SIMPLEX, SLICING);
	//testForRectSurf3D(fout, POLYTOP, CUTTING);
	//testForRectSurf3D(fout, POLYTOP, SLICING);
	//testForRectMesh3D(fout, SIMPLEX, CUTTING);
	//testForRectMesh3D(fout, SIMPLEX, SLICING);
	//testForRectMesh3D(fout, POLYTOP, CUTTING);
	//testForRectMesh3D(fout, POLYTOP, SLICING);
	//testForRectMesh4D(fout, PROJECT);
	testForRectMesh4D(fout, SLICING);

	return 0;

}
