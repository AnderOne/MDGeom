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
	std::vector<t_mesh_3d::t_vert> VERT{
		{0., 0., 0.}, {0., 0., 1.}, {0., 1., 0.}, {0., 1., 1.},
		{1., 0., 0.}, {1., 0., 1.}, {1., 1., 0.}, {1., 1., 1.}
	};
	std::vector<t_edge> EDGE{
		{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
		{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
		{0, 3}, {0, 5}, {0, 6}, {1, 7}, {2, 7}, {4, 7},
		//...
		{0, 7}
	};
	std::vector<t_face> FACE{
		{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
		{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
		{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
		{0, 3, 12}, {0, 4, 13}, {1, 5, 12},
		//...
		{0, 15, 18}, {1, 16, 18}, {2, 17, 18},
		{7, 12, 18}, {11, 14, 18}, {10, 13, 18}
	};
	std::vector<t_body> BODY{
		{0, 6, 13, 16}, {2, 8, 14, 16},
		{1, 7, 14, 17}, {4, 10, 12, 17},
		{3, 9, 12, 15}, {5, 11, 13, 15}
	};
	t_mesh_3d mesh(std::move(VERT), std::move(EDGE), std::move(FACE), std::move(BODY));

	//Iteration over mesh elements:
	std::cout << std::scientific << std::setprecision(5);
	for (const auto &body: mesh) {
		std::cout << "START BODY [" << body.id() << "]:\n";
		for (const auto &face: body) {
			std::cout << "START FACE [" << face.id() << "] lying on bodies: {";
			for (const auto &link: face.link()) {
				std::cout << link.id();
				std::cout << ", ";
			}
			std::cout << "}\n";
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
		std::cout << "END BODY\n";
	}

	//Output mesh into file:
	std::ofstream fout("test.txt");
	fout << "1\n";
	fout << mesh;

	return 0;
}
