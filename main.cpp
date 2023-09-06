#include "raylib.h";

// Most likely don't need any more than 30 frames per second.
static const int TARGET_FRAMERATE = 30;

int main()
{
	InitWindow( 800, 450, "C-Sweeper" );
		
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		DrawText( "nothing to see here, yet..", 50, 50, 20, LIGHTGRAY );

		DrawFPS( 10, 10 );
		EndDrawing();

		// Delay to aim for target framerate.
		WaitTime( 1.0 / TARGET_FRAMERATE );
	}

	CloseWindow();

	return 0;
}