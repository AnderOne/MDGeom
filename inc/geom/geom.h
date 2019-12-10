#ifndef __INCLUDE_GEOM_H
#define __INCLUDE_GEOM_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct { unsigned size; const void *hand; } t_vert_4d_list;
typedef struct { unsigned size; const void *hand; } t_vert_3d_list;

typedef struct { unsigned size; const void *hand; } t_edge_list;
typedef struct { unsigned size; const void *hand; } t_face_list;
typedef struct { unsigned size; const void *hand; } t_cell_list;

typedef struct { const double *val; } t_vert_4d_data;
typedef struct { const double *val; } t_vert_3d_data;
typedef struct { const int *ind; } t_edge_data;

typedef struct { unsigned num; const int *ind; } t_face_data;
typedef struct { unsigned num; const int *ind; } t_cell_data;

typedef struct { const void *hand; }
t_mesh_4d_hand;
typedef struct { const void *hand; }
t_mesh_3d_hand;

//...

int set_vert_4d(t_vert_4d_list *list, int i, const double val[4]);
int set_vert_3d(t_vert_3d_list *list, int i, const double val[3]);
int set_edge(t_edge_list *list, int i, const int ind[2]);
int set_face(t_face_list *list, int i, int num, const int ind[]);
int set_cell(t_cell_list *list, int i, int num, const int ind[]);

int get_vert_4d(t_vert_4d_data *vert, const t_vert_4d_list *list, int i);
int get_vert_3d(t_vert_3d_data *vert, const t_vert_3d_list *list, int i);
int get_edge(t_edge_data *edge, const t_edge_list *list, int i);
int get_face(t_face_data *face, const t_face_list *list, int i);
int get_cell(t_cell_data *cell, const t_cell_list *list, int i);

int new_vert_4d_list(t_vert_4d_list *list, int size);
int new_vert_3d_list(t_vert_3d_list *list, int size);
int new_edge_list(t_edge_list *list, int size);
int new_face_list(t_face_list *list, int size);
int new_cell_list(t_cell_list *list, int size);

int del_vert_4d_list(t_vert_4d_list *list);
int del_vert_3d_list(t_vert_3d_list *list);
int del_edge_list(t_edge_list *list);
int del_face_list(t_face_list *list);
int del_cell_list(t_cell_list *list);

//...

int get_vert_4d_list(t_vert_4d_list *list, const t_mesh_4d_hand *hand);
int get_vert_3d_list(t_vert_3d_list *list, const t_mesh_3d_hand *hand);

int get_edge_4d_list(t_edge_list *list, const t_mesh_4d_hand *hand);
int get_edge_3d_list(t_edge_list *list, const t_mesh_3d_hand *hand);

int get_face_4d_list(t_face_list *list, const t_mesh_4d_hand *hand);
int get_face_3d_list(t_face_list *list, const t_mesh_3d_hand *hand);

int get_cell_4d_list(t_cell_list *list, const t_mesh_4d_hand *hand);

int new_mesh_4d(t_mesh_4d_hand *hand, t_vert_4d_list *vert, t_edge_list *edge, t_face_list *face, t_cell_list *cell);
int new_mesh_3d(t_mesh_3d_hand *hand, t_vert_3d_list *vert, t_edge_list *edge, t_face_list *face);

int new_rect_4d(t_mesh_4d_hand *hand, double min[4], double max[4]);
int new_rect_3d(t_mesh_3d_hand *hand, double min[3], double max[3]);

int del_mesh_4d(t_mesh_4d_hand *hand);
int del_mesh_3d(t_mesh_3d_hand *hand);

#ifdef __cplusplus
}
#endif

#endif //__INCLUDE_GEOM_H
