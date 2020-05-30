/**
 * Copyright (c) 2019-2020 Andrey Baranov <armath123@gmail.com>
 *
 * This file is part of MDGeom (Multi-Dimensional Geometry).
 *
 * MDGeom is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * MDGeom is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with MDGeom;
 * if not, see <http://www.gnu.org/licenses/>
**/

#pragma once

#include "base.hpp"
#include "mesh.hpp"

namespace GEOM {

using namespace BASE;
using namespace MESH;

namespace METH {

//Метод проецирования сетки на подпространство:
template <typename T, unsigned N, unsigned M>
auto project(const t_mesh<T, N, M> &_mesh, const t_basis<T, N, N - 1> &_basis) {

	typedef t_mesh<T, N - 1, (M == N)? (N - 1): (M)> t_projected_mesh;
	typedef typename t_projected_mesh::t_vert t_projected_vert;

	const std::vector<t_vector<T, N>> &old_vert = _mesh.vert();
	std::vector<t_projected_vert> new_vert(old_vert.size());

	for (int i = 0; i < old_vert.size(); ++ i) {
		new_vert[i] = _basis.put(old_vert[i]);
	}

	return t_projected_mesh{
	std::move(new_vert),
	_mesh.grid()
	};
}

//...

}//METH

}//GEOM
