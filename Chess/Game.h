#ifndef GAME_H
#define GAME_H

#include "Main.h"
#include "ChessPieces.h"
#include <string>
#include <vector>

enum State{INIT, PLAYING, CHOOSING, MOVING, END};
enum CastlingType{NONE, SHORT, LONG};

class Game{
public:
	Game(){};
	~Game(){};
	void init();
	void click_handler(int, int, int, int);
	void end_move();

	State current_state;
	Color current_player;
	ChessPiece selected_piece;
	vector<int> destination;
	int max_counter;
	CastlingType castling;
};

void update_moving_title(int i, int j, ChessPiece selected_piece);
void convert(int i, int j, char * tmp);

#endif