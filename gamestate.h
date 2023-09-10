#pragma once
enum GameState {
	Menu,
	Game,
	Win,
	Loss
};

void set_state( GameState to );
void render_state();