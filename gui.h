#pragma once
#include "raylib.h"
#include <vector>
#include <string>

extern Texture2D gridTex;
extern Texture2D emptyTex;
extern Texture2D flagTex;
extern Texture2D mineTex;
extern Texture2D logoTex;
extern Texture2D selectionTex;
extern Texture2D arrowTex;

extern Font myFont;

void init_gui();

struct Dropdown
{
public:
	int option;
	bool down;
	std::vector<std::string> options;

	Dropdown( std::vector<std::string> values )
	{
		option = 0;
		down = false;
		options = values;
	}
};

void draw_string( const char* text, Vector2 pos, Color color, int fontSize = 20 );
bool mouse_in_rect( int x, int y, int w, int h );
void render_button( const char* text, Rectangle rect, void (*onclick)() = nullptr, int fontSize = 26 );
void draw_background( Rectangle rect );
void render_dropdown( Rectangle rect, Dropdown* dropdown, int fontSize = 22 );