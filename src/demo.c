#include <geom/geom.h>
#include <stdio.h>

int main() {

	double min[] = {-1, -1, -1, -1};
	double max[] = {+1, +1, +1, +1};

	t_mesh_4d_hand mesh;

	new_rect_4d(&mesh, min, max);
	//...
	del_mesh_4d(&mesh);

	return 0;
}
