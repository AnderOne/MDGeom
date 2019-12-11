#include <geom/geom.hpp>
#include <geom/geom.h>

#include <sstream>
#include <cstdio>
#include <memory>
#include <map>

using namespace GEOM;

template <typename T> struct t_temp_list {

	typedef std::vector<T> t_list;
	typedef T t_item;

	const t_item &get(int i) const { return (*const_ptr)[i]; }

	bool set(int i, const T &val) {
		if (!is_const()) { (*ptr)[i] = val; return 1; }
		return 0;
	}

	const t_list &const_data() const { return *const_ptr; }

	t_list &data() { return *ptr; }

	bool is_const() const { return ptr == nullptr; }

	int size() const { return const_ptr->size(); }

	t_temp_list(const t_list *_ptr):
	const_ptr(_ptr), ptr(nullptr) {}

	t_temp_list(size_t _size):
	ptr(new t_list (_size)) { const_ptr = ptr; }

	~t_temp_list() { delete ptr; }

private:
	const t_list *const_ptr;
	t_list *ptr;
};

struct t_vert_4d_list: public t_temp_list<t_vector_4d> {
	template <typename ... TT>
	t_vert_4d_list(const TT &... args):
	t_temp_list<t_vector_4d>
	(args ...) {}
};
struct t_vert_3d_list: public t_temp_list<t_vector_3d> {
	template <typename ... TT>
	t_vert_3d_list(const TT &... args):
	t_temp_list<t_vector_3d>
	(args ...) {}
};

struct t_edge_list: public t_temp_list<t_edge> {
	template <typename ... TT>
	t_edge_list(const TT &... args):
	t_temp_list<t_edge>
	(args ...) {}
};
struct t_face_list: public t_temp_list<t_face> {
	template <typename ... TT>
	t_face_list(const TT &... args):
	t_temp_list<t_face>
	(args ...) {}
};
struct t_cell_list: public t_temp_list<t_cell> {
	template <typename ... TT>
	t_cell_list(const TT &... args):
	t_temp_list<t_cell>
	(args ...) {}
};

struct t_mesh_4d_hand {
	t_mesh_4d mesh;
};
struct t_mesh_3d_hand {
	t_mesh_3d mesh;
};

//...

namespace {

template <typename T, typename H>
std::vector<T> mov_list(H *src) {
	std::vector<T> res;
	if (src->is_const()) res = std::move(src->data()); else res = src->const_data();
	return res;
}

}

//...

extern "C" {

int set_vert_4d_list_data(t_vert_4d_list *list, int i, const double val[4]) {

	return !list->set(i, {val[0], val[1], val[2], val[3]});
}

int set_vert_3d_list_data(t_vert_3d_list *list, int i, const double val[3]) {

	return !list->set(i, {val[0], val[1], val[2]});
}

int set_edge_list_data(t_edge_list *list, int i, const int ind[2]) {

	return !list->set(i, {ind[0], ind[1]});
}

int set_face_list_data(t_face_list *list, int i, int num, const int ind[]) {

	return !list->set(i, t_face(ind, num));
}

int set_cell_list_data(t_cell_list *list, int i, int num, const int ind[]) {

	return !list->set(i, t_cell(ind, num));
}


int get_vert_4d_list_data(t_vert_4d_data *vert, const t_vert_4d_list *list, int i) {

	const auto &val = list->get(i);
	vert->val = val.data();

	return 0;
}

int get_vert_3d_list_data(t_vert_3d_data *vert, const t_vert_3d_list *list, int i) {

	const auto &val = list->get(i);
	vert->val = val.data();

	return 0;
}

int get_edge_list_data(t_edge_data *edge, const t_edge_list *list, int i) {

	const auto &ind = list->get(i);
	edge->ind = ind.data();

	return 0;
}

int get_face_list_data(t_face_data *face, const t_face_list *list, int i) {

	const auto &ind = list->get(i);
	face->ind = ind.data();
	face->num = ind.size();

	return 0;
}

int get_cell_list_data(t_cell_data *cell, const t_cell_list *list, int i) {

	const auto &ind = list->get(i);
	cell->ind = ind.data();
	cell->num = ind.size();

	return 0;
}

int get_vert_4d_list_size(const t_vert_4d_list *list) {
	return list->size();
}

int get_vert_3d_list_size(const t_vert_3d_list *list) {
	return list->size();
}

int get_edge_list_size(const t_edge_list *list) {
	return list->size();
}

int get_face_list_size(const t_face_list *list) {
	return list->size();
}

int get_cell_list_size(const t_cell_list *list) {
	return list->size();
}


int new_vert_4d_list(t_vert_4d_list **list, int size) {
	off_vert_4d_list(list);
	*list = new t_vert_4d_list(size);
	return 0;
}

int new_vert_3d_list(t_vert_3d_list **list, int size) {
	off_vert_3d_list(list);
	*list = new t_vert_3d_list(size);
	return 0;
}

int new_edge_list(t_edge_list **list, int size) {
	off_edge_list(list);
	*list = new t_edge_list(size);
	return 0;
}

int new_face_list(t_face_list **list, int size) {
	off_face_list(list);
	*list = new t_face_list(size);
	return 0;
}

int new_cell_list(t_cell_list **list, int size) {
	off_cell_list(list);
	*list = new t_cell_list(size);
	return 0;
}


int off_vert_4d_list(t_vert_4d_list **list) {
	delete *list; *list = nullptr;
	return 0;
}

int off_vert_3d_list(t_vert_3d_list **list) {
	delete *list; *list = nullptr;
	return 0;
}

int off_edge_list(t_edge_list **list) {
	delete *list; *list = nullptr;
	return 0;
}

int off_face_list(t_face_list **list) {
	delete *list; *list = nullptr;
	return 0;
}

int off_cell_list(t_cell_list **list) {
	delete *list; *list = nullptr;
	return 0;
}


//...

int get_mesh_4d_vert_list(t_vert_4d_list **list, const t_mesh_4d_hand *hand) {
	off_vert_4d_list(list);
	*list = new t_vert_4d_list(&(hand->mesh.vert()));
	return 0;
}

int get_mesh_3d_vert_list(t_vert_3d_list **list, const t_mesh_3d_hand *hand) {
	off_vert_3d_list(list);
	*list = new t_vert_3d_list(&(hand->mesh.vert()));
	return 0;
}


int get_mesh_4d_edge_list(t_edge_list **list, const t_mesh_4d_hand *hand) {
	off_edge_list(list);
	*list = new t_edge_list(&(hand->mesh.edge()));
	return 0;
}

int get_mesh_3d_edge_list(t_edge_list **list, const t_mesh_3d_hand *hand) {
	off_edge_list(list);
	*list = new t_edge_list(&(hand->mesh.edge()));
	return 0;
}


int get_mesh_4d_face_list(t_face_list **list, const t_mesh_4d_hand *hand) {
	off_face_list(list);
	*list = new t_face_list(&(hand->mesh.face()));
	return 0;
}

int get_mesh_3d_face_list(t_face_list **list, const t_mesh_3d_hand *hand) {
	off_face_list(list);
	*list = new t_face_list(&(hand->mesh.face()));
	return 0;
}


int get_mesh_4d_cell_list(t_cell_list **list, const t_mesh_4d_hand *hand) {
	off_cell_list(list);
	*list = new t_cell_list(&(hand->mesh.cell()));
	return 0;
}


int new_mesh_4d(t_mesh_4d_hand **hand, t_vert_4d_list **vert, t_edge_list **edge, t_face_list **face, t_cell_list **cell) {

	off_mesh_4d(hand);

	*hand = new t_mesh_4d_hand{
	t_mesh_4d(
		mov_list<t_vector_4d>(*vert),
		mov_list<t_edge>(*edge),
		mov_list<t_face>(*face),
		mov_list<t_cell>(*cell)
	)};

	off_vert_4d_list(vert);
	off_edge_list(edge);
	off_face_list(face);
	off_cell_list(cell);

	return 0;
}

int new_mesh_3d(t_mesh_3d_hand **hand, t_vert_3d_list **vert, t_edge_list **edge, t_face_list **face) {

	off_mesh_3d(hand);

	*hand = new t_mesh_3d_hand{
	t_mesh_3d(
		mov_list<t_vector_3d>(*vert),
		mov_list<t_edge>(*edge),
		mov_list<t_face>(*face)
	)};

	off_vert_3d_list(vert);
	off_edge_list(edge);
	off_face_list(face);

	return 0;
}


int new_rect_4d(t_mesh_4d_hand **hand, const double min[4], const double max[4]) {

	off_mesh_4d(hand);

	*hand = new t_mesh_4d_hand{
	t_rect_4d(
	{min[0], min[1], min[2], min[3]},
	{max[0], max[1], max[2], max[3]}
	)};

	return 0;
}

int new_rect_3d(t_mesh_3d_hand **hand, const double min[4], const double max[4]) {

	off_mesh_3d(hand);

	*hand = new t_mesh_3d_hand{
	t_rect_3d(
	{min[0], min[1], min[2]},
	{max[0], max[1], max[2]}
	)};

	return 0;
}


int off_mesh_4d(t_mesh_4d_hand **hand) {

	if (*hand != nullptr) {
		delete *hand;
		*hand = nullptr;
	}
	return 0;
}

int off_mesh_3d(t_mesh_3d_hand **hand) {

	if (*hand != nullptr) {
		delete *hand;
		*hand = nullptr;
	}
	return 0;
}


int rot_mesh_4d(t_mesh_4d_hand *hand, unsigned i, unsigned j, double phi) {

	hand->mesh.rot(i, j, phi);

	return 0;
}

int rot_mesh_3d(t_mesh_3d_hand *hand, unsigned i, unsigned j, double phi) {

	hand->mesh.rot(i, j, phi);

	return 0;
}

int mov_mesh_4d(t_mesh_4d_hand *hand, const double dir[4]) {

	hand->mesh.mov({dir[0], dir[1], dir[2], dir[3]});
	return 0;
}

int mov_mesh_3d(t_mesh_3d_hand *hand, const double dir[3]) {

	hand->mesh.mov({dir[0], dir[1], dir[2]});
	return 0;
}


int out_mesh_4d(FILE *out, const t_mesh_4d_hand *hand) {

	std::stringstream str;
	str << hand->mesh;
	fprintf(out, "%s", str.str().c_str());

	return 0;
}

int out_mesh_3d(FILE *out, const t_mesh_3d_hand *hand) {

	std::stringstream str;
	str << hand->mesh;
	fprintf(out, "%s", str.str().c_str());

	return 0;
}


}
