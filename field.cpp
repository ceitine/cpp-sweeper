#include <stdlib.h>
#include <vector>
#include <iostream>
#include "raylib.h"
#include "tile.cpp"
#include "difficulty.cpp"
#include <string>

static const Color colors[8] =
{
	BLUE,
	GREEN,
	RED,
	DARKBLUE,
	BROWN,
	SKYBLUE,
	BLACK,
	GRAY
};

class Field
{
public:
	int revealed;

	Field( Difficulty difficulty )
	{
		this->size = getGridSize( difficulty );
		this->difficulty = difficulty;
		this->tiles = std::vector<Tile*>( size.x * size.y );

		gridTex = LoadTexture( "assets/grid.png" );
		emptyTex = LoadTexture( "assets/grid_empty.png" );
		flagTex = LoadTexture( "assets/flag.png" );
		mineTex = LoadTexture( "assets/mine.png" );

		SetTextureFilter( gridTex, TEXTURE_FILTER_POINT );
		SetTextureFilter( emptyTex, TEXTURE_FILTER_POINT );
		SetTextureFilter( flagTex, TEXTURE_FILTER_POINT );
		SetTextureFilter( mineTex, TEXTURE_FILTER_POINT );

		generate();
	}
	
	void discard()
	{
		for ( Tile* tile : this->tiles )
			delete tile;

		delete this;
	}

	Vec2I render( int width, int height )
	{
		int w = width / this->size.x;
		int h = height / this->size.y;
		float size = w > h ? h : w;

		float bombScale = size / 16 * 0.75;

		float x = 0;
		float y = 0;

		// Go through all tiles.
		for ( int j = 0; j < this->size.y; j++ )
		{
			x = 0;

			for ( int i = 0; i < this->size.x; i++ )
			{
				int index = getIndex( i, j );
				Tile* tile = tiles[index];
				if ( tile == nullptr )
					continue;

				Vector2 pos = Vector2{ x, y };

				// If we haven't revealed tile, we should just draw it as "hidden".
				if ( !tile->revealed )
				{
					DrawTextureEx( gridTex, pos, 0, size / 16, WHITE );

					if ( tile->type == Mine )
					{
						DrawTextureEx( mineTex, Vector2{ x + size / 8, y + size / 8 }, 0, size / 16 * 0.75f, WHITE );
					}

					// Set position.
					x = i * size;
					y = j * size;

					continue;
				}

				// If we are revealed, everything should draw differently.
				switch ( tile->type )
				{
				case TileType::Empty:
					DrawTextureEx( emptyTex, pos, 0, size / 16, WHITE );

					if ( tile->value > 0 )
					{
						std::string text = std::to_string( tile->value );

						DrawText( text.c_str(), x + 6, y + 6, w / 2, BLACK );
						DrawText( text.c_str(), x + 5, y + 5, w / 2, colors[tile->value] );
					}

					break;

				case TileType::Mine:
					DrawTextureEx( mineTex, Vector2{ x + size / 8, y + size / 8 }, 0, size / 16 * 0.75f, WHITE );

					break;

				case TileType::Flag:
					DrawTextureEx( flagTex, pos, 0, size / 16, WHITE );

					break;
				}

				// Increment on x-axis.
				x += size;
			}

			// Increment on y-axis.
			y += size;
		}

		return { (int)x, (int)y };
	}

	void reveal( int x, int y )
	{
		// Check if we are out of bounds.
		if ( x >= size.x || y >= size.y
		  || x < 0 || y < 0 ) return;

		// Ignore if we have a revealed tile or a flag.
		int index = getIndex( x, y );
		if ( index >= size.x * size.y )
			return;

		Tile* tile = tiles[index];
		if ( tile->revealed || tile->type == Flag )
			return;

		if ( tile->type == Mine )
		{
			// todo: game over screen
			return;
		}

		// Reveal the tile also up the count.
		tile->revealed = true;
		this->revealed++;

		// Check if we have revealed everything we can.
		if ( this->revealed == this->tiles.size() - bombCount )
		{
			// todo: show win screen.
			return;
		}

		// Reveal nearby tiles with no mines.
		int mines = calculateMines( x, y );
		if ( mines == 0 )
		{
			for ( int i = x - 1; i <= x + 1; i++ )
			for ( int j = y - 1; j <= y + 1; j++ )
				reveal( i, j );

			return;
		}

		tile->value = mines;
	}

private:
	std::vector<Tile*> tiles;
	Vec2I size;
	Difficulty difficulty;
	int bombCount;

	Texture gridTex;
	Texture emptyTex;
	Texture flagTex;
	Texture mineTex;

	int calculateMines( int x, int y )
	{
		int count = 0;

		// Go through all neighboring mines in a 9x9 grid.
		for ( int i = x - 1; i < x + 2; i++ )
		for ( int j = y - 1; j < y + 2; j++ )
		{
			if ( i >= size.x || j >= size.y
			  || i < 0 || j < 0 ) continue;

			int index = getIndex( i, j );
			Tile* tile = tiles[index];

			if ( tile->type == Mine )
				count++;
		}

		return count;
	}

	void generate()
	{
		int bombs = getBombCount( this->difficulty );
		bombCount = bombs;

		// Fill with empty grids.
		for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			tiles[getIndex( x, y )] = new Tile{ Empty };

		// Fill grid with bombs.
		while ( bombs > 0 )
		{
			int index = getIndex( rand() % size.x, rand() % size.y );
			Tile* tile = tiles[index];
			if ( tile->type == TileType::Mine )
				continue;

			tile->type = Mine;
			bombs--;
		}
	}

	int getIndex( int x, int y )
	{
		return x + y * size.x;
	}
};