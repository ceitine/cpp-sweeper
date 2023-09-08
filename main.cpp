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
	SetTargetFPS( TARGET_FRAMERATE );
	
	field = new Field( Difficulty::Intermediate );
	field->reveal( 5, 5 );

	// Do rendering and updates.
	while ( !WindowShouldClose() )
	{
		BeginDrawing();
		ClearBackground( RAYWHITE );
		Vec2I size = field->render( 605, 600 );
		DrawFPS( 10, 10 );
		EndDrawing();
	}

	CloseWindow();

	return 0;
}