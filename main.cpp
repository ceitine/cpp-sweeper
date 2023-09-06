#include "raylib.h"
#include "grid.cpp"
#include <string>

// Most likely don't need any more than 30 frames per second.
static const int TARGET_FRAMERATE = 30;

Grid grid;

int main()
{
	// Initialize window.
	InitWindow( 800, 450, "cpp-sweeper" );
	SetTargetFPS( TARGET_FRAMERATE );

	grid = Grid::Grid( Difficulty::Intermediate );
	grid.reveal( 5, 5 );

	// Do rendering and updates.
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		grid.render();
		DrawFPS( 10, 10 );
		EndDrawing();
	}

	CloseWindow();

	return 0;
}