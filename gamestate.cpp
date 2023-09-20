#include "gamestate.h"
#include "constants.h"
#include "gui.h"
#include "difficulty.h"
#include "field.cpp"

static const int HEADER_HEIGHT = 35;

static GameState state = GameState::Menu;
static Difficulty difficulty = Difficulty::Intermediate;
static Vec2I window_size;

static Field* field = nullptr;
static float timer;
static bool started;

static Dropdown<Difficulty>* difficulties = new Dropdown<Difficulty> 
{
	new Difficulty[3]
	{
		Difficulty::Beginner,
		Difficulty::Intermediate,
		Difficulty::Expert
	}
};

void set_state( GameState to )
{
	state = to;
	started = false;
	timer = 0;

	// Create field and set window size according to difficulty.
	if ( to == GameState::Game )
	{
		field = new Field( difficulty );

		Vec2I size = getWindowSize( difficulty );
		window_size = Vec2I { size.x, size.y + HEADER_HEIGHT };
	}

	// Delete field and reset size to default.
	else if ( to == GameState::Menu )
	{
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
	DrawTextureEx( logoTex, {20, 20}, 0, logoScale, WHITE );

	float width = 200;
	float height = 35;

	render_button( "Start", { 40, 150, width, height }, [] { set_state( GameState::Game ); } );
	render_dropdown<Difficulty>( { 40, 160 + height, width, height }, difficulties );
}

void render_game()
{
	// If field doesn't exist, we shouldn't do anything here.
	if ( field == nullptr )
		return;

	// Field
	float padding = 10;
	float w = (window_size.x - padding * 2) / field->size.x;
	float h = (window_size.y - padding * 2) / field->size.y;
	float size = w > h ? h : w;

	Vector2 fieldSize = { field->size.x * size, field->size.y * size };
	Vector2 fieldPos = { (int)padding, (int)(window_size.y - fieldSize.y - padding) };
	DrawRectangle( fieldPos.x - 2, fieldPos.y - 2, fieldSize.x + 4, fieldSize.y + 4, GRAY );
	field->render( fieldPos, size );

	// Header
	float height = HEADER_HEIGHT - 5;
	draw_background( { padding, 5, window_size.x - height - padding - 20, height + 3 } );
	render_button( "X", { (float)window_size.x - height - padding, 5, height, height }, [] { set_state( GameState::Menu ); } );
	
	int minutes = (int)(timer / 60);
	int seconds = (int)timer - minutes * 60;
	const char* formatted = TextFormat( "%s%d:%s%d", minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds );
	draw_string( "TIME:", { 15, 8 }, PINK, 25 );
	draw_string( formatted, { 83, 8 }, WHITE, 25);

	draw_string( "SCORE:", { 160, 8 }, PINK, 25 );
	draw_string( TextFormat( "%d", field->revealed ), {245, 8}, WHITE, 25);

	// Lets handle our input here if we are actually in game.
	if ( state != GameState::Game )
		return;

	// Add to our timer.
	if ( started )
	{
		float dt = GetFrameTime();
		timer += dt;
	}

	Vec2I grid = {
		floor( (GetMouseX() - fieldPos.x) / size ),
		floor( (GetMouseY() - fieldPos.y) / size )
	};

	// Check if we are within our field bounds.
	if ( grid.x < 0 || grid.x >= field->size.x
	  || grid.y < 0 || grid.y >= field->size.y ) return;

	if ( IsMouseButtonReleased( MOUSE_BUTTON_LEFT ) )
	{
		started = true;
		field->reveal( grid.x, grid.y );
	}
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
