#include "gamestate.h"
#include "constants.h"
#include "gui.h"
#include "difficulty.h"
#include "field.cpp"

static GameState state = GameState::Menu;
static Difficulty difficulty = Difficulty::Intermediate;
static Vec2I window_size;

static std::string splashes[] =
{
	"probably the worst minesweeper",
	"don't click on the mines",
	"test"
};

static std::string splash;
static Field* field = nullptr;

void set_state( GameState to )
{
	state = to;

	// Create field and set window size according to difficulty.
	if ( to == GameState::Game )
	{
		field = new Field( difficulty );
		window_size = getWindowSize( difficulty );
	}

	// Delete field and reset size to default.
	else if ( to == GameState::Menu )
	{
		splash = splashes[rand() % (sizeof( splashes ) / sizeof( splashes[0] ))];
		window_size = { WINDOW_WIDTH, WINDOW_HEIGHT };

		if ( field != nullptr )
			delete field;
	}
	else
		return;

	// Actually assign window size.
	SetWindowSize( window_size.x, window_size.y );
}

void render_menu()
{
	float logoScale = 3;
	DrawTextureEx( logoTex, {0, 0}, 0, logoScale, WHITE );
	draw_string( splash.c_str(), {150, 70}, YELLOW, 20 );

	float width = 200;
	float height = 30;

	render_button( "START GAME", Rectangle{ 50, 250, width, height }, [] { set_state( GameState::Game ); } );
}

void render_game()
{
	// If field doesn't exist, we shouldn't do anything here.
	if ( field == nullptr )
		return;

	float padding = 10;
	float w = (window_size.x - padding * 2) / field->size.x;
	float h = (window_size.y - padding * 2) / field->size.y;
	float size = w > h ? h : w;

	Vector2 fieldSize = { field->size.x * size, field->size.y * size };
	Vector2 fieldPos = { (int)padding, (int)(window_size.y - fieldSize.y - padding) };
	DrawRectangle( fieldPos.x - 2, fieldPos.y - 2, fieldSize.x + 4, fieldSize.y + 4, BLACK );
	field->render( fieldPos, size );

	// Lets handle our input here if we are actually in game.
	if ( state != GameState::Game )
		return;

	Vec2I grid = {
		floor( (GetMouseX() - fieldPos.x) / size ),
		floor( (GetMouseY() - fieldPos.y) / size )
	};

	// Check if we are within our field bounds.
	if ( grid.x < 0 || grid.x >= field->size.x
	  || grid.y < 0 || grid.y >= field->size.y ) return;

	draw_string( TextFormat( "%i, %i", grid.x, grid.y ), { 10, 10 }, RED, 25 );

	if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) )
		field->reveal( grid.x, grid.y );
	else if ( IsMouseButtonReleased( MOUSE_BUTTON_RIGHT ) )
		field->toggle_flag( grid.x, grid.y );
}

void render_end( bool win = false )
{
	if ( state != GameState::Menu )
		set_state( GameState::Menu );

	// Shared rendering here...
	render_game();
	DrawRectangle( 0, 0, window_size.x, window_size.y, { 0, 0, 0, 150 } );

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
