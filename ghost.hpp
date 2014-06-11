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

#ifndef GHOST_HPP
#define GHOST_HPP

#include <utility>
#include <cstddef>
#include <tui.h>
#include "paintable.hpp"

class ghost : public paintable {
	private:
		int start_pos_x             = -1;
		int start_pos_y             = -1;
		int cur_pos_x               = -1;
		int cur_pos_y               = -1;\
		attr_t color                = -1;
		const char * reactions_name = NULL;
	public:
		void * reactions;
		ghost();
		ghost(attr_t color, std::pair<int, int> start_pos);
		ghost(attr_t color, std::pair<int, int> start_pos, WINDOW * screen);
		ghost(const ghost &);
		ghost(ghost &&);
		~ghost();
		ghost & operator=(const ghost &);
		void paint();
		void reset();
		std::pair<unsigned int, unsigned int> beginning_position();
		std::pair<unsigned int, unsigned int> current_position();
};


#endif  // GHOST_HPP
