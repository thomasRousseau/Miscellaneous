#include <vector>
#include <cmath>
#include <cfloat>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

extern int wkingx;
extern int wkingy;
extern int bkingx;
extern int bkingy;

#pragma once

enum ChessPieceType {blank, pawn, bishop, king, queen, rook, knight};
enum Color {black, white, none};

class ChessPiece {
public:
	ChessPiece() : x(-1), y(-1), color(none), type(blank){};
	ChessPiece(int pos_x, int pos_y, Color color, ChessPieceType type, string name) : x(pos_x), y(pos_y), color(color), type(type), hasnt_moved(true), name(name){};
	~ChessPiece(){};
	int x;
	int y;
	Color color;
	ChessPieceType type;
	bool hasnt_moved;
	float alt_offset;
	string name;

	vector<vector<int> > knight_moves(vector<vector<ChessPiece> > & board, bool checking);
	vector<vector<int> > pawn_moves(vector<vector<ChessPiece> > & board, bool checking);
	vector<vector<int> > rook_moves(vector<vector<ChessPiece> > & board, bool checking);
	vector<vector<int> > bishop_moves(vector<vector<ChessPiece> > & board, bool checking);
	vector<vector<int> > king_moves(vector<vector<ChessPiece> > & board, bool checking);
	vector<vector<int> > queen_moves(vector<vector<ChessPiece> > & board, bool checking);
	void make_move(vector<int>);

	vector<vector<int> > moves(vector<vector<ChessPiece> > & board, bool checking){
		vector<vector<int> > v;
		switch(type){
			case knight:
				return knight_moves(board, checking);
			case pawn:
				return pawn_moves(board, checking);
			case rook:
				return rook_moves(board, checking);
			case bishop:
				return bishop_moves(board, checking);
			case king:
				return king_moves(board, checking);
			case queen:
				return queen_moves(board, checking);
			default:
				return v;
		}
	};

	bool is_check(vector<vector<ChessPiece> > &);
};

typedef vector<vector<ChessPiece> > Board;
extern Board board;

class None : public ChessPiece{
public:
	None():ChessPiece(-1, -1, none, blank, string("none")){};
	~None(){};
};


class Pawn : public ChessPiece{
public:
	Pawn(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, pawn, string("pawn")){};
	~Pawn(){};
};

class Bishop : public ChessPiece{
public:
	Bishop(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, bishop, string("bishop")){};
	~Bishop(){};
};

class King : public ChessPiece{
public:
	King(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, king, string("king")){};
	~King(){};
};

class Queen : public ChessPiece{
public:
	Queen(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, queen, string("queen")){};
	~Queen(){};
};

class Rook : public ChessPiece{
public:
	Rook(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, rook, string("rook")){};
	~Rook(){};
};

class Knight : public ChessPiece{
public:
	Knight(int pos_x, int pos_y, Color color):ChessPiece(pos_x, pos_y, color, knight, string("knight")){};
	~Knight(){};
};

void new_board();
void print_board(Board &);
Color invert_color(Color);
vector<vector<int> > remove_check(vector<vector<int> > input, int x, int y);
void locate_king(Board b, Color color, int & x, int & y);
bool is_checkmate(Board & b, Color c);
