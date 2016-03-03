#include "Main.h"

static unsigned int DEFAULT_SCREENWIDTH = 1024;
static unsigned int DEFAULT_SCREENHEIGHT = 768;

static string appTitle ("3D ChessGame using OpenGL - IGR202 Project");
static GLint window;
static bool fullScreen = false;
static Game game;

static int counter = -1;


void init () {
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
	glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.2f, 1.0f); // Background color
    init_graphics();                       // loading_pieces
    game = Game();
    game.init();
    camera.resize(DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
}

void drawScene () {
    draw_table();
    draw_board();
    if (game.current_state == MOVING){
        draw_moving_pieces(game, counter);
        if (game.castling == SHORT)
            draw_moving_pieces_short_castling(game, counter);
        if (game.castling == LONG)
            draw_moving_pieces_long_castling(game, counter);
    }
}

void reshape(int w, int h) {
    camera.resize (w, h);
    graphics_resize(w, h);
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply (); 
    drawScene ();
    glFlush ();
    glutSwapBuffers (); 
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
    case 'f':
        if (fullScreen) {
            glutReshapeWindow (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
            fullScreen = false;
        } else {
            glutFullScreen ();
            fullScreen = true;
        }      
        break;
    case 'q':
    case 27:
        exit (0);
        break;
    }
}

void mouse (int button, int state, int x, int y) {
    game.click_handler(button, state, x, y);
    camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
    camera.handleMouseMoveEvent (x, y);
}

void passive_motion(int x, int y) {
    handle_mouse_hover(x, y);
}

void idle () {
    if (game.current_state == MOVING){
        if (counter < 0)
            counter = 0;
        else if (counter >= 0)
            counter++;
        if (counter >= game.max_counter){
            game.end_move();
            counter = -1;
        }
    }
    glutPostRedisplay (); 
}

int main (int argc, char ** argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    window = glutCreateWindow (appTitle.c_str ());
    init ();
    glutIdleFunc (idle);
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutMotionFunc (motion);
    glutPassiveMotionFunc (passive_motion);
    glutMouseFunc (mouse);
    glutMainLoop ();
    return 0;
}

void update_window_title(string s){
    static char winTitle [128];
    sprintf( winTitle, s.c_str() ); 
    glutSetWindowTitle (winTitle);
}

void reset_counter(){
    counter = -1;
}

