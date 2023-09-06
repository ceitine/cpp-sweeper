#include "raylib.h"
#include "grid.cpp"
#include <string>

// Most likely don't need any more than 30 frames per second.
static const int TARGET_FRAMERATE = 30;

Grid grid;

int main()
{
	InitWindow( 800, 450, "C-Sweeper" );
	
	grid = Grid::Grid( Difficulty::Beginner );

	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		
		DrawText( "nothing to see here, yet..", 50, 50, 20, LIGHTGRAY );
		DrawFPS( 10, 10 );

		grid.render();

		EndDrawing();

		// Delay to aim for target framerate.
		WaitTime( 1.0 / TARGET_FRAMERATE );
	}

	CloseWindow();

	return 0;
}