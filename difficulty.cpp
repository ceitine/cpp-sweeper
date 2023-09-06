#include "structs.cpp"

enum Difficulty
{
	Beginner = 0,
	Intermediate,
	Expert
};


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