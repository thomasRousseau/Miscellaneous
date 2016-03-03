#include "ChessPieces.h"

using namespace std;

int wkingx;
int wkingy;
int bkingx;
int bkingy; 

Board board;

// returns a fully loaded chess board with both black and white
void new_board(){
	wkingx = 0;
	wkingy = 4;
	bkingx = 7;
	bkingy = 4; 
	board.resize(8, vector<ChessPiece>(8, None()));	
	for (unsigned int i = 0 ; i < 8 ; i++)
		for (unsigned int j = 0 ; j < 8 ; j++)
			board[i][j] = None();
	// rooks
	board[0][0] = Rook(0, 0, white);
	board[0][7] = Rook(0, 7, white);
	board[7][0] = Rook(7, 0, black);
	board[7][7] = Rook(7, 7, black);
	// knights
	board[0][1] = Knight(0, 1, white);
	board[0][6] = Knight(0, 6, white);
	board[7][1] = Knight(7, 1, black);
	board[7][6] = Knight(7, 6, black);
	// bishops
	board[0][2] = Bishop(0, 2, white);
	board[0][5] = Bishop(0, 5, white);
	board[7][2] = Bishop(7, 2, black);
	board[7][5] = Bishop(7, 5, black);
	// queens
	board[0][3] = Queen(0, 3, white);
	board[7][3] = Queen(7, 3, black);
	// kings
	board[0][4] = King(0, 4, white);
	board[7][4] = King(7, 4, black);
	// pawns
	for (int i = 0 ; i < 8 ; i++){
		board[1][i] = Pawn(1, i, white);
		board[6][i] = Pawn(6, i, black);
	}
} 

// debug funtion printing the board
void print_board(Board & board){
	for (int i = 0 ; i < 8 ; i++){
		cerr << "-----------------" << endl << "|";
		for (int j = 0 ; j < 8 ; j++){
			switch (board[i][j].type){
				case queen:
					cerr << "Q|";
					break;
				case king:
					cerr << "K|";
					break;
				case rook:
					cerr << "R|";
					break;
				case bishop:
					cerr << "B|";
					break;
				case knight:
					cerr << "k|";
					break;
				case pawn:
					cerr << "P|";
					break;
				case blank:
					cerr << " |";
					break;
			}
		}
		cerr << endl;
	}
	cerr << "-----------------" << endl;	
}

// returns whether a long castling ("roque long") is possible
bool long_castling(Color c){ 
	if (c == white && board[0][4].hasnt_moved && board[0][0].hasnt_moved && board[0][1].type == blank && board[0][2].type == blank && board[0][3].type == blank) return true;
	if (c == black && board[7][4].hasnt_moved && board[7][0].hasnt_moved && board[7][1].type == blank && board[7][2].type == blank && board[7][3].type == blank) return true;
	return false;
}

// returns whether a small_castling ("roque court") is possible
bool small_castling(Color c){
	if (c == white && board[0][4].hasnt_moved && board[0][7].hasnt_moved && board[0][6].type == blank && board[0][5].type == blank) return true;
	if (c == black && board[7][4].hasnt_moved && board[7][7].hasnt_moved && board[7][6].type == blank && board[7][5].type == blank) return true;
	return false;
}

// returns the list of possible moves for a knight
vector<vector<int> > ChessPiece::knight_moves(Board & board, bool checking){
	vector<vector<int> > possible_moves;
	vector<int> v;
	v.push_back(x+2); v.push_back(y+1); possible_moves.push_back(v); v.clear();
	v.push_back(x+2); v.push_back(y-1); possible_moves.push_back(v); v.clear();
	v.push_back(x-2); v.push_back(y+1); possible_moves.push_back(v); v.clear();
	v.push_back(x-2); v.push_back(y-1); possible_moves.push_back(v); v.clear();
	v.push_back(x+1); v.push_back(y+2); possible_moves.push_back(v); v.clear();
	v.push_back(x-1); v.push_back(y+2); possible_moves.push_back(v); v.clear();
	v.push_back(x+1); v.push_back(y-2); possible_moves.push_back(v); v.clear();
	v.push_back(x-1); v.push_back(y-2); possible_moves.push_back(v); v.clear();	
	// deleting out of range moves
	unsigned int i = 0;
	while (i < possible_moves.size()){
		if (possible_moves[i][0] >= 8 || possible_moves[i][1] >= 8 || possible_moves[i][0] < 0 || possible_moves[i][1] < 0) possible_moves.erase(possible_moves.begin()+i); else i++;
	}
	i = 0;	
	// deleting friendly kill moves
	while (i < possible_moves.size()){
		if(board[possible_moves[i][0]][possible_moves[i][1]].color == color) possible_moves.erase(possible_moves.begin()+i); else i++;
	}
	if (checking)
		return remove_check(possible_moves, x, y);
	else
		return possible_moves;
}

// returns the list of possible moves for a pawn
vector<vector<int> > ChessPiece::pawn_moves(Board & board, bool checking){
	vector<vector<int> > possible_moves;
	vector<int> v;
	if (color == white){
		if (y > 0 && board[x+1][y-1].color == black){v.push_back(x+1); v.push_back(y-1); possible_moves.push_back(v); v.clear();}
		if (y < 7 && board[x+1][y+1].color == black){v.push_back(x+1); v.push_back(y+1); possible_moves.push_back(v); v.clear();}
		if (x < 7 && board[x+1][y].type == blank){v.push_back(x+1); v.push_back(y); possible_moves.push_back(v); v.clear();}
		if (x == 1 && board[x+1][y].type == blank && board[x+2][y].type == blank){v.push_back(x+2); v.push_back(y); possible_moves.push_back(v); v.clear();}
	}
	if (color == black){
		if (y > 0 && board[x-1][y-1].color == white){v.push_back(x-1); v.push_back(y-1); possible_moves.push_back(v); v.clear();}
		if (y < 7 && board[x-1][y+1].color == white){v.push_back(x-1); v.push_back(y+1); possible_moves.push_back(v); v.clear();}
		if (x > 0 && board[x-1][y].type == blank){v.push_back(x-1); v.push_back(y); possible_moves.push_back(v); v.clear();}
		if (x == 6 && board[x-1][y].type == blank && board[x-2][y].type == blank){v.push_back(x-2); v.push_back(y); possible_moves.push_back(v); v.clear();}
	}
	if (checking)
		return remove_check(possible_moves, x, y);
	else
		return possible_moves;
}

// returns the list of possible moves for a rook
vector<vector<int> > ChessPiece::rook_moves(Board & board, bool checking){
	vector<vector<int> > possible_moves;
	vector<int> v;
	Color foe_color;
	if (color == white) foe_color = black;
	else foe_color = white;
	int xx = x-1; 
	while(xx >= 0 && board[xx][y].type == blank){v.push_back(xx); v.push_back(y); possible_moves.push_back(v); v.clear(); xx--;}
	if (xx >= 0 && board[xx][y].color == foe_color){v.push_back(xx); v.push_back(y); possible_moves.push_back(v); v.clear();}
	xx = x+1;
	while(xx <= 7 && board[xx][y].type == blank){v.push_back(xx); v.push_back(y); possible_moves.push_back(v); v.clear(); xx++;}
	if (xx <= 7 && board[xx][y].color == foe_color){v.push_back(xx); v.push_back(y); possible_moves.push_back(v); v.clear();}
	int yy = y-1;
	while(yy >= 0 && board[x][yy].type == blank){v.push_back(x); v.push_back(yy); possible_moves.push_back(v); v.clear(); yy--;}
	if (yy >= 0 && board[x][yy].color == foe_color){v.push_back(x); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	yy = y+1;
	while(yy <= 7 && board[x][yy].type == blank){v.push_back(x); v.push_back(yy); possible_moves.push_back(v); v.clear(); yy++;}
	if (yy <= 7 && board[x][yy].color == foe_color){v.push_back(x); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	if (checking)
		return remove_check(possible_moves, x, y);
	else
		return possible_moves;
}

// returns the list of possible moves for a bishop
vector<vector<int> > ChessPiece::bishop_moves(Board & board, bool checking){
	vector<vector<int> > possible_moves;
	vector<int> v;
	Color foe_color;
	if (color == white) foe_color = black;
	else foe_color = white;
	int xx = x-1; int yy = y-1;
	while(xx >= 0 && yy >= 0 && board[xx][yy].type == blank){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear(); xx--; yy--;}
	if (xx >= 0 && yy >= 0 && board[xx][yy].color == foe_color){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	xx = x+1; yy = y+1;
	while(xx <= 7 && yy <= 7 && board[xx][yy].type == blank){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear(); xx++; yy++;}
	if (xx <= 7 && yy <= 7 && board[xx][yy].color == foe_color){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	xx = x-1; yy = y+1;
	while(xx >= 0 && yy <= 7 && board[xx][yy].type == blank){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear(); xx--; yy++;}
	if (xx >= 0 && yy <= 7 && board[xx][yy].color == foe_color){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	xx = x+1; yy = y-1;
	while(yy >= 0 && xx <= 7 && board[xx][yy].type == blank){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear(); xx++; yy--;}
	if (yy >= 0 && xx <= 7 && board[xx][yy].color == foe_color){v.push_back(xx); v.push_back(yy); possible_moves.push_back(v); v.clear();}
	if (checking)
		return remove_check(possible_moves, x, y);
	else
		return possible_moves;			
}

// returns the list of possible moves for a king
vector<vector<int> > ChessPiece::king_moves(Board & board, bool checking){
	vector<vector<int> > possible_moves;
	vector<int> v;
	if (x-1>=0 && board[x-1][y].color != color){v.push_back(x-1); v.push_back(y); possible_moves.push_back(v); v.clear();}
	if (x+1<=7 && board[x+1][y].color != color){v.push_back(x+1); v.push_back(y); possible_moves.push_back(v); v.clear();}
	if (y-1>=0 && board[x][y-1].color != color){v.push_back(x); v.push_back(y-1); possible_moves.push_back(v); v.clear();}
	if (y+1<=7 && board[x][y+1].color != color){v.push_back(x); v.push_back(y+1); possible_moves.push_back(v); v.clear();}
	if (x-1>=0 && y-1>=0 && board[x-1][y-1].color != color){v.push_back(x-1); v.push_back(y-1); possible_moves.push_back(v); v.clear();}
	if (x+1<=7 && y-1>=0 && board[x+1][y-1].color != color){v.push_back(x+1); v.push_back(y-1); possible_moves.push_back(v); v.clear();}
	if (x-1>=0 && y+1<=7 && board[x-1][y+1].color != color){v.push_back(x-1); v.push_back(y+1); possible_moves.push_back(v); v.clear();}
	if (x+1<=7 && y+1<=7 && board[x+1][y+1].color != color){v.push_back(x+1); v.push_back(y+1); possible_moves.push_back(v); v.clear();}
	if (color == white && small_castling(white)){v.push_back(0); v.push_back(6); possible_moves.push_back(v); v.clear();}
	if (color == black && small_castling(black)){v.push_back(7); v.push_back(6); possible_moves.push_back(v); v.clear();}
	if (color == white && long_castling(white)){v.push_back(0); v.push_back(1); possible_moves.push_back(v); v.clear();}
	if (color == black && long_castling(black)){v.push_back(7); v.push_back(7); possible_moves.push_back(v); v.clear();}
	if (checking)
		return remove_check(possible_moves, x, y);
	else
		return possible_moves;
}

// returns the list of possible moves for a queen
vector<vector<int> > ChessPiece::queen_moves(Board & board, bool checking){
	vector<vector<int> > bishop = bishop_moves(board, checking);
	vector<vector<int> > rook = rook_moves(board, checking);
	bishop.insert(bishop.end(), rook.begin(), rook.end());
	if (checking)
		return remove_check(bishop, x, y);
	else
		return bishop;
}

// swap the piece from one position to another
void ChessPiece::make_move(vector<int> destination){
	board[x][y] = None();
	x = destination[0]; y = destination[1];
	board[destination[0]][destination[1]] = *this;
	if (type == king && color == white){wkingx = x; wkingy = y; }
	if (type == king && color == black){bkingx = x; bkingy = y; }
	this->hasnt_moved = false;
}

// returns whether the king is in check
bool ChessPiece::is_check(vector<vector<ChessPiece> > & b){
	Color opposite = invert_color(color);
	for (unsigned int i = 0 ; i < 8 ; i++)
		for (unsigned int j = 0 ; j < 8 ; j++)
			if (b[i][j].color == opposite){
				vector<vector<int> > pos = b[i][j].moves(b, false);
				for (unsigned int k = 0 ; k < pos.size() ; k++){
					if (x == pos[k][0] && y == pos[k][1])
						return true;
				}
			}
	return false;
}

// returns whether the king is in checkmate
bool is_checkmate(Board & b, Color c){
	Color opposite = invert_color(c);
	for (unsigned int i = 0 ; i < 8 ; i++)
		for (unsigned int j = 0 ; j < 8 ; j++)
			if (b[i][j].color == opposite)
				if (b[i][j].moves(b, true).size() > 0)
					return false;
	return true;
}

// returns the opposite color
Color invert_color(Color c){
	if (c == white) return black;
	else if (c == black) return white;
	else return none;
}

// remove the check positions in the input list
vector<vector<int> > remove_check(vector<vector<int> > input, int x, int y){
	vector<vector<int> > output;
	for (unsigned int k = 0 ; k < input.size() ; k++){
		// board recopy
		vector<vector<ChessPiece> > tmp_board;
		tmp_board.resize(8, vector<ChessPiece>(8, None()));	
		for (unsigned int i = 0 ; i < 8 ; i++){
			for (unsigned int j = 0 ; j < 8 ; j++){
				switch(board[i][j].type){
					case knight:
					tmp_board[i][j] = Knight(i, j, board[i][j].color); break;
					case bishop:
					tmp_board[i][j] = Bishop(i, j, board[i][j].color); break;
					case rook:
					tmp_board[i][j] = Rook(i, j, board[i][j].color); break;
					case pawn:
					tmp_board[i][j] = Pawn(i, j, board[i][j].color); break;
					case queen:
					tmp_board[i][j] = Queen(i, j, board[i][j].color); break;
					case king:
					tmp_board[i][j] = King(i, j, board[i][j].color); break;
					default:
					tmp_board[i][j] = None(); break;
				}}}
		// special case for king
		if (tmp_board[x][y].type == king){
			int kingx = input[k][0]; 
			int kingy = input[k][1];
			tmp_board[kingx][kingy] = King(kingx, kingy, tmp_board[x][y].color);
			tmp_board[x][y] = None();
			if ( ! tmp_board[kingx][kingy].is_check(tmp_board)){
				vector<int> coords = {input[k][0], input[k][1]};
				output.push_back(coords);
			}
		}
		// all other cases
		else{
			// simulation
			Color our_color = tmp_board[x][y].color;
			ChessPiece p = tmp_board[x][y];
			tmp_board[p.x][p.y] = None();
			p.x = input[k][0]; p.y = input[k][1];
			tmp_board[input[k][0]][input[k][1]] = p;
			// test if check
			int kingx = 0; int kingy = 0;
			if (our_color == white){kingx = wkingx; kingy = wkingy; }
			if (our_color == black){kingx = bkingx; kingy = bkingy; }
			if ( ! tmp_board[kingx][kingy].is_check(tmp_board) ){
				vector<int> coords = {input[k][0], input[k][1]};
				output.push_back(coords);
			}
		}	
	}
	return output;
}