#include <geom/geom.hpp>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <map>

using namespace GEOM::MESH;
using namespace GEOM;

int main() {

	std::vector<t_mesh_3d::t_vert> VERT{
		{0., 0., 0.}, {0., 0., 1.}, {0., 1., 0.}, {0., 1., 1.},
		{1., 0., 0.}, {1., 0., 1.}, {1., 1., 0.}, {1., 1., 1.}
	};
	std::vector<t_edge> EDGE{
		{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3},
		{2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7},
		//...
		{0, 3}, {0, 5}, {0, 6},
		{1, 7}, {2, 7}, {4, 7},
		//...
		{0, 7}
	};
	std::vector<t_face> FACE{
		{6, 11, 16}, {8, 10, 17}, {9, 11, 17},
		{3, 7, 15}, {4, 10, 15}, {5, 7, 16},
		{1, 6, 14}, {2, 8, 13}, {2, 9, 14},
		{0, 3, 12}, {0, 4, 13}, {1, 5, 12},
	};

	t_mesh_3d mesh(
	std::move(VERT), std::move(EDGE), std::move(FACE)
	);

	std::ofstream fout("test.txt");
	fout << "1\n" << mesh.vert().size();
	fout << "\n";
	for (auto vert: mesh.vert()) {
		fout << vert;
		fout << "\n";
	}
	fout << "\n" << mesh.edge().size();
	fout << "\n";
	for (auto edge: mesh.edge()) {
		fout << edge[0];
		fout << "\t";
		fout << edge[1];
		fout << "\n";
	}
	fout << "\n" << mesh.face().size();
	fout << "\n";
	for (auto face: mesh.face()) {
		fout << face[0];
		fout << "\t";
		fout << face[1];
		fout << "\t";
		fout << face[2];
		fout << "\n";
	}

	return 0;
}
