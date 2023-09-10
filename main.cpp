#include "raylib.h"
#include "gui.h"
#include "gamestate.h"
#include "constants.h"

int main()
{
	// Initialize window.
	InitWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Minesweeper" );
	SetWindowIcon( LoadImage( "assets/mine.png" ) );
	SetTargetFPS( TARGET_FRAMERATE );

	// Load the textures.
	init_gui();
	
	// Do rendering and updates.
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		render_state();
		DrawFPS( 10, 10 );
		EndDrawing();
	}

	CloseWindow();

	return 0;
}