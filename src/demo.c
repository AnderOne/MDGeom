#include <geom/geom.h>
#include <stdio.h>
#include <math.h>

int main() {

	const double min[] = {-1, -1, -1, -1}, max[] = {+1, +1, +1, +1};

	t_mesh_4d_hand *rect = NULL; new_rect_4d(&rect, min, max);

	FILE *fout = fopen("test.txt", "w");
	const int num_step = 100; fprintf(fout, "%i\n", num_step);

	for (int t = 0; t < num_step; ++ t) {

		printf("time: %i\n", t);

		t_mesh_3d_hand *mesh = NULL;
		get_mesh_4d_sect_3d(&mesh, rect);
		out_mesh_3d(fout, mesh);
		fprintf(fout, "\n");
		off_mesh_3d(&mesh);

		rot_mesh_4d(rect, 0, 3, M_PI / 200);
		rot_mesh_4d(rect, 1, 3, M_PI / 200);
		rot_mesh_4d(rect, 2, 3, M_PI / 200);
		rot_mesh_4d(rect, 0, 1, M_PI / 200);
		rot_mesh_4d(rect, 0, 2, M_PI / 200);
		rot_mesh_4d(rect, 1, 2, M_PI / 200);

		double s = 0.1 * sin(t * M_PI / 10);
		double dir[] = {0, s, -s, s};
		mov_mesh_4d(rect, dir);

	}

	off_mesh_4d(&rect);

	fclose(fout);

	return 0;

}
