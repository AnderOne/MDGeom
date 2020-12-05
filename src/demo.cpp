#include <geom/geom.hpp>
#include "mesh.hpp"
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

enum t_test {
	PROJECT,
	SLICING,
	CUTTING
};

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

//Test cases:

namespace {

const std::array<t_vector_3d, 2> center = {t_vector_3d{+0., +0., +0.}, t_vector_3d{+0., +0., +0.}};
const std::array<t_vector_3d, 2> normal = {t_vector_3d{+0., +0., +1.}, t_vector_3d{-1., +1., +0.}};
const std::array<t_plane_3d, 2> plane = {
	t_plane_3d(),
	t_plane_3d(t_vector_3d{0., 0., 0.}, t_vector_3d{0., 0., 1.}, t_vector_3d{-1, -1, 0.})
};

}

template <unsigned M>
void runTest3D(std::ostream &fout, const t_mesh<double, 3, M> &mesh, t_test test) {

	//print(mesh, std::cout);
	switch (test) {
	case CUTTING:
		fout << (1 + center.size()) << "\n\n" << mesh << "\n\n";
		for (int i = 0; i < center.size(); ++ i) {
			fout << getClipped(mesh, center[i], normal[i]);
		}
		break;
	case SLICING:
		fout << (1 + plane.size()) << "\n\n" << mesh << "\n\n";
		for (int i = 0; i < plane.size(); ++ i) {
			fout << getSection(mesh, plane[i]);
		}
		break;
	default:
		assert(false);
	}
}

template <unsigned M>
void runTest4D(std::ostream &fout, const t_mesh<double, 4, M> &mesh, t_test test) {

	auto MESH = mesh;

	const int n = 100; fout << n << "\n\n";
	for (int t = 0; t < n; ++ t) {

		std::cout << ":: " << t;
		std::cout << std::endl;

		switch (test) {
		case PROJECT:
			fout << getProject(MESH, t_space<4, 3>()) << "\n\n";
			break;
		case SLICING:
			fout << getSection(MESH, t_space<4, 3>()) << "\n\n";
			break;
		default:
			assert(false);
		}

		const double h = M_PI / (2 * n);
		const double s = 0.1 * sin(t * M_PI / (n / 10));
		t_vector_4d v = {0, s, -s, s};
		MESH = MESH.rot(0, 3, h).
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
	//fout << std::fixed;

	//Tests for 4D:
	//runTest4D(fout, getRectMesh4D(POLYTOP), PROJECT);
	//runTest4D(fout, getRectSurf4D(POLYTOP), PROJECT);
	//runTest4D(fout, getRectMesh4D(POLYTOP), SLICING);
	//runTest4D(fout, getRectSurf4D(POLYTOP), SLICING);

	//Tests for 3D:
	//runTest3D(fout, getRectMesh3D(SIMPLEX), CUTTING);
	//runTest3D(fout, getRectSurf3D(SIMPLEX), CUTTING);
	//runTest3D(fout, getRectMesh3D(POLYTOP), CUTTING);
	//runTest3D(fout, getRectSurf3D(POLYTOP), CUTTING);
	//runTest3D(fout, getTetrMesh3D(), CUTTING);
	//runTest3D(fout, getTetrSurf3D(), CUTTING);
	//runTest3D(fout, getRectMesh3D(SIMPLEX), SLICING);
	//runTest3D(fout, getRectSurf3D(SIMPLEX), SLICING);
	//runTest3D(fout, getRectMesh3D(POLYTOP), SLICING);
	//runTest3D(fout, getRectSurf3D(POLYTOP), SLICING);
	//runTest3D(fout, getTetrMesh3D(), SLICING);
	//runTest3D(fout, getTetrSurf3D(), SLICING);
	//...

	return 0;

}
