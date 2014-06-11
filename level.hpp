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

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <utility>
#include <vector>
#include <fstream>
#include <tui.h>
#include "ghost.hpp"
#include "pacman.hpp"

namespace {
	using namespace std;

	class level : public paintable {
		public:
			ghost * ghosts;
			unsigned int amount_ghosts;

			unsigned int amount_walls;
			unsigned int ** places_of_walls;
			pair<unsigned int, unsigned int> size;  // X, Y
			pacman * pac_man;

			level() : level(vector<ghost>(), vector<pair<unsigned int, unsigned int>>(), make_pair(-1, -1), NULL, pacman()) {}
			level(const vector<ghost> & ghosts_v, const vector<pair<unsigned int, unsigned int>> & walls_v, pair<unsigned int, unsigned int> lvlsize, WINDOW * lvlscreen, const pacman & new_pac) :
			                                                               paintable(lvlscreen), ghosts(new ghost[ghosts_v.size()]), amount_ghosts(ghosts_v.size()),
			                                                               amount_walls(walls_v.size()), size(lvlsize), pac_man(new pacman) {
				places_of_walls = new unsigned int*[amount_walls];
				for(unsigned int i = 0; i < amount_walls; ++i) {
					places_of_walls[i] = new unsigned int[2];
					places_of_walls[i][0] = walls_v[i].first;
					places_of_walls[i][1] = walls_v[i].second;
				}
				for(ghost * gho = ghosts; gho < (ghosts + amount_ghosts); ++gho) {
					*gho = ghosts_v[gho - ghosts];
					gho->reset();
				}
				*pac_man = new_pac;
			}
			level(const level &) noexcept = delete;
			level(level && lvl) noexcept : paintable(lvl.screen) {
			  amount_ghosts = lvl.amount_ghosts;
			  amount_walls = lvl.amount_walls;
			  ghosts = lvl.ghosts;
			  places_of_walls = lvl.places_of_walls;
			  size = lvl.size;
			  pac_man = lvl.pac_man;

			  lvl.amount_ghosts = -1;
			  lvl.amount_walls = -1;
			  lvl.ghosts = NULL;
			  lvl.places_of_walls = NULL;
			  lvl.size = make_pair(-1, -1);
			  lvl.screen = NULL;
			  lvl.pac_man = NULL;
			}

			~level() noexcept {
				if(ghosts)
					delete[] ghosts;
				if(places_of_walls) {
					for(unsigned int i = 0; i < amount_walls; ++i) {
						places_of_walls[i][0] = -1;
						places_of_walls[i][1] = -1;
						delete[] places_of_walls[i];
						places_of_walls[i] = NULL;
					}
					delete[] places_of_walls;
				}
				if(pac_man)
					delete pac_man;

				ghosts          = NULL;
				places_of_walls = NULL;
				pac_man         = NULL;

				amount_walls  = -1;
				amount_ghosts = -1;
				size.first    = -1;
				size.second   = -1;
			}

			level & operator=(const level & lvl) {
			  amount_ghosts = lvl.amount_ghosts;
			  amount_walls = lvl.amount_walls;
			  ghosts = new ghost[lvl.amount_ghosts];
			  places_of_walls = new unsigned int*[amount_walls];
				for(unsigned int i = 0; i < amount_walls; ++i) {
					places_of_walls[i] = new unsigned int[2];
					places_of_walls[i][0] = lvl.places_of_walls[i][0];
					places_of_walls[i][1] = lvl.places_of_walls[i][1];
				}
				memcpy(const_cast<ghost *>(ghosts), lvl.ghosts, amount_ghosts);
			  size = lvl.size;
			  screen = lvl.screen;
				return *this;
			}

			void paint() {
				clear();
				for(unsigned int i = 0; i < amount_walls; ++i)
					mvaddch(places_of_walls[i][1], places_of_walls[i][0], '#');
				for(unsigned int i = 0; i < amount_ghosts; ++i)
					ghosts[i].paint();
				pac_man->paint();
			}

			void paint(WINDOW *& scr) {
				paintable::paint(scr);
				wclear(scr);
				for(unsigned int i = 0; i < amount_walls; ++i)
					mvaddch(places_of_walls[i][1], places_of_walls[i][0], '#');
				for(unsigned int i = 0; i < amount_ghosts; ++i)
					ghosts[i].paintable::paint(scr);
				pac_man->paintable::paint(scr);
			}
	};
}

#endif  // LEVEL_HPP
