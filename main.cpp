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

	// Initialize everything.
	init_gui();
	set_state( GameState::Menu );
	
	// Do rendering and updates.
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( { 180, 180, 180 } );
		render_state();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}