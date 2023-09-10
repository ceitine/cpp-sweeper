#pragma once
#include "raylib.h"

extern Texture2D gridTex;
extern Texture2D emptyTex;
extern Texture2D flagTex;
extern Texture2D mineTex;
extern Texture2D logoTex;

extern Font myFont;

void init_gui();

void draw_string( const char* text, Vector2 pos, Color color, int fontSize = 20, float rotation = 0 );
bool mouse_in_rect( int x, int y, int w, int h );
void render_button( const char* text, Rectangle rect, void (*onclick)() = nullptr, int fontSize = 26 );