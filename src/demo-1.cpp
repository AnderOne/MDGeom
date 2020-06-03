#include <geom/geom.hpp>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <map>

using namespace GEOM::MESH;
using namespace GEOM::FILE;
using namespace GEOM;

int main() {

	//Generate 3d-mesh by its elements:
	std::vector<t_surf_3d::t_vert> VERT{
		{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
		{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
	};
	std::vector<t_edge> EDGE{
		{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
		{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
		{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7}
	};
	std::vector<t_face> FACE{
		{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
		{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
		{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
		{0, 3, 12}, {0, 4, 13}, {1, 5, 12},
	};
	t_surf_3d mesh(
		std::move(VERT),
		std::move(EDGE),
		std::move(FACE)
	);

	//Iteration over mesh elements:
	std::cout << std::scientific << std::setprecision(5);
	for (const auto &face: mesh) {
		std::cout << "START FACE [" << face.id() << "]:\n";
		for (const auto &edge: face) {
			std::cout << "START EDGE [" << edge.id() << "] lying on faces: {";
			for (const auto &link: edge.link()) {
				std::cout << link.id();
				std::cout << ", ";
			}
			std::cout << "}\n";
			for (const auto &vert: edge) {
				std::cout << "START VERT [" << vert.id() << "] lying on edges: {";
				for (const auto &link: vert.link()) {
					std::cout << link.id();
					std::cout << ", ";
				}
				std::cout << "}\n";
				std::cout << vert.data();
				std::cout << "\n";
				std::cout << "END VERT\n";
			}
			std::cout << "END EDGE\n";
		}
		std::cout << "END FACE\n";
	}

	//Output mesh into file:
	std::ofstream fout("test.txt");
	fout << "1\n";
	fout << mesh;

	return 0;
}
