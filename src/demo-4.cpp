#include <geom/geom.hpp>

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <map>

using namespace GEOM::BASE;
using namespace GEOM::MESH;
using namespace GEOM::METH;
using namespace GEOM::FILE;
using namespace GEOM;

int main() {

	//Generate 3d-mesh by its elements:
	std::vector<t_mesh_3d::t_vert> VERT{
		{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1},
		{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}
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
	t_mesh_3d mesh(
		std::move(VERT),
		std::move(EDGE),
		std::move(FACE),
		std::move(BODY)
	);

	t_plane_3d plane;
	t_mesh_2d slice = getSlice(mesh, plane);

	//Output slice into file:
	std::ofstream fout("test.txt");
	fout << "1\n\n";
	fout << slice;

	return 0;
}
