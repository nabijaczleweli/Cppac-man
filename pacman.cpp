// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "pacman.hpp"

using namespace std;

const constexpr static chtype pacman_look_up    = 'v';
const constexpr static chtype pacman_look_down  = '\357';
const constexpr static chtype pacman_look_left  = '>';
const constexpr static chtype pacman_look_right = '<';

static void register_natives() noexcept {
	static bool registered_natives = false;
	if(registered_natives)
		return;
	registered_natives = true;
	init_pair(1, COLOR_YELLOW, COLOR_BLACK);
}

pacman::pacman() : pacman::pacman(make_pair(-1, -1), NULL) {};
pacman::pacman(const pair<unsigned int, unsigned int> & start_pos, WINDOW * scr) : paintable::paintable(scr), starting_position(new unsigned int[2]), current_position(new unsigned int[2]) {
	starting_position[0] = start_pos.first;
	starting_position[1] = start_pos.second;
	reset();
	register_natives();
}

pacman & pacman::operator=(const pacman & pac) {
	screen = pac.screen;

	current_position[0]  = pac.current_position[0];
	current_position[1]  = pac.current_position[1];

	starting_position[0] = pac.starting_position[0];
	starting_position[1] = pac.starting_position[1];

	return *this;
}
pacman::~pacman() {
	screen = NULL;
	delete[] current_position;
	delete[] starting_position;
}

pair<unsigned int, unsigned int> pacman::beginning_position() {
	return make_pair(starting_position[0], starting_position[1]);
}
pair<unsigned int, unsigned int> pacman::actual_position() {
	return make_pair(current_position[0], current_position[1]);
}
void pacman::paint() {
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	move(current_position[1], current_position[0]);
	switch(looking) {
		case direction::right :
			addch(pacman_look_right);
			break;
		case direction::left  :
			addch(pacman_look_left);
			break;
		case direction::up    :
			addch(pacman_look_up);
			break;
		case direction::down  :
			addch(pacman_look_down);
			break;
	}
	attron(A_BOLD);
	attroff(COLOR_PAIR(1));
}
void pacman::reset() noexcept {
	current_position[0] = starting_position[0];
	current_position[1] = starting_position[1];
}