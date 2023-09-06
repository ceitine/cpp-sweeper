#include "raylib.h"
#include "difficulty.cpp"
#include <stdlib.h>
#include <vector>
#include <iostream>

enum TileType
{
	Empty,
	Flag,
	Bomb
}; 

struct Tile
{
public:
	bool revealed;
	TileType type;

	Tile() 
	{
		this->type = Empty;
		this->revealed = false;
	}

	Tile( TileType type ) : Tile()
	{
		this->type = type;
	}

	void setType( TileType type )
	{
		this->type = type;
	}
};

class Grid
{
public:
	Grid()
	{
		this->bombsLeft = 0;
		this->difficulty = Difficulty::Beginner;
	}

	Grid( Difficulty difficulty ) 
	{
		this->size = getGridSize( difficulty );
		this->difficulty = difficulty;
		this->tiles = std::vector<Tile>( size.x * size.y );

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
			Tile tile = tiles[index];

			switch ( tile.type )
			{
			case TileType::Empty:
				DrawRectangle( x * 50, y * 50, 48, 48, LIGHTGRAY );
				break;
			case TileType::Bomb:
				DrawRectangle( x * 50, y * 50, 48, 48, RED );
				break;
			case TileType::Flag:
				DrawRectangle( x * 50, y * 50, 48, 48, GREEN );
				break;
			}
		}
	}

	// todo: implement revealing
	int reveal( int x, int y )
	{

	}

private:
	std::vector<Tile> tiles;
	Vec2I size;
	Difficulty difficulty;
	int bombsLeft;

	void generate()
	{
		int bombs = getBombCount( this->difficulty );
		bombsLeft = bombs;

		// Set the other grids to empty.
		for ( int x = 0; x < size.x; x++ )
		for ( int y = 0; y < size.y; y++ )
		{
			int index = getIndex( x, y );
			tiles[index] = Tile{ Empty };
		}

		// Fill grid with bombs.
		while ( bombs > 0 )
		{
			int index = getIndex( rand() % size.x, rand() % size.y );
			if ( tiles[index].type == TileType::Bomb )
				continue;

			tiles[index].type = Bomb;
			bombs--;
		}
	}

	int getIndex( int x, int y )
	{
		return x + y * (size.x - 1);
	}
};