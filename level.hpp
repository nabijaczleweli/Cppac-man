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
#include <curses.h>
#include "ghost.hpp"

namespace {
	using namespace std;

	struct level {
		public:
			const ghost * ghosts;
			unsigned short int amount_ghosts;

			unsigned int amount_walls;
			const pair<unsigned short int, unsigned short int> * places_of_walls;
			pair<unsigned short int, unsigned short int> size;  // X, Y
			WINDOW * screen;
		private:
			char ** cache = NULL;
			void make_cache() {
				cache = new char*[size.first];
				for(unsigned short int i = 0; i < size.second; ++i)
					cache[i] = new char[size.second];
				for(unsigned short int x = 0; x < size.first; ++x)
					for(unsigned short int y = 0; y < size.second; ++y)
						cache[x][y] = ' ';
				for(unsigned short int pofw = 0; pofw < amount_walls; ++pofw) {
					const auto & wall = places_of_walls[pofw];
					cache[wall.first][wall.second] = '#';
				}
			}
		public:
			level() : level(vector<ghost>(), vector<pair<unsigned short int, unsigned short int>>(), make_pair(-1, -1), NULL) {}
			level(const vector<ghost> & ghosts_v, const vector<pair<unsigned short int, unsigned short int>> & walls_v, pair<unsigned short int, unsigned short int> lvlsize, WINDOW * lvlscreen) :
			                                                               ghosts(new ghost[ghosts_v.size()]), amount_ghosts(ghosts_v.size()), amount_walls(walls_v.size()),
			                                                               places_of_walls(new pair<unsigned short int, unsigned short int>[walls_v.size()]), size(lvlsize), screen(lvlscreen) {
				memcpy(const_cast<pair<unsigned short int, unsigned short int> *>(places_of_walls), walls_v.data(), amount_walls);
				memcpy(const_cast<ghost *>(ghosts), ghosts_v.data(), amount_ghosts);
			}
			level(const level &) = delete;
			level(level && lvl) {
			  amount_ghosts = lvl.amount_ghosts;
			  amount_walls = lvl.amount_walls;
			  ghosts = lvl.ghosts;
			  places_of_walls = lvl.places_of_walls;
			  size.first = lvl.size.first;
			  size.second = lvl.size.second;
			  screen = lvl.screen;

			  lvl.amount_ghosts = -1;
			  lvl.amount_walls = -1;
			  lvl.ghosts = NULL;
			  lvl.places_of_walls = NULL;
			  lvl.size.first = -1;
			  lvl.size.second = -1;
			  lvl.screen = NULL;
			}

			~level() {
				if(ghosts)
					delete[] ghosts;
				if(places_of_walls)
					delete[] places_of_walls;
				if(cache) {
					for(unsigned short int x = 0; x < size.first; ++x) {
						delete[] cache[x];
						cache[x] = NULL;
					}
					delete[] cache;
				}

				ghosts          = NULL;
				places_of_walls = NULL;
				cache           = NULL;

				amount_walls  = -1;
				amount_ghosts = -1;
				size.first    = -1;
				size.second   = -1;
			}

			level & operator=(const level & lvl) {
			  amount_ghosts = lvl.amount_ghosts;
			  amount_walls = lvl.amount_walls;
			  ghosts = new ghost[lvl.amount_ghosts];
			  places_of_walls = new pair<unsigned short int, unsigned short int>[lvl.amount_walls];
				memcpy(const_cast<pair<unsigned short int, unsigned short int> *>(places_of_walls), lvl.places_of_walls, amount_walls);
				memcpy(const_cast<ghost *>(ghosts), lvl.ghosts, amount_ghosts);
			  size.first = lvl.size.first;
			  size.second = lvl.size.second;
			  screen = lvl.screen;
				return *this;
			}

			void paint() {
				if(!cache)
					make_cache();
				for(unsigned short int x = 0; x < size.first; ++x)
					for(unsigned short int y = 0; y < size.second; ++y)
						mvwaddch(screen, y, x, cache[x][y]);
				wrefresh(screen);
			}

			void paint(WINDOW * screen) {
				this->screen = screen;
				paint();
			}
	};
}


#endif  // LEVEL_HPP
