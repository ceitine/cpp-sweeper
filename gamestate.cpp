#include "gamestate.h"
#include "constants.h"
#include "gui.h"
#include "difficulty.h"
#include "field.cpp"

static GameState state = GameState::Menu;
static Difficulty difficulty = Difficulty::Intermediate;
static Vec2I size;

static Field* field = nullptr;

void set_state( GameState to )
{
	state = to;

	// Create field and set window size according to difficulty.
	if ( to == GameState::Game )
	{
		field = new Field( difficulty );
		size = getWindowSize( difficulty );
	}

	// Delete field and rset size to default.
	else if ( to == GameState::Menu )
	{
		size = { WINDOW_WIDTH, WINDOW_HEIGHT };
		delete field;
	}
	else
		return;

	// Actually assign window size.
	SetWindowSize( size.x, size.y );
}
	

void render_menu()
{
	DrawTexture( logoTex, 0, 0, WHITE );

	float width = 200;
	float height = 20;

	render_button( "START GAME", Rectangle{ 0, 0, width, height }, [] { set_state( GameState::Game ); } );
}

void render_game()
{
	if ( field == nullptr )
		return;

	field->render( size.x, size.y );
}

void render_end( bool win = false )
{
	// Shared rendering here...

	// Draw win screen.
	if ( win )
	{
		return;
	}

	// Draw loss screen.
}

void render_state()
{
	// Render depending on actual game state.
	switch ( state )
	{
	case GameState::Menu:
		render_menu();
		break;

	case GameState::Game:
		render_game();
		break;

	case GameState::Win:
		render_end( true );
		break;

	case GameState::Loss:
		render_end();
		break;
	}
}