#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include "raylib.h"
#include "difficulty.cpp"

enum TileType
{
	Empty,
	Flag,
	Mine
}; 

struct Tile
{
public:
	bool revealed;
	uint8_t value;
	TileType type;

	Tile() 
	{
		this->type = Empty;
		this->revealed = false;
		this->value = 0;
	}

	Tile( TileType type ) : Tile()
	{
		this->type = type;
	}
};

class Grid
{
public:
	int revealed;

	Grid()
	{
		this->size = { 0,0 };
		this->bombCount = 0;
		this->difficulty = Difficulty::Beginner;
		this->revealed = 0;
	}

	Grid( Difficulty difficulty ) 
	{
		this->size = getGridSize( difficulty );
		this->difficulty = difficulty;
		this->tiles = std::vector<Tile*>( size.x * size.y );

		generate();
	}
	
	~Grid()
	{
		
	}

	void render()
	{
		for ( int x = 0; x < size.x; x++ )
		for ( int y = 0; y < size.y; y++ )
		{
			int index = getIndex( x, y );
			Tile* tile = tiles[index];

			if ( !tile->revealed )
			{
				DrawRectangle( x * 50, y * 50, 48, 48, DARKGRAY );

				if ( tile->type == Mine )
				{
					DrawRectangle( x * 50 + 4, y * 50 + 4, 40, 40, RED );
				}
				continue;
			}

			switch ( tile->type )
			{
			case TileType::Empty:
				DrawRectangle( x * 50, y * 50, 48, 48, LIGHTGRAY );

				if ( tile->value > 0 )
				{
					DrawText( std::to_string( tile->value ).c_str(), x * 50, y * 50, 20, BLUE );
				}

				break;

			case TileType::Mine:
				DrawRectangle( x * 50, y * 50, 48, 48, RED );
				break;

			case TileType::Flag:
				DrawRectangle( x * 50, y * 50, 48, 48, GREEN );
				break;
			}
		}
	}

	int calculateMines( int x, int y )
	{
		int count = 0;
	
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

	// todo: implement revealing
	void reveal( int x, int y )
	{
		// Check if we are out of bounds.
		if ( x >= size.x || y >= size.y
		  || x < 0 || y < 0 ) return;

		// Ignore if we have a revealed tile or a flag.
		int index = getIndex( x, y );
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
			for ( int i = x - 1; i < x + 2; i++ )
			for ( int j = y - 1; j < y + 2; j++ )
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

	void generate()
	{
		int bombs = getBombCount( this->difficulty );
		bombCount = bombs;

		// Set the other grids to empty.
		for ( int x = 0; x < size.x; x++ )
		for ( int y = 0; y < size.y; y++ )
		{
			int index = getIndex( x, y );
			tiles[index] = new Tile{ Empty };
		}

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

		// Change window information.
		SetWindowSize( size.x * 50, size.y * 50 );
	}

	int getIndex( int x, int y )
	{
		return x + y * size.x;
	}
};