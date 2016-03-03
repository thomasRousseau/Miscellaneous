#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GL/glut.h>
#include <algorithm>
#include "Mesh.h"
#include "Vec3.h"
#include "Camera.h"
#include "ChessPieces.h"
#include "Game.h"
#include "Texture.h"

// resize factors because the mesh are not the same size
#define FACTOR_ROOK   0.241779
#define FACTOR_PAWN   0.247309
#define FACTOR_BISHOP 0.319832
#define FACTOR_KNIGHT 0.235909
#define FACTOR_KING   0.398655
#define FACTOR_QUEEN  0.362103
// square to mesh conversion factor
#define FACTOR_SQUARE 0.6

using namespace std;

extern Mesh rook_mesh;
extern Mesh pawn_mesh;
extern Mesh bishop_mesh;
extern Mesh white_knight_mesh;
extern Mesh black_knight_mesh;
extern Mesh king_mesh;
extern Mesh queen_mesh;
extern Mesh table_mesh;
extern Camera camera;
extern vector<int> mouse_hover_mesh_index;
extern vector<int> mouse_click_mesh_index;
extern vector<int> possible_moves;

static vector<int> black_squares = {788, 786, 791, 792, 686, 685, 670, 671, 789, 780, 782, 783, 688, 689, 675, 676, 775, 776, 699, 697, 682, 683, 680, 764, 771, 772, 701, 702, 693, 694, 765, 766, 715, 713, 705, 707, 768, 750, 752, 753, 717, 718, 709, 710, 737, 738, 746, 747, 721, 723, 729, 730, 741, 743, 756, 757, 664, 665, 727, 731, 735, 744, 759, 760};
static vector<int> white_squares = {661, 787, 784, 785, 793, 687, 672, 673, 778, 779, 790, 698, 684, 681, 674, 677, 777, 774, 781, 700, 690, 691, 678, 679, 773, 714, 695, 696, 692, 767, 763, 762, 770, 716, 703, 704, 706, 769, 748, 749, 711, 712, 708, 736, 739, 740, 751, 754, 719, 720, 722, 725, 733, 734, 745, 755, 724, 726, 728, 732, 742, 761, 758, 669};
static vector<vector<int>> squares_vertex_index = {{325, 330, 335, 385, 381, 380, 382, 326, 319}, {328, 329, 333, 337, 379, 377, 376, 383, 327}, {331, 332, 334, 336, 378, 372, 373, 375, 384}, {341, 340, 338, 339, 345, 371, 369, 370, 374}, {397, 342, 343, 344, 346, 354, 367, 366, 368}, {396, 394, 349, 347, 348, 353, 355, 363, 365}, {395, 393, 390, 350, 351, 352, 356, 362, 364}, {318, 392, 389, 388, 359, 357, 358, 321, 322}, {315, 317, 391, 387, 386, 360, 361, 320, 316}};
static vector<int> squares = {670, 671, 673, 672, 686, 685, 793, 687, 791, 792, 784, 785, 788, 786, 661, 787, 677, 674, 675, 676, 681, 684, 688, 689, 790, 698, 782, 783, 779, 778, 789, 780, 680, 764, 678, 679, 682, 683, 690, 691, 699, 697, 781, 700, 776, 775, 777, 774, 763, 762, 765, 766, 692, 767, 693, 694, 695, 696, 701, 702, 773, 714, 771, 772, 752, 753, 748, 749, 768, 750, 769, 706, 705, 707, 703, 704, 715, 713, 716, 770, 751, 754, 746, 747, 739, 740, 737, 738, 708, 736, 709, 710, 712, 711, 718, 717, 756, 757, 745, 755, 741, 743, 734, 733, 729, 730, 722, 725, 721, 723, 719, 720, 758, 669, 759, 760, 761, 742, 735, 744, 728, 732, 727, 731, 724, 726, 664, 665};

void init_graphics();
void init_meshes();
void graphics_resize(int, int);
void draw_piece(Mesh, float, float, float, bool);
void draw_piece_to_square(Mesh, int, int, Color);
void draw_moving_pieces(Game &g, int counter);
void draw_moving_pieces_short_castling(Game game, int counter);
void draw_moving_pieces_long_castling(Game game, int counter);
void draw_board();

void draw_textured_black_triangle(unsigned int i);

void handle_mouse_hover(int, int);
void handle_mouse_click(int, int);
void draw_table();

void convert_mesh_to_board(int, int & i, int & j);
bool mesh_is_black(int);
bool mesh_is_white(int);
bool mesh_is_possible_move(int);
bool mesh_is_hover(int m);
bool mesh_is_click(int m);

void add_to_possible_moves(vector<vector<int> > list);
void clear_possible_moves();
bool is_possible_move(int, int);

#endif