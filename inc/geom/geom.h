#ifndef __INCLUDE_GEOM_H
#define __INCLUDE_GEOM_H

#ifndef __cplusplus
#include <stdio.h>
#else
#include <cstdio>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { const double *val; } t_vert_4d_data;
typedef struct { const double *val; } t_vert_3d_data;
typedef struct { const int *ind; } t_edge_data;

typedef struct {
	unsigned num; const int *ind;
} t_face_data;

typedef struct {
	unsigned num; const int *ind;
} t_cell_data;

typedef struct t_mesh_4d_hand
t_mesh_4d_hand;
typedef struct t_mesh_3d_hand
t_mesh_3d_hand;

typedef struct t_vert_4d_list
t_vert_4d_list;
typedef struct t_vert_3d_list
t_vert_3d_list;

typedef struct t_edge_list
t_edge_list;
typedef struct t_face_list
t_face_list;
typedef struct t_cell_list
t_cell_list;

//...

int set_vert_4d_list_data(t_vert_4d_list *list, int i, const double val[4]);
int set_vert_3d_list_data(t_vert_3d_list *list, int i, const double val[3]);
int set_edge_list_data(t_edge_list *list, int i, const int ind[2]);
int set_face_list_data(t_face_list *list, int i, int num, const int ind[]);
int set_cell_list_data(t_cell_list *list, int i, int num, const int ind[]);

int get_vert_4d_list_data(t_vert_4d_data *vert, const t_vert_4d_list *list, int i);
int get_vert_3d_list_data(t_vert_3d_data *vert, const t_vert_3d_list *list, int i);
int get_edge_list_data(t_edge_data *edge, const t_edge_list *list, int i);
int get_face_list_data(t_face_data *face, const t_face_list *list, int i);
int get_cell_list_data(t_cell_data *cell, const t_cell_list *list, int i);

int get_vert_4d_list_size(const t_vert_4d_list *list);
int get_vert_3d_list_size(const t_vert_3d_list *list);
int get_edge_list_size(const t_edge_list *list);
int get_face_list_size(const t_face_list *list);
int get_cell_list_size(const t_cell_list *list);

int new_vert_4d_list(t_vert_4d_list **list, int size);
int new_vert_3d_list(t_vert_3d_list **list, int size);
int new_edge_list(t_edge_list **list, int size);
int new_face_list(t_face_list **list, int size);
int new_cell_list(t_cell_list **list, int size);

int off_vert_4d_list(t_vert_4d_list **list);
int off_vert_3d_list(t_vert_3d_list **list);
int off_edge_list(t_edge_list **list);
int off_face_list(t_face_list **list);
int off_cell_list(t_cell_list **list);

//...

int get_mesh_4d_vert_list(t_vert_4d_list **list, const t_mesh_4d_hand *hand);
int get_mesh_3d_vert_list(t_vert_3d_list **list, const t_mesh_3d_hand *hand);

int get_mesh_4d_edge_list(t_edge_list **list, const t_mesh_4d_hand *hand);
int get_mesh_3d_edge_list(t_edge_list **list, const t_mesh_3d_hand *hand);

int get_mesh_4d_face_list(t_face_list **list, const t_mesh_4d_hand *hand);
int get_mesh_3d_face_list(t_face_list **list, const t_mesh_3d_hand *hand);

int get_mesh_4d_cell_list(t_cell_list **list, const t_mesh_4d_hand *hand);

int new_mesh_4d(t_mesh_4d_hand **hand, t_vert_4d_list **vert, t_edge_list **edge, t_face_list **face, t_cell_list **cell);
int new_mesh_3d(t_mesh_3d_hand **hand, t_vert_3d_list **vert, t_edge_list **edge, t_face_list **face);

int new_rect_4d(t_mesh_4d_hand **hand, const double min[4], const double max[4]);
int new_rect_3d(t_mesh_3d_hand **hand, const double min[3], const double max[3]);

int off_mesh_4d(t_mesh_4d_hand **hand);
int off_mesh_3d(t_mesh_3d_hand **hand);

//...

int rot_mesh_4d(t_mesh_4d_hand *hand, unsigned i, unsigned j, double phi);
int rot_mesh_3d(t_mesh_3d_hand *hand, unsigned i, unsigned j, double phi);

int mov_mesh_4d(t_mesh_4d_hand *hand, const double dir[4]);
int mov_mesh_3d(t_mesh_3d_hand *hand, const double dir[3]);

//...

int out_mesh_4d(FILE *out, const t_mesh_4d_hand *hand);
int out_mesh_3d(FILE *out, const t_mesh_3d_hand *hand);

#ifdef __cplusplus
}
#endif

#endif //__INCLUDE_GEOM_H
