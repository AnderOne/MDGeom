#pragma once

#include "mesh.hpp"

namespace GEOM {

using namespace MESH;

namespace RECT {

template <typename T, unsigned N> struct t_rect: public t_mesh<T, N> {

	t_rect(const t_vector<T, N> &min, const t_vector<T, N> &max);

	//...
};

}//RECT

//...

struct t_rect_4d:
public t_mesh_4d {

	t_rect_4d(const t_vector_4d &min, const t_vector_4d &max);

	t_mesh_3d cross(const t_slice_3d &slice) const;

	//...
};

struct t_rect_3d:
public t_mesh_3d {

	t_rect_3d(const t_vector_3d &min, const t_vector_3d &max);

	//...
};

//...

}//GEOM
