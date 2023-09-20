#pragma once
#include "raylib.h"
#include <vector>

extern Texture2D gridTex;
extern Texture2D emptyTex;
extern Texture2D flagTex;
extern Texture2D mineTex;
extern Texture2D logoTex;
extern Texture2D selectionTex;

extern Font myFont;

void init_gui();

template <typename T>
struct Dropdown
{
public:
	int option;
	bool down;
	T* options;

	Dropdown( T* options )
	{
		option = 0;
		down = false;
		this->options = options;
	}

	T get_value()
	{
		return options[option];
	}
};

void draw_string( const char* text, Vector2 pos, Color color, int fontSize = 20 );
bool mouse_in_rect( int x, int y, int w, int h );
void render_button( const char* text, Rectangle rect, void (*onclick)() = nullptr, int fontSize = 26 );
void draw_background( Rectangle rect );
template <typename T>
void render_dropdown( Rectangle rect, Dropdown<T>* dropdown );