#include "gui.h"
#include <iostream>

const Color BUTTON_BORDER = { 155, 155, 155, 255 };
const Color BUTTON_BACK = { 210, 210, 210, 255 };
const int BORDER_WIDTH = 1;

Texture2D gridTex;
Texture2D emptyTex;
Texture2D flagTex;
Texture2D mineTex;
Texture2D logoTex;

Font myFont;

void init_gui()
{
	gridTex = LoadTexture( "assets/grid.png" );
	emptyTex = LoadTexture( "assets/grid_empty.png" );
	flagTex = LoadTexture( "assets/flag.png" );
	mineTex = LoadTexture( "assets/mine.png" );
	logoTex = LoadTexture( "assets/logo.png" );
															  
	SetTextureFilter( gridTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( emptyTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( flagTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( mineTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( logoTex, TEXTURE_FILTER_POINT );

	myFont = LoadFont( "assets/font.ttf" );
}

void draw_string( const char* text, Vector2 pos, Color color, int fontSize )
{
	DrawTextEx( myFont, text, Vector2{ 1 + pos.x, 1 + pos.y }, fontSize, 0, BLACK );
	DrawTextEx( myFont, text, pos, fontSize, 0, color );
}

bool mouse_in_rect( int x, int y, int w, int h )
{
	Vector2 pos = GetMousePosition();
	return pos.x >= x && pos.x <= x + w
		&& pos.y >= y && pos.y <= y + h;
}

void render_button( const char* text, Rectangle rect, void (*onclick)(), int fontSize )
{
	bool hovered = mouse_in_rect( rect.x, rect.y, rect.width, rect.height );
	if ( hovered )
	{
		// Call onclick...
		if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) && onclick != nullptr )
			onclick();
	}

	DrawRectangle( rect.x, rect.y, rect.width, rect.height, BUTTON_BORDER );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH * 2, rect.height - BORDER_WIDTH * 2, BUTTON_BACK );

	Vector2 size = MeasureTextEx( myFont, text, fontSize, 0 );
	Vector2 pos = { rect.x + rect.width / 2 - size.x / 2, rect.y + rect.height / 2 - size.y / 2 };
	draw_string( text, pos, RAYWHITE, fontSize );
}