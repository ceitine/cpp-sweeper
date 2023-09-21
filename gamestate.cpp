#pragma warning( disable : 4244 ) 
#pragma warning( disable : 4838 ) 

#include "gamestate.h"
#include "constants.h"
#include "gui.h"
#include "difficulty.h"
#include "field.cpp"

static const int HEADER_HEIGHT = 35;

static GameState state = GameState::Menu;
static Vec2I window_size;

static Field* field = nullptr;
static float timer;
static bool started;

bool closing = false;

Dropdown* difficulties = new Dropdown 
{
	std::vector<std::string>( 
	{
		"Beginner",
		"Intermediate",
		"Expert"
	} )
};

void set_state( GameState to )
{
	state = to;
	started = false;

	// Create field and set window size according to difficulty.
	if ( to == GameState::Game )
	{
		timer = 0;

		Difficulty difficulty = (Difficulty)difficulties->option;
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
	else if ( to == GameState::Win || to == GameState::Loss )
	{
		for ( int y = 0; y < field->size.y; y++ )
		for ( int x = 0; x < field->size.x; x++ )
			field->reveal( x, y, true );

		return;
	}

	// Actually assign window size.
	SetWindowSize( window_size.x, window_size.y );
}

void request_close()
{
	closing = true;
}

void render_menu()
{
	float logoScale = 3;
	DrawTextureEx( logoTex, {20, 20}, 0, logoScale, WHITE );

	float width = 200;
	float height = 35;

	render_button( "Start", { 40, 150, width, height }, [] { set_state( GameState::Game ); } );
	render_dropdown( { 40, 160 + height, width, height }, difficulties );
	render_button( "Exit", { 40, window_size.y - height - 40, 100, height }, request_close );
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
	
	if ( state == GameState::Game )
	{
		int minutes = (int)(timer / 60);
		int seconds = (int)timer - minutes * 60;
		const char* formatted = TextFormat( "%s%d:%s%d", minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds );
		draw_string( "TIME:", { 15, 8 }, PINK, 25 );
		draw_string( formatted, { 83, 8 }, WHITE, 25 );

		draw_string( "SCORE:", { 160, 8 }, PINK, 25 );
		draw_string( TextFormat( "%d", field->revealed ), { 245, 8 }, WHITE, 25 );
	}

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
	render_game();
	DrawRectangle( 0, 0, window_size.x, window_size.y, { 0, 0, 0, 150 } );

	Color col = win
		? GREEN
		: RED;

	const char* text = win
		? "YOU WIN!"
		: "YOU LOSE!";

	// Show results
	float height = 35;

	int fontSize = 50;
	Vector2 size = MeasureTextEx( myFont, text, fontSize, 0 );
	float startY = window_size.y / 2 - (size.y + height * 2 + 40) / 2;
	draw_string( text, { window_size.x / 2 - size.x / 2, startY }, col, fontSize );

	int minutes = (int)(timer / 60);
	int seconds = (int)timer - minutes * 60;
	const char* formatted = TextFormat( "%s%d:%s%d", minutes < 10 ? "0" : "", minutes, seconds < 10 ? "0" : "", seconds );
	draw_string( "TIME:", { 15, 8 }, PINK, 25 );
	draw_string( formatted, { 83, 8 }, WHITE, 25 );

	draw_string( "SCORE:", { 160, 8 }, PINK, 25 );
	draw_string( TextFormat( "%d", field->revealed ), { 245, 8 }, WHITE, 25 );

	// Buttons
	render_button( "Retry", { window_size.x / 2 - 150.0f / 2, startY + size.y + 5, 150, height }, [] { set_state( GameState::Game ); } );
	render_button( "Menu", { window_size.x / 2 - 100.0f / 2, startY + size.y + 20 + height, 100, height }, [] { set_state( GameState::Menu ); } );
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
