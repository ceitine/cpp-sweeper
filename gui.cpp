#include "gui.h"
#include <iostream>

const Color UI_HIGHLIGHT = { 220, 220, 220, 255 };
const Color UI_SHADOW = { 105, 105, 105, 255 };
const Color UI_BACK = { 190, 190, 190, 255 };
const int BORDER_WIDTH = 3;

Texture2D gridTex;
Texture2D emptyTex;
Texture2D flagTex;
Texture2D mineTex;
Texture2D logoTex;
Texture2D selectionTex;

Font myFont;

void init_gui()
{
	gridTex = LoadTexture( "assets/grid.png" );
	emptyTex = LoadTexture( "assets/grid_empty.png" );
	flagTex = LoadTexture( "assets/flag.png" );
	mineTex = LoadTexture( "assets/mine.png" );
	logoTex = LoadTexture( "assets/logo.png" );
	selectionTex = LoadTexture( "assets/selection.png" );
															  
	SetTextureFilter( gridTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( emptyTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( flagTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( mineTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( logoTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( selectionTex, TEXTURE_FILTER_POINT );

	myFont = LoadFont( "assets/font.ttf" );
}

void draw_select( Rectangle rect, float expand = 3 )
{
	float offset = expand + expand * std::sin( GetTime() * 4 );

	// Top left.
	DrawTextureRec( selectionTex, { 0, 0, 8, 8 }, { rect.x - offset, rect.y - offset }, WHITE );

	// Top right.
	DrawTextureRec( selectionTex, { 8, 0, 8, 8 }, { rect.x + rect.width + offset - expand - 4, rect.y - offset }, WHITE );

	// Bottom left.
	DrawTextureRec( selectionTex, { 0, 8, 8, 8 }, { rect.x - offset, rect.y + rect.height + offset - expand - 4 }, WHITE );

	// Bottom right.
	DrawTextureRec( selectionTex, { 8, 8, 8, 8 }, { rect.x + rect.width + offset - expand - 4, rect.y + rect.height + offset - expand - 4 }, WHITE );
}

void draw_string( const char* text, Vector2 pos, Color color, int fontSize )
{
	DrawTextEx( myFont, text, { 2 + pos.x, 2 + pos.y }, fontSize, 0, {0, 0, 0, 150} );
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
	float y = rect.y;
	bool hovered = mouse_in_rect( rect.x, rect.y, rect.width, rect.height );
	if ( hovered )
	{
		if ( IsMouseButtonDown( MOUSE_LEFT_BUTTON ) )
			y += BORDER_WIDTH / 2;

		// Call onclick...
		else if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) && onclick != nullptr )
			onclick();
	}

	DrawRectangle( rect.x, rect.y + rect.height, rect.width, BORDER_WIDTH, UI_SHADOW );
	DrawRectangle( rect.x, y, rect.width, rect.height, UI_HIGHLIGHT );
	DrawRectangle( rect.x + BORDER_WIDTH, y + BORDER_WIDTH, rect.width - BORDER_WIDTH * 2, rect.height - BORDER_WIDTH * 2, UI_BACK );

	Vector2 size = MeasureTextEx( myFont, text, fontSize, 0 );
	Vector2 pos = { rect.x + rect.width / 2 - size.x / 2, y + rect.height / 2 - size.y / 2 };
	draw_string( text, pos, RAYWHITE, fontSize );
	
	if ( hovered )
		draw_select( rect );
}

void draw_background( Rectangle rect )
{
	DrawRectangle( rect.x, rect.y, rect.width, rect.height, UI_HIGHLIGHT );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH, rect.height - BORDER_WIDTH, UI_SHADOW );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH * 2, rect.height - BORDER_WIDTH * 2, UI_BACK );
}