#pragma once
enum GameState {
	Menu,
	Game,
	Win,
	Loss
};

extern bool closing;

void set_state( GameState to );
void render_state();