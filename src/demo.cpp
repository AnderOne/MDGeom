#include <geom/geom.hpp>
#include <fstream>

using namespace GEOM;

int main() {

	t_rect_4d rect{{-1, -1, -1, -1}, {+1, +1, +1, +1}};

	std::ofstream out("test.txt");
	constexpr int num_step = 100;
	out << num_step << "\n";
	
	for (int t = 0; t < num_step; ++ t) {

		std::cout << "time: " << t << std::endl;
		t_slice_3d slice;
		t_mesh_3d mesh = rect.cross(slice);
		out << mesh << "\n";

		rect.rot(0, 3, M_PI / 200);
		rect.rot(1, 3, M_PI / 200);
		rect.rot(2, 3, M_PI / 200);
		rect.rot(0, 1, M_PI / 200);
		rect.rot(0, 2, M_PI / 200);
		rect.rot(1, 2, M_PI / 200);
		double s = 0.1 * sin(t * M_PI / 10);
		rect.mov({0, s, -s, s});

	}

	out.close();

	return 0;
}
