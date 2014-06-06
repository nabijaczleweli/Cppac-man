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

#include "ghost.hpp"

using namespace std;

ghost::ghost() : ghost::ghost(static_cast<attr_t>(-1), make_pair(-1, -1)) {}
ghost::ghost(attr_t color, pair<int, int> start_pos) : ghost::ghost(color, start_pos, stdscr) {}
ghost::ghost(attr_t newcolor, pair<int, int> start_pos, WINDOW * screen) : paintable(screen), start_pos_x(start_pos.first), start_pos_y(start_pos.second), color(newcolor) {
	reset();
}
ghost::ghost(ghost && gho) : paintable(paintable::screen), start_pos_x(gho.start_pos_x), start_pos_y(gho.start_pos_y), cur_pos_x(gho.cur_pos_x), cur_pos_y(gho.cur_pos_y), color(gho.color) {
	gho.screen      = NULL;
	gho.start_pos_x = -1;
	gho.start_pos_y = -1;
	gho.cur_pos_x   = -1;
	gho.cur_pos_y   = -1;
	gho.color       = -1;
}
ghost::ghost(const ghost & gho) : paintable(paintable::screen), start_pos_x(gho.start_pos_x), start_pos_y(gho.start_pos_y), cur_pos_x(gho.cur_pos_x), cur_pos_y(gho.cur_pos_y), color(gho.color) {}

ghost & ghost::operator=(const ghost & gho) {
	start_pos_x = gho.start_pos_x;
	start_pos_y = gho.start_pos_y;
	cur_pos_x   = gho.cur_pos_x;
	cur_pos_y   = gho.cur_pos_y;
	color       = gho.color;
	return *this;
};

void ghost::paint() {

}

void ghost::reset() {
	cur_pos_x = start_pos_x;
	cur_pos_y = start_pos_y;
}

pair<unsigned int, unsigned int> ghost::beginning_position() {
	return make_pair(start_pos_x, start_pos_y);
}
pair<unsigned int, unsigned int> ghost::current_position() {
	return make_pair(cur_pos_x, cur_pos_y);
}