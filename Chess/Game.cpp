#include "Graphics.h"
#include "Game.h"
#include <unistd.h>

#define MOVING_SPEED 10

#define WHITE_TURN "White's turn to play"
#define BLACK_TURN "Black's turn to play"
#define MOVING_MESSAGE "%s %s moving from %s to %s"
#define WHITE_CHECK "White king is in check"
#define BLACK_CHECK "Black king is in check"
#define WHITE_CHECKMATE "White king is in checkmate. Black player wins !"
#define BLACK_CHECKMATE "Black king is in checkmate. White player wins !"
#define TIME_TO_WAIT 10
#define PLAY_AGAIN "This game is over and will reset in %d seconds ..."

void Game::init(){
	current_state = PLAYING;
	current_player = white;
	new_board();
	selected_piece = None();
	destination = {-1, -1};
	update_window_title(WHITE_TURN);
	reset_counter();
}

void Game::click_handler(int button, int state, int x, int y){
	if (button == 0 && state == 0){
		switch(current_state){
			// Player have not yet clicked on a valid square
			case PLAYING:
			handle_mouse_click(x, y);
	    	if (mouse_click_mesh_index[0] >= 0){
				int i, j = 0;
				convert_mesh_to_board(mouse_click_mesh_index[0], i, j);
				if (board[i][j].color == current_player){
					current_state = CHOOSING;
					add_to_possible_moves(board[i][j].moves(board, true));
					selected_piece = board[i][j];
				}
			}
			break;
			// Player has clicked on a valid square and is shown
			// his possible moves
			case CHOOSING:
			handle_mouse_click(x, y);
	    	if (mouse_click_mesh_index[0] >= 0){
				int i, j = 0;
				convert_mesh_to_board(mouse_click_mesh_index[0], i, j);
				if ( is_possible_move(i, j) ){
					board[selected_piece.x][selected_piece.y] = None();
					//check for castling
					if (selected_piece.type == king){
						if (abs(selected_piece.y - j)>=2){
							if (selected_piece.y < j){
								this->castling = SHORT;
								board[selected_piece.x][7] = None();
							}
							else if (selected_piece.y > j){
								this->castling = LONG;
								board[selected_piece.x][0] = None();
							}
						}
					}
					destination = {i, j};
					max_counter = MOVING_SPEED*abs(selected_piece.x-i) + abs(selected_piece.y-j);
					current_state = MOVING;
					clear_possible_moves();
					update_moving_title(i, j, selected_piece);
				}
				else{
					clear_possible_moves();
					current_state = PLAYING;
					selected_piece = None();
				}
			}

			default:
			break;
		}
	}
}

void Game::end_move(){
	char title[100];
	if (this->castling == SHORT){
		board[selected_piece.x][5] = Rook(selected_piece.x, 5, selected_piece.color);
	}
	else if (this->castling == LONG){
		board[selected_piece.x][2] = Rook(selected_piece.x, 2, selected_piece.color);
	}
	this->castling = NONE;
	selected_piece.make_move(destination);
	destination = {-1, -1};
	if (is_checkmate(board, current_player)){
		if (current_player == white){
			sprintf(title, BLACK_CHECKMATE);
			update_window_title(title);
		}
		else if (current_player == black){
			sprintf(title, WHITE_CHECKMATE);
			update_window_title(title);
		}
		usleep(5000000);
		for (int sec = TIME_TO_WAIT ; sec >= 0 ; sec--){
			sprintf(title, PLAY_AGAIN, sec);
			update_window_title(title);
			usleep(1000000);
		}
		current_state = INIT;
	}
	else if (current_player == black && board[wkingx][wkingy].is_check(board)){
		sprintf(title, WHITE_CHECK);
		update_window_title(title);
	}
	else if (current_player == white && board[bkingx][bkingy].is_check(board)){
		sprintf(title, BLACK_CHECK);
		update_window_title(title);
	}
	if (current_state != INIT){
		current_player = invert_color(current_player);
		current_state = PLAYING;
		if (current_player == white) update_window_title(WHITE_TURN);
		else if (current_player == black) update_window_title(BLACK_TURN);
	}
	if (current_state == INIT)
		this->init();
}

void update_moving_title(int i, int j, ChessPiece selected_piece){
	char t1[2]; char t2[2]; 
	convert(i, j, t1); convert(selected_piece.x, selected_piece.y, t2); 
	char title[100];
	if (selected_piece.color == white) sprintf(title, MOVING_MESSAGE, "White", selected_piece.name.c_str(), t2, t1);
	else if (selected_piece.color == black) sprintf(title, MOVING_MESSAGE, "Black", selected_piece.name.c_str(), t2, t1);
	update_window_title(title);
}

void convert(int i, int j, char * tmp){
	sprintf(tmp, "%c%d", static_cast<char>(65+j), i+1);
}
