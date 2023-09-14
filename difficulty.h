#pragma once
#include "vec2i.h"

enum Difficulty
{
	Beginner,
	Intermediate,
	Expert
};

// These values are taken from a random google search result. lol
static int getBombCount( Difficulty difficulty )
{
	switch ( difficulty )
	{
	case Difficulty::Beginner:
		return 10;
		break;

	case Difficulty::Intermediate:
		return 40;
		break;

	case Difficulty::Expert:
		return 99;
		break;

	default:
		throw "Incorrect difficulty.";
		break;
	}
}

static Vec2I getGridSize( Difficulty difficulty )
{
	switch ( difficulty )
	{
	case Difficulty::Beginner:
		return { 8, 8 };
		break;

	case Difficulty::Intermediate:
		return { 16, 16 };
		break;

	case Difficulty::Expert:
		return { 30, 16 };
		break;

	default:
		throw "Incorrect difficulty.";
		break;
	}
}

static Vec2I getWindowSize( Difficulty difficulty )
{
	switch ( difficulty )
	{
	case Difficulty::Beginner:
		return { 500, 500 };
		break;

	case Difficulty::Intermediate:
		return { 700, 700 };
		break;

	case Difficulty::Expert:
		return { 900, 600 };
		break;

	default:
		throw "Incorrect difficulty.";
		break;
	}
}