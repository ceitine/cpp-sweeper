#include "raylib.h"
#include "field.cpp"
#include <string>

// Most likely don't need any more than 30 frames per second.
static const int TARGET_FRAMERATE = 30;

Field* field = nullptr;

int main()
{
	// Initialize window.
	InitWindow( 800, 600, "cpp-sweeper" );
	SetWindowIcon( LoadImage( "assets/mine.png" ) );
	SetTargetFPS( TARGET_FRAMERATE );

	// Load the textures.
	init();
	
	// Initilize a field.
	field = new Field( Difficulty::Intermediate );
	field->reveal( 5, 5 );

	// Do rendering and updates.
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		if ( IsKeyDown( KEY_ENTER ) )
		{
			delete field;
			field = new Field( Difficulty::Intermediate );
			field->reveal( 5, 5 );
		}

		Vec2I size = field->render( 605, 600 );
		DrawFPS( 10, 10 );
		EndDrawing();
	}

	CloseWindow();
	delete field;

	return 0;
}