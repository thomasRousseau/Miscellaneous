#include "Graphics.h"
using namespace std;

#include <unistd.h>

#define WOOD_TEXTURE_RATIO 2

static unsigned int CURRENT_WINDOW_HEIGHT;
static unsigned int CURRENT_WINDOW_WIDTH;
static Vec3f lightSource = Vec3f(0.0f, 1000.0f, 0.0f);
Camera camera;

Mesh rook_mesh;
Mesh pawn_mesh;
Mesh bishop_mesh;
Mesh king_mesh;
Mesh white_knight_mesh;
Mesh black_knight_mesh;
Mesh queen_mesh;
Mesh table_mesh;

#define TABLE_Y_OFFSET -0.5

vector<int> mouse_hover_mesh_index {-1, -1};
vector<int> mouse_click_mesh_index {-1, -1};
vector<int> possible_moves = vector<int>(); 

void init_graphics(){
   GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 20.0 };
   GLfloat light0_position[] = {0.0, 100.0, 0.0};
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   init_meshes();
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
   init_textures();
}

// loads all the meshes (OFF files)
// rotates, scales and initiliaze fixed offsets
void init_meshes(){
	table_mesh.loadOFF("off/table.off");
    table_mesh.scale(3);
    table_mesh.rotate(x, -M_PI/2.0);

    rook_mesh.loadOFF("off/rook.off");
	rook_mesh.scale(FACTOR_SQUARE*FACTOR_ROOK);
    pawn_mesh.loadOFF("off/pawn.off");
	pawn_mesh.scale(FACTOR_SQUARE*FACTOR_PAWN);
    bishop_mesh.loadOFF("off/bishop.off");
	bishop_mesh.scale(FACTOR_SQUARE*FACTOR_BISHOP);
    white_knight_mesh.loadOFF("off/knight.off");
	white_knight_mesh.scale(FACTOR_SQUARE*FACTOR_KNIGHT);
    white_knight_mesh.rotate(y, M_PI/2.0);
    black_knight_mesh.loadOFF("off/knight.off");
    black_knight_mesh.scale(FACTOR_SQUARE*FACTOR_KNIGHT);
    black_knight_mesh.rotate(y, -M_PI/2.0);
    king_mesh.loadOFF("off/king.off");
	king_mesh.scale(FACTOR_SQUARE*FACTOR_KING);
    queen_mesh.loadOFF("off/queen.off");
	queen_mesh.scale(FACTOR_SQUARE*FACTOR_QUEEN);
}

// update the locals window size variables
void graphics_resize(int w, int h){
    CURRENT_WINDOW_WIDTH = w;
    CURRENT_WINDOW_HEIGHT = h;
}

/***********************************/
/* Drawing function (table/pieces) */
/***********************************/

// TODO : Make everything looks super awesome

// draw the given mesh with the given offset
void draw_piece(Mesh mesh, float offset_x, float offset_y, float offset_z, bool white){
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < mesh.T.size(); i++) 
        for (unsigned int j = 0; j < 3; j++) {
            Vertex & v = mesh.V[mesh.T[i].v[j]];
            Vertex vtmp = Vertex(v);
            vtmp.p[0] += offset_x;
            vtmp.p[1] += offset_y;
			vtmp.p[2] += offset_z;     
                
            GLfloat mat_ambiant[] = {float(white) * 1.0f, float(white) * 1.0f, float(white) * 1.0f, 1.0f};
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiant);

            glNormal3f (vtmp.n[0], vtmp.n[1], vtmp.n[2]); // Specifies current normal vertex   
            glVertex3f (vtmp.p[0], vtmp.p[1], vtmp.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
        }
    glEnd();
}

vector<vector<float> > around_square(Vertex v1, Vertex v2, Vertex v3){
    vector<vector<float> > square = {{0, 0},{0, 0}};
    square[0][0] = min(v1.p[0], min(v2.p[0], v3.p[0]));
    square[0][1] = min(v1.p[2], min(v2.p[2], v3.p[2]));
    square[1][0] = max(v1.p[0], max(v2.p[0], v3.p[0]));
    square[1][1] = max(v1.p[2], max(v2.p[2], v3.p[2]));
    return square;
}

void draw_wood(unsigned int i){
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, medium_texture);    
    GLfloat mat_ambiant[] = { 1.0, 1.0, 1.0, 1.0 };
    glMaterialfv(GL_FRONT, GL_DIFFUSE,mat_ambiant);
    glBegin (GL_TRIANGLES);
    const Vertex & v1 = table_mesh.V[table_mesh.T[i].v[0]]; Vertex v1tmp = Vertex(v1); v1tmp.p[1] += TABLE_Y_OFFSET;
    const Vertex & v2 = table_mesh.V[table_mesh.T[i].v[1]]; Vertex v2tmp = Vertex(v2); v2tmp.p[1] += TABLE_Y_OFFSET;
    const Vertex & v3 = table_mesh.V[table_mesh.T[i].v[2]]; Vertex v3tmp = Vertex(v3); v3tmp.p[1] += TABLE_Y_OFFSET;
    
    float epsilon = 0.000001;
    if (abs(v1tmp.p[1]-v2tmp.p[1])<=epsilon && abs(v2tmp.p[1]-v3tmp.p[1])<=epsilon){
        glColor3f (1, 1, 1);
        vector<vector<float> > square = around_square(v1tmp, v2tmp, v3tmp);
        float w = square[0][0]-square[1][0];
        float h = square[0][1]-square[1][1];
        float m = max(w, h);
        glColor3ub(200,200,200);
        glTexCoord2f(WOOD_TEXTURE_RATIO*(v1tmp.p[0]-square[0][0])/m,WOOD_TEXTURE_RATIO*(v1tmp.p[2]-square[0][1])/m);
        glNormal3f (v1tmp.n[0], v1tmp.n[1], v1tmp.n[2]); 
        glVertex3f (v1tmp.p[0], v1tmp.p[1], v1tmp.p[2]); 
        glColor3ub(200,200,200);
        glTexCoord2f(WOOD_TEXTURE_RATIO*(v2tmp.p[0]-square[0][0])/m,WOOD_TEXTURE_RATIO*(v2tmp.p[2]-square[0][1])/m);
        glNormal3f (v2tmp.n[0], v2tmp.n[1], v2tmp.n[2]); 
        glVertex3f (v2tmp.p[0], v2tmp.p[1], v2tmp.p[2]); 
        glColor3ub(200,200,200);
        glTexCoord2f(WOOD_TEXTURE_RATIO*(v3tmp.p[0]-square[0][0])/m,WOOD_TEXTURE_RATIO*(v3tmp.p[2]-square[0][1])/m);
        glNormal3f (v3tmp.n[0], v3tmp.n[1], v3tmp.n[2]); 
        glVertex3f (v3tmp.p[0], v3tmp.p[1], v3tmp.p[2]); 
        glColor3ub(200,200,200);
    }
    else{
        glColor3ub(200,200,200);
        glTexCoord2f(0,0);
        glNormal3f (v1tmp.n[0], v1tmp.n[1], v1tmp.n[2]); 
        glVertex3f (v1tmp.p[0], v1tmp.p[1], v1tmp.p[2]); 
        glColor3ub(200,200,200);
        glTexCoord2f(2,0);
        glNormal3f (v2tmp.n[0], v2tmp.n[1], v2tmp.n[2]); 
        glVertex3f (v2tmp.p[0], v2tmp.p[1], v2tmp.p[2]); 
        glColor3ub(200,200,200);
        glTexCoord2f(2,2);
        glNormal3f (v3tmp.n[0], v3tmp.n[1], v3tmp.n[2]); 
        glVertex3f (v3tmp.p[0], v3tmp.p[1], v3tmp.p[2]); 
        glColor3ub(200,200,200);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

// draw the whole table
void draw_table(){
    GLfloat mat_ambiant[] = {0.7, 0.7, 0.7, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambiant);
    GLfloat mat_diffuse[4];
    for (unsigned int i = 0; i < table_mesh.T.size(); i++){
        bool is_square = false;
        if ( mesh_is_black(i) )         { mat_diffuse[0] = 0.0; mat_diffuse[1] = 0.0; mat_diffuse[2] = 0.0; mat_diffuse[3] = 1.0; is_square = true;}
        else if ( mesh_is_white(i) )    { mat_diffuse[0] = 1.0; mat_diffuse[1] = 1.0; mat_diffuse[2] = 1.0; mat_diffuse[3] = 1.0; is_square = true;}
        if ( mesh_is_possible_move(i) ) { mat_diffuse[0] = 0.0; mat_diffuse[1] = 1.0; mat_diffuse[2] = 0.0; mat_diffuse[3] = 1.0; is_square = true;}
        if ( mesh_is_hover(i) )         { mat_diffuse[0] = 0.0; mat_diffuse[1] = 0.0; mat_diffuse[2] = 0.6; mat_diffuse[3] = 1.0; is_square = true;}
        if ( mesh_is_click(i) )         { mat_diffuse[0] = 0.0; mat_diffuse[1] = 0.0; mat_diffuse[2] = 1.0; mat_diffuse[3] = 1.0; is_square = true;}
        
        if (is_square){
            glMaterialfv(GL_FRONT, GL_DIFFUSE,mat_diffuse);
            glBegin (GL_TRIANGLES);
            for (unsigned int j = 0; j < 3; j++) {
                Vertex & v = table_mesh.V[table_mesh.T[i].v[j]];
                Vertex vtmp = Vertex(v);
                vtmp.p[1] += TABLE_Y_OFFSET;
                glNormal3f (vtmp.n[0], vtmp.n[1], vtmp.n[2]); // Specifies current normal vertex   
                glVertex3f (vtmp.p[0], vtmp.p[1], vtmp.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)    
            }
            glEnd();
        }

        else {
            draw_wood(i);
        }
    }   
}

// draw the given mesh to the given square of the board
void draw_piece_to_square(Mesh mesh, int x, int y, Color color){
    int indexp = squares_vertex_index[x][y];
    int indexm = squares_vertex_index[x+1][y+1];
    Vec3f min_color, max_color;
    if (color == white) {min_color = Vec3f(0.2, 0.2, 0.2); max_color = Vec3f(0.9, 0.9, 0.9); }
    if (color == black) {min_color = Vec3f(0.01, 0.01, 0.01); max_color = Vec3f(0.2, 0.2, 0.2); }
    draw_piece(mesh, (table_mesh.V[indexm].p[0]+table_mesh.V[indexp].p[0])/2.0, TABLE_Y_OFFSET + table_mesh.max_alt - mesh.min_alt, (table_mesh.V[indexm].p[2]+table_mesh.V[indexp].p[2])/2.0, color == white);
}

// draw the whole board given the board the board variable
void draw_board(){
    for (unsigned int i = 0 ; i < 8 ; i++)
        for (unsigned int j = 0 ; j < 8 ; j++)
            switch (board[i][j].type){
                case queen:
                draw_piece_to_square(queen_mesh, j, i, board[i][j].color);
                break;
                case king:
                draw_piece_to_square(king_mesh, j, i, board[i][j].color);
                break;
                case bishop:
                draw_piece_to_square(bishop_mesh, j, i, board[i][j].color);
                break;
                case rook:
                draw_piece_to_square(rook_mesh, j, i, board[i][j].color);
                break;
                case knight:
                if (board[i][j].color == white) draw_piece_to_square(white_knight_mesh, j, i, board[i][j].color);
                else if (board[i][j].color == black) draw_piece_to_square(black_knight_mesh, j, i, board[i][j].color);
                break;
                case pawn:
                draw_piece_to_square(pawn_mesh, j, i, board[i][j].color);
                break;
                default:
                break;
            }
}

// draw a moving piece
void draw_moving_pieces(Game &game, int counter){
    int init_p = squares_vertex_index[game.selected_piece.y][game.selected_piece.x];
    int init_m = squares_vertex_index[game.selected_piece.y+1][game.selected_piece.x+1];
    int end_p = squares_vertex_index[game.destination[1]][game.destination[0]];
    int end_m = squares_vertex_index[game.destination[1]+1][game.destination[0]+1];
    Vec3f init = (table_mesh.V[init_m].p+table_mesh.V[init_p].p)/2.0;
    Vec3f end = (table_mesh.V[end_m].p+table_mesh.V[end_p].p)/2.0;
    float ratio = float(counter)/game.max_counter;
    Vec3f pos = (1-ratio) * init + ratio * end; 
    Mesh mesh;
    switch (game.selected_piece.type){
        case queen:
        mesh = queen_mesh; break;
        case king:
        mesh = king_mesh; break;
        case bishop:
        mesh = bishop_mesh; break;
        case rook:
        mesh = rook_mesh; break;
        case knight:
        if (game.selected_piece.color == white) mesh = white_knight_mesh; 
        else if (game.selected_piece.color == black) mesh = black_knight_mesh; 
        break;
        case pawn:
        mesh = pawn_mesh; break;
        case blank:
        break;
    }
    draw_piece(mesh, pos[0], TABLE_Y_OFFSET + table_mesh.max_alt - mesh.min_alt, pos[2], game.selected_piece.color == white);
}

void draw_moving_pieces_short_castling(Game game, int counter){
    int init_p = squares_vertex_index[7][game.selected_piece.x];
    int init_m = squares_vertex_index[8][game.selected_piece.x+1];
    int end_p =  squares_vertex_index[5][game.selected_piece.x];
    int end_m =  squares_vertex_index[6][game.selected_piece.x+1];
    Vec3f init = (table_mesh.V[init_m].p+table_mesh.V[init_p].p)/2.0;
    Vec3f end = (table_mesh.V[end_m].p+table_mesh.V[end_p].p)/2.0;
    float ratio = float(counter)/game.max_counter;
    Vec3f pos = (1-ratio) * init + ratio * end; 
    draw_piece(rook_mesh, pos[0], TABLE_Y_OFFSET + table_mesh.max_alt - rook_mesh.min_alt, pos[2], game.selected_piece.color == white);
}

void draw_moving_pieces_long_castling(Game game, int counter){
    int init_p = squares_vertex_index[0][game.selected_piece.x];
    int init_m = squares_vertex_index[1][game.selected_piece.x+1];
    int end_p =  squares_vertex_index[2][game.selected_piece.x];
    int end_m =  squares_vertex_index[3][game.selected_piece.x+1];
    Vec3f init = (table_mesh.V[init_m].p+table_mesh.V[init_p].p)/2.0;
    Vec3f end = (table_mesh.V[end_m].p+table_mesh.V[end_p].p)/2.0;
    float ratio = float(counter)/game.max_counter;
    Vec3f pos = (1-ratio) * init + ratio * end; 
    draw_piece(rook_mesh, pos[0], TABLE_Y_OFFSET + table_mesh.max_alt - rook_mesh.min_alt, pos[2], game.selected_piece.color == white);
}

/******************************************/
/* 2D <-> 3D relationships and conversion */
/******************************************/

// returns whether the ray originating from o with the direction w
// intersects the triangle p0, p1, p2 
bool intersectionRayonTriangle (Vec3f o, Vec3f w, Vec3f p0, Vec3f p1, Vec3f p2){
    float epsilon = 0.00001;
    Vec3f e0 = p1 - p0;
    Vec3f e1 = p2 - p0;
    Vec3f n = cross(e0, e1);
    n.normalize();
    Vec3f q = cross(w, e1);
    float a = dot(e0, q);
    if (dot(n, w) >= 0 || abs(a) < epsilon)
        return false;
    Vec3f s = (o - p0) / a;
    Vec3f r = cross(s, e0);
    float b0 = dot(s, q);
    float b1 = dot(r, w);
    float b2 = 1 - b0 - b1;
    if (b0 < 0 || b1 < 0 || b2 < 0)
        return false;
    float t = dot(e1, r);
    if (t >= 0)
        return true;
    return false;
}

// converts the 2D (x, y) coordonnates of the click into the 
// corresponding 3D point (intersection between the camera-click ray
// and the plan containing the origin and orthogonal to the camera vector)
void get_reverse_coord(int x, int y, float &p1, float &p2, float &p3 ){
    float m[4][4];
    float _curquat[4];
    _curquat[0] = - camera.curquat[0];
    _curquat[1] = - camera.curquat[1];
    _curquat[2] = - camera.curquat[2];
    _curquat[3] = - camera.curquat[3];
    build_rotmatrix(m, _curquat);
    float a[3];
    float q[3];
    Vec3f c;
    camera.getPos(c[0], c[1], c[2]);
    float H = c.length()*tan(camera.getFovAngle()*M_PI/180)/2.42; 
    float W = camera.getAspectRatio()*H;
    q[0] = (x - CURRENT_WINDOW_WIDTH/2.0)/(CURRENT_WINDOW_WIDTH/2.0)*W;
    q[1] = -(y - CURRENT_WINDOW_HEIGHT/2.0)/(CURRENT_WINDOW_HEIGHT/2.0)*H;
    q[2] = c.length();
    mult(m, q, a);
    p1 = a[0] - c[0];
    p2 = a[1] - c[1]; 
    p3 = a[2] - c[2];
}

// update the 2 mesh indexes from a given click 
vector<int> convert_coordonates(int x, int y){
    Vec3f c, p, click;
    vector<int> m = {-1, -1};
    camera.getPos(c[0], c[1], c[2]);
    get_reverse_coord(x, y, click[0], click[1], click[2]);
    for (unsigned i = 0 ; i < squares.size() ; i++){
        int i1 = table_mesh.T[squares[i]].v[0];
        int i2 = table_mesh.T[squares[i]].v[1];
        int i3 = table_mesh.T[squares[i]].v[2];
        Vec3f v1(table_mesh.V[i1].p[0], table_mesh.V[i1].p[1] + TABLE_Y_OFFSET, table_mesh.V[i1].p[2]);
        Vec3f v2(table_mesh.V[i2].p[0], table_mesh.V[i2].p[1] + TABLE_Y_OFFSET, table_mesh.V[i2].p[2]);
        Vec3f v3(table_mesh.V[i3].p[0], table_mesh.V[i3].p[1] + TABLE_Y_OFFSET, table_mesh.V[i3].p[2]);
        if (intersectionRayonTriangle(c, click-c, v1, v2, v3)){
            m[0] = squares[(i/2)*2];
            m[1] = squares[(i/2)*2+1];
            break;}
    }
    return m;
}

/***************************************************/
/* Graphical mouse events handlers (click / hover) */
/***************************************************/

// update the mouse_hover_mesh_index given the mouse position 
void handle_mouse_hover(int x, int y){
    vector<int> m = convert_coordonates(x, y);
    mouse_hover_mesh_index[0] = m[0];
    mouse_hover_mesh_index[1] = m[1];
}

// update the mouse_click_mesh_index given the mouse position
void handle_mouse_click(int x, int y){
    vector<int> m = convert_coordonates(x, y);
    mouse_click_mesh_index[0] = m[0];
    mouse_click_mesh_index[1] = m[1];
}

/*****************************************/
/* FACE <-> BOARD relationship functions */
/*****************************************/

// convert the face index m into board (i, j) coordonnates
void convert_mesh_to_board(int m, int & i, int & j){
    unsigned int index = find(squares.begin(), squares.end(), m) - squares.begin();
    if ( index >= squares.size() ){
        i = -1;
        j = -1;
    }
    else
    {
        index /= 2; 
        i = index/8;
        j = index%8;
    }
}

// returns whether the given face is black
bool mesh_is_black(int m){
    int i, j;
    convert_mesh_to_board(m, i, j);
    return (i >= 0 && j >=0 && (i+j)%2==0 ); 
}

// returns whether the given face is white
bool mesh_is_white(int m){
    int i, j;
    convert_mesh_to_board(m, i, j);
    return (i >= 0 && j >=0 && (i+j)%2==1 );
}

// returns whether the given face belong to the possible_move vector
bool mesh_is_possible_move(int m){
    unsigned int index = find(possible_moves.begin(), possible_moves.end(), m) - possible_moves.begin();
    return (index < possible_moves.size());
}

// returns whether the face is currently hovered
bool mesh_is_hover(int m){
    return (mouse_hover_mesh_index[0] == m || mouse_hover_mesh_index[1] == m);
}

// returns whether the face is currently clicked
bool mesh_is_click(int m){
    return (mouse_click_mesh_index[0] == m || mouse_click_mesh_index[1] == m);
}

/**********************************/
/* Pieces moves related functions */
/**********************************/

void add_to_possible_moves(vector<vector<int> > list){
    possible_moves.clear();
    for (unsigned int k = 0 ; k < list.size() ; k++){
        possible_moves.push_back(squares[2*(8*list[k][0]+list[k][1])]);
        possible_moves.push_back(squares[2*(8*list[k][0]+list[k][1])+1]);
    }
}

void clear_possible_moves(){
    possible_moves.clear();
}

bool is_possible_move(int i, int j){
    unsigned int index = find(possible_moves.begin(), possible_moves.end(), squares[2*(8*i+j)]) - possible_moves.begin();
    return ( index < possible_moves.size() );
}