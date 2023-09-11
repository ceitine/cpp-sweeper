#pragma once
#include <cstdint>

enum TileState
{
	None,
	Flagged
};

struct Tile
{
public:
	bool revealed;
	bool isMine;
	uint8_t value;
	TileState state;

	Tile()
	{
		this->state = None;
		this->revealed = false;
		this->isMine = false;
		this->value = 0;
	}
};
