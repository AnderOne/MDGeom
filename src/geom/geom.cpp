#include <geom/geom.hpp>
#include <geom/geom.h>

#include <memory>
#include <map>

using namespace GEOM;

static std::map<const void *, std::unique_ptr<std::vector<t_vector_4d>>> VERT_4D_LIST;
static std::map<const void *, std::unique_ptr<std::vector<t_vector_3d>>> VERT_3D_LIST;
static std::map<const void *, std::unique_ptr<std::vector<t_edge>>> EDGE_LIST;
static std::map<const void *, std::unique_ptr<std::vector<t_face>>> FACE_LIST;
static std::map<const void *, std::unique_ptr<std::vector<t_cell>>> CELL_LIST;

#define GET_LIST(TYPE, HAND) \
(const_cast<std::vector<TYPE> &> (*reinterpret_cast<const std::vector<TYPE> *> (HAND)))

#define NEW_LIST(TYPE, SIZE) (std::make_unique<std::vector<TYPE>> (SIZE))

#define PTR(TYPE, HAND) \
(const_cast<TYPE *> (reinterpret_cast<const TYPE *> (HAND)))

extern "C" {

int set_vert_4d(t_vert_4d_list *list, int i, const double val[4]) {

	if ((list == nullptr) || !VERT_4D_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vec = GET_LIST(t_vector_4d, list->hand);
	vec[i] = {val[0], val[1], val[2], val[3]};

	return 0;
}

int set_vert_3d(t_vert_3d_list *list, int i, const double val[3]) {

	if ((list == nullptr) || !VERT_3D_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vec = GET_LIST(t_vector_3d, list->hand);
	vec[i] = {val[0], val[1], val[2]};

	return 0;
}

int set_edge(t_edge_list *list, int i, const int ind[2]) {

	if ((list == nullptr) || !EDGE_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vec = GET_LIST(t_edge, list->hand);
	vec[i] = {ind[0], ind[1]};

	return 0;
}

int set_face(t_face_list *list, int i, int num, const int ind[]) {

	if ((list == nullptr) || !FACE_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vec = GET_LIST(t_face, list->hand);
	vec[i] = t_face(ind, num);

	return 0;
}

int set_cell(t_cell_list *list, int i, int num, const int ind[]) {

	if ((list == nullptr) || !CELL_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vec = GET_LIST(t_cell, list->hand);
	vec[i] = t_cell(ind, num);

	return 0;
}


int get_vert_4d(t_vert_4d_data *vert, const t_vert_4d_list *list, int i) {

	const auto &vec = GET_LIST(t_vector_4d, list->hand);

	vert->val = vec[i].data();

	return 0;
}

int get_vert_3d(t_vert_3d_data *vert, const t_vert_3d_list *list, int i) {

	const auto &vec = GET_LIST(t_vector_3d, list->hand);

	vert->val = vec[i].data();

	return 0;
}

int get_edge(t_edge_data *edge, const t_edge_list *list, int i) {

	const auto &vec = GET_LIST(t_edge, list->hand);

	edge->ind = vec[i].data();

	return 0;
}

int get_face(t_face_data *face, const t_face_list *list, int i) {

	const auto &vec = GET_LIST(t_face, list->hand);

	face->num = vec[i].size();
	face->ind = vec[i].data();

	return 0;
}

int get_cell(t_cell_data *cell, const t_cell_list *list, int i) {

	const auto &vec = GET_LIST(t_cell, list->hand);

	cell->num = vec[i].size();
	cell->ind = vec[i].data();

	return 0;
}


int new_vert_4d_list(t_vert_4d_list *list, int size) {

	auto ptr = NEW_LIST(t_vector_4d, size);
	list->hand = ptr.get();
	list->size = size;
	VERT_4D_LIST[ptr.get()] = std::move(ptr);

	return 0;
}

int new_vert_3d_list(t_vert_3d_list *list, int size) {

	auto ptr = NEW_LIST(t_vector_3d, size);
	list->hand = ptr.get();
	list->size = size;
	VERT_3D_LIST[ptr.get()] = std::move(ptr);

	return 0;
}

int new_edge_list(t_edge_list *list, int size) {

	auto ptr = NEW_LIST(t_edge, size);
	list->hand = ptr.get();
	list->size = size;
	EDGE_LIST[ptr.get()] = std::move(ptr);

	return 0;
}

int new_face_list(t_face_list *list, int size) {

	auto ptr = NEW_LIST(t_face, size);
	list->hand = ptr.get();
	list->size = size;
	FACE_LIST[ptr.get()] = std::move(ptr);

	return 0;
}

int new_cell_list(t_cell_list *list, int size) {

	auto ptr = NEW_LIST(t_cell, size);
	list->hand = ptr.get();
	list->size = size;
	CELL_LIST[ptr.get()] = std::move(ptr);

	return 0;
}


int del_vert_4d_list(t_vert_4d_list *list) {

	if ((list == nullptr) || !VERT_4D_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	VERT_4D_LIST.erase(list->hand);
	list->hand = nullptr;
	list->size = 0;
	return 0;
}

int del_vert_3d_list(t_vert_3d_list *list) {

	if ((list == nullptr) || !VERT_3D_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	VERT_3D_LIST.erase(list->hand);
	list->hand = nullptr;
	list->size = 0;
	return 0;
}

int del_edge_list(t_edge_list *list) {

	if ((list == nullptr) || !EDGE_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	EDGE_LIST.erase(list->hand);
	list->hand = nullptr;
	list->size = 0;
	return 0;
}

int del_face_list(t_face_list *list) {

	if ((list == nullptr) || !FACE_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	FACE_LIST.erase(list->hand);
	list->hand = nullptr;
	list->size = 0;
	return 0;
}

int del_cell_list(t_cell_list *list) {

	if ((list == nullptr) || !CELL_LIST.count(list->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	CELL_LIST.erase(list->hand);
	list->hand = nullptr;
	list->size = 0;
	return 0;
}


//...

int get_vert_4d_list(t_vert_4d_list *list, const t_mesh_4d_hand *hand) {

	t_mesh_4d *mesh = PTR(t_mesh_4d, hand->hand);
	list->size =   mesh->vert().size();
	list->hand = &(mesh->vert());

	return 0;
}

int get_vert_3d_list(t_vert_3d_list *list, const t_mesh_3d_hand *hand) {

	t_mesh_3d *mesh = PTR(t_mesh_3d, hand->hand);
	list->size =   mesh->vert().size();
	list->hand = &(mesh->vert());

	return 0;
}


int get_edge_4d_list(t_edge_list *list, const t_mesh_4d_hand *hand) {

	t_mesh_4d *mesh = PTR(t_mesh_4d, hand->hand);
	list->size =   mesh->edge().size();
	list->hand = &(mesh->edge());

	return 0;
}

int get_edge_3d_list(t_edge_list *list, const t_mesh_3d_hand *hand) {

	t_mesh_3d *mesh = PTR(t_mesh_3d, hand->hand);
	list->size =   mesh->edge().size();
	list->hand = &(mesh->edge());

	return 0;
}


int get_face_4d_list(t_face_list *list, const t_mesh_4d_hand *hand) {

	t_mesh_4d *mesh = PTR(t_mesh_4d, hand->hand);
	list->size =   mesh->face().size();
	list->hand = &(mesh->face());

	return 0;
}

int get_face_3d_list(t_face_list *list, const t_mesh_3d_hand *hand) {

	t_mesh_3d *mesh = PTR(t_mesh_3d, hand->hand);
	list->size =   mesh->face().size();
	list->hand = &(mesh->face());

	return 0;
}


int get_cell_4d_list(t_cell_list *list, const t_mesh_4d_hand *hand) {

	t_mesh_4d *mesh = PTR(t_mesh_4d, hand->hand);
	list->size =   mesh->cell().size();
	list->hand = &(mesh->cell());

	return 0;
}


int new_mesh_4d(t_mesh_4d_hand *hand, t_vert_4d_list *vert, t_edge_list *edge, t_face_list *face, t_cell_list *cell) {

	if (!VERT_4D_LIST.count(vert->hand) || !EDGE_LIST.count(edge->hand) ||
	    !FACE_LIST.count(face->hand) || !CELL_LIST.count(cell->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vert_list = GET_LIST(t_vector_4d, vert->hand);
	auto &edge_list = GET_LIST(t_edge, edge->hand);
	auto &face_list = GET_LIST(t_face, face->hand);
	auto &cell_list = GET_LIST(t_cell, cell->hand);

	hand->hand = new t_mesh_4d(
		std::move(vert_list),
		std::move(edge_list),
		std::move(face_list),
		std::move(cell_list)
	);

	VERT_4D_LIST.erase(vert->hand);
	EDGE_LIST.erase(edge->hand);
	FACE_LIST.erase(face->hand);
	CELL_LIST.erase(cell->hand);

	vert->hand = nullptr;
	edge->hand = nullptr;
	face->hand = nullptr;
	cell->hand = nullptr;
	vert->size = 0;
	edge->size = 0;
	face->size = 0;
	cell->size = 0;

	return 0;
}

int new_mesh_3d(t_mesh_3d_hand *hand, t_vert_3d_list *vert, t_edge_list *edge, t_face_list *face) {

	if (!VERT_3D_LIST.count(vert->hand) || !EDGE_LIST.count(edge->hand) ||
	    !FACE_LIST.count(face->hand)) {
		//ERROR: ACCESS VIOLATION!
		return -1;
	}
	auto &vert_list = GET_LIST(t_vector_3d, vert->hand);
	auto &edge_list = GET_LIST(t_edge, edge->hand);
	auto &face_list = GET_LIST(t_face, face->hand);

	hand->hand = new t_mesh_3d(
		std::move(vert_list),
		std::move(edge_list),
		std::move(face_list)
	);

	VERT_3D_LIST.erase(vert->hand);
	EDGE_LIST.erase(edge->hand);
	FACE_LIST.erase(face->hand);

	vert->hand = nullptr;
	edge->hand = nullptr;
	face->hand = nullptr;
	vert->size = 0;
	edge->size = 0;
	face->size = 0;

	return 0;
}


int new_rect_4d(t_mesh_4d_hand *hand, double min[4], double max[4]) {

	hand->hand = new t_rect_4d(
	{min[0], min[1], min[2], min[3]}, {max[0], max[1], max[2], max[3]}
	);

	return 0;
}

int new_rect_3d(t_mesh_3d_hand *hand, double min[3], double max[3]) {

	hand->hand = new t_rect_3d(
	{min[0], min[1], min[2]}, {max[0], max[1], max[2]}
	);

	return 0;
}


int del_mesh_4d(t_mesh_4d_hand *hand) {

	t_mesh_4d *mesh = PTR(t_mesh_4d, hand->hand);
	delete mesh;
	hand->hand = nullptr;
	return 0;
}

int del_mesh_3d(t_mesh_3d_hand *hand) {

	t_mesh_3d *mesh = PTR(t_mesh_3d, hand->hand);
	delete mesh;
	hand->hand = nullptr;
	return 0;
}


}
