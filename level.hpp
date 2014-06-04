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
#include "ghost.hpp"

namespace {
	using namespace std;

	struct level {
		const ghost * ghosts;
		unsigned short int amount_ghosts;

		unsigned int amount_walls;
		const pair<unsigned short int, unsigned short int> * places_of_walls;

		level() = delete;
		level(const vector<ghost> & ghosts_v, const vector<pair<unsigned short int, unsigned short int>> & walls) : ghosts(new ghost[ghosts_v.size()]), amount_ghosts(ghosts_v.size()),
		                                                                                                            amount_walls(walls.size()),
		                                                                                                            places_of_walls(new pair<unsigned short int, unsigned short int>[walls.size()]) {
			memcpy(const_cast<pair<unsigned short int, unsigned short int> *>(places_of_walls), walls.data(), amount_walls);
			memcpy(const_cast<ghost *>(ghosts), ghosts_v.data(), amount_ghosts);
		}
		level(const level &) = delete;
		level(level &&) = delete;

		~level() {
			if(ghosts)
				delete[] ghosts;
			if(places_of_walls)
				delete[] places_of_walls;

			ghosts          = NULL;
			places_of_walls = NULL;

			amount_walls  = -1;
			amount_ghosts = -1;
		}
	};
}


#endif  // LEVEL_HPP
