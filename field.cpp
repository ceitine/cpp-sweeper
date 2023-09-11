#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include "raylib.h"
#include "tile.h"
#include "gui.h"
#include "difficulty.h"
#include "gamestate.h"

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
	Vec2I size;

	Field( Difficulty difficulty )
	{
		this->size = getGridSize( difficulty );
		this->difficulty = difficulty;
		this->tiles = std::vector<Tile*>( size.x * size.y );

		generate();
	}
	
	~Field()
	{
		// Free tile pointers from our tile vector.
		for ( Tile* tile : this->tiles )
			delete tile;
	}

	void render( Vector2 pos, float size )
	{
		float bombScale = size / 16 * 0.75;

		float x = pos.x;
		float y = pos.y;

		// Go through all tiles.
		for ( int j = 0; j < this->size.y; j++ )
		{
			x = pos.x;

			for ( int i = 0; i < this->size.x; i++ )
			{
				int index = getIndex( i, j );
				Tile* tile = tiles[index];
				if ( tile == nullptr )
					continue;

				// Get position.
				x = pos.x + i * size;
				y = pos.y + j * size;
				Vector2 position = Vector2{ x, y };

				// If we haven't revealed tile, we should just draw it as "hidden".
				if ( !tile->revealed )
				{
					DrawTextureEx( gridTex, position, 0, size / 16, WHITE );

					if ( tile->state == TileState::Flagged )
						DrawTextureEx( flagTex, position, 0, size / 16, WHITE );

					continue;
				}

				// If we are revealed, everything should draw differently.
				if ( !tile->isMine )
				{
					DrawTextureEx( emptyTex, position, 0, size / 16, WHITE );

					if ( tile->value > 0 )
					{
						float scale = size / 1.5f;
						std::string text = std::to_string( tile->value );
						Vector2 strSize = MeasureTextEx( myFont, text.c_str(), scale, 0 );

						draw_string( text.c_str(), Vector2{ x + size / 2 - strSize.x / 2, y + size / 2 - strSize.y / 2 }, colors[tile->value], scale );
					}

					continue;
				}

				// Draw mines when revealed.
				DrawTextureEx( mineTex, Vector2{ x + size / 8, y + size / 8 }, 0, size / 16 * 0.75f, WHITE );
			}
		}
	}

	void toggle_flag( int x, int y )
	{
		// Check if we are out of bounds.
		if ( x >= size.x || y >= size.y
		  || x < 0 || y < 0 ) return;

		// Ignore if we have a revealed tile or a flag.
		int index = getIndex( x, y );
		if ( index >= size.x * size.y )
			return;

		Tile* tile = tiles[index];
		if ( tile->revealed )
			return;

		// Toggle the flag.
		tile->state = tile->state == TileState::Flagged
			? TileState::None
			: TileState::Flagged;
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
		if ( tile->revealed || tile->state == TileState::Flagged )
			return;

		// Whoops.. We lost.
		if ( tile->isMine )
		{
			set_state( GameState::Loss );
			return;
		}

		// Reveal the tile also up the count.
		tile->revealed = true;
		this->revealed++;

		// Check if we have revealed everything we can.
		if ( this->revealed == this->tiles.size() - bombCount )
		{
			set_state( GameState::Win );
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
	Difficulty difficulty;
	int bombCount;

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

			if ( tile->isMine )
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
			tiles[getIndex( x, y )] = new Tile();

		// Fill grid with bombs.
		while ( bombs > 0 )
		{
			int index = getIndex( rand() % size.x, rand() % size.y );
			Tile* tile = tiles[index];
			if ( tile->isMine )
				continue;

			tile->isMine = true;
			bombs--;
		}
	}

	int getIndex( int x, int y )
	{
		return x + y * size.x;
	}
};