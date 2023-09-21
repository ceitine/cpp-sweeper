#pragma warning( disable : 4244 ) 
#include "gui.h"
#include <iostream>

const Color UI_HIGHLIGHT = { 220, 220, 220, 255 };
const Color UI_SHADOW = { 145, 145, 145, 255 };
const Color UI_BACK = { 190, 190, 190, 255 };
const int BORDER_WIDTH = 3;

Texture2D gridTex;
Texture2D emptyTex;
Texture2D flagTex;
Texture2D mineTex;
Texture2D logoTex;
Texture2D selectionTex;
Texture2D arrowTex;

Font myFont;

void init_gui()
{
	gridTex = LoadTexture( "assets/grid.png" );
	emptyTex = LoadTexture( "assets/grid_empty.png" );
	flagTex = LoadTexture( "assets/flag.png" );
	mineTex = LoadTexture( "assets/mine.png" );
	logoTex = LoadTexture( "assets/logo.png" );
	selectionTex = LoadTexture( "assets/selection.png" );
	arrowTex = LoadTexture( "assets/arrow.png" );
															  
	SetTextureFilter( gridTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( emptyTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( flagTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( mineTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( logoTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( selectionTex, TEXTURE_FILTER_POINT );
	SetTextureFilter( arrowTex, TEXTURE_FILTER_POINT );

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
	DrawTextEx( myFont, text, { 1 + pos.x,  1 + pos.y }, fontSize, 0, { 0, 0, 0, 150 } );
	DrawTextEx( myFont, text, pos, fontSize, 0, color );
}

bool mouse_in_rect( int x, int y, int w, int h )
{
	Vector2 pos = GetMousePosition();
	return pos.x >= x && pos.x < x + w
		&& pos.y >= y && pos.y < y + h;
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
		{
			onclick();
			return;
		}
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

void render_dropdown( Rectangle rect, Dropdown* dropdown, int fontSize )
{
	if ( dropdown == nullptr )
		return;

	float padding = 2;
	float optionHeight = dropdown->options.size() * (fontSize + padding * 2);

	// Handle input.
	bool hovered = mouse_in_rect( rect.x, rect.y, rect.width, rect.height + (dropdown->down ? optionHeight : 0) );
	if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) )
		dropdown->down = hovered;
	
	DrawRectangle( rect.x, rect.y + rect.height, rect.width, BORDER_WIDTH, UI_SHADOW );
	DrawRectangle( rect.x, rect.y, rect.width, rect.height, UI_HIGHLIGHT );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH * 2, rect.height - BORDER_WIDTH * 2, UI_BACK );

	float size = 16;
	float scale = (rect.height / 35);
	float gap = rect.height / 2 + scale * size / 2;
	DrawTextureEx( arrowTex, { rect.x + rect.width - gap, rect.y + size * scale / 2 + BORDER_WIDTH / 2 }, 0, scale, WHITE );

	draw_string( dropdown->options[dropdown->option].c_str(), { rect.x + 5, rect.y + rect.height / 2 - fontSize / 2 }, RAYWHITE, fontSize );

	// Should we display the options?
	if ( dropdown->down )
	{
		float offset = rect.height;
		int i = 0;

		draw_background( { rect.x, rect.y + offset - BORDER_WIDTH, rect.width, optionHeight + BORDER_WIDTH * 2 } );

		for ( std::string option : dropdown->options )
		{
			Vector2 pos = { rect.x, rect.y + offset };
			bool hovered = mouse_in_rect( pos.x, pos.y, rect.width, fontSize + padding * 2 );
			Color col = { 215, 215, 215, 255 };

			if ( hovered )
			{
				DrawRectangle( pos.x, pos.y, rect.width, fontSize + padding * 2, { 255, 255, 255, 100 } );
				col = RAYWHITE;

				if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) )
				{
					dropdown->option = i;
					dropdown->down = false;
				}
			}

			draw_string( option.c_str(), { pos.x + 5, pos.y }, col, fontSize );
			
			offset += fontSize + padding * 2;
			i++;
		}
	}

	if ( hovered )
		draw_select( rect );
}

void draw_background( Rectangle rect )
{
	DrawRectangle( rect.x, rect.y, rect.width, rect.height, UI_HIGHLIGHT );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH, rect.height - BORDER_WIDTH, UI_SHADOW );
	DrawRectangle( rect.x + BORDER_WIDTH, rect.y + BORDER_WIDTH, rect.width - BORDER_WIDTH * 2, rect.height - BORDER_WIDTH * 2, UI_BACK );
}