#pragma once

#include "mesh.hpp"

namespace GEOM {

namespace RECT {

using namespace BASE;
using namespace MESH;

template <unsigned N>
struct t_rect:
public t_mesh<N> {

	t_rect(const t_vector<double, N> &min, const t_vector<double, N> &max);

	t_mesh<N - 1> cross(const t_basis<double, N, N - 1> &slice) const;

	//...
};

struct t_rect_4d:
public t_mesh_4d {

	t_rect_4d(const t_vector_4d &min, const t_vector_4d &max);

	t_mesh_3d cross(const t_slice_3d &slice) const;

	typedef MESH::t_vert<4> t_vert;

	//...
};

struct t_rect_3d:
public t_mesh_3d {

	t_rect_3d(const t_vector_3d &min, const t_vector_3d &max);

	typedef MESH::t_vert<3> t_vert;

	//...
};

//...

}//RECT
	
}//GEOM
