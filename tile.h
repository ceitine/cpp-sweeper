#pragma once
#include <cstdint>

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
