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

//
//   /$$$$$$                                               /$$
//  |_  $$_/                                              | $$
//    | $$   /$$$$$$/$$$$   /$$$$$$   /$$$$$$   /$$$$$$  /$$$$$$    /$$$$$$$
//    | $$  | $$_  $$_  $$ /$$__  $$ /$$__  $$ /$$__  $$|_  $$_/   /$$_____/
//    | $$  | $$ \ $$ \ $$| $$  \ $$| $$  \ $$| $$  \__/  | $$    |  $$$$$$
//    | $$  | $$ | $$ | $$| $$  | $$| $$  | $$| $$        | $$ /$$ \____  $$
//   /$$$$$$| $$ | $$ | $$| $$$$$$$/|  $$$$$$/| $$        |  $$$$/ /$$$$$$$/
//  |______/|__/ |__/ |__/| $$____/  \______/ |__/         \___/  |_______/
//                        | $$
//                        | $$
//    /$$$                |__/
//   /$$ $$
//  |  $$$
//   /$$ $$/$$
//  | $$  $$_/
//  | $$\  $$
//  |  $$$$/$$
//   \____/\_/
//
//
//   /$$$$$$$             /$$$$$$  /$$
//  | $$__  $$           /$$__  $$|__/
//  | $$  \ $$  /$$$$$$ | $$  \__/ /$$ /$$$$$$$   /$$$$$$   /$$$$$$$
//  | $$  | $$ /$$__  $$| $$$$    | $$| $$__  $$ /$$__  $$ /$$_____/
//  | $$  | $$| $$$$$$$$| $$_/    | $$| $$  \ $$| $$$$$$$$|  $$$$$$
//  | $$  | $$| $$_____/| $$      | $$| $$  | $$| $$_____/ \____  $$
//  | $$$$$$$/|  $$$$$$$| $$      | $$| $$  | $$|  $$$$$$$ /$$$$$$$/
//  |_______/  \_______/|__/      |__/|__/  |__/ \_______/|_______/
//
//

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <curses.h>
// <tui.h>?
#include "utils.hpp"
#include "level.hpp"
#include "ASCII charcodes.hpp"

using namespace std;
using namespace std::chrono;
using namespace utils;

//
//   /$$$$$$$                      /$$                                /$$     /$$
//  | $$__  $$                    | $$                               | $$    |__/
//  | $$  \ $$  /$$$$$$   /$$$$$$$| $$  /$$$$$$   /$$$$$$  /$$$$$$  /$$$$$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$
//  | $$  | $$ /$$__  $$ /$$_____/| $$ |____  $$ /$$__  $$|____  $$|_  $$_/  | $$ /$$__  $$| $$__  $$ /$$_____/
//  | $$  | $$| $$$$$$$$| $$      | $$  /$$$$$$$| $$  \__/ /$$$$$$$  | $$    | $$| $$  \ $$| $$  \ $$|  $$$$$$
//  | $$  | $$| $$_____/| $$      | $$ /$$__  $$| $$      /$$__  $$  | $$ /$$| $$| $$  | $$| $$  | $$ \____  $$
//  | $$$$$$$/|  $$$$$$$|  $$$$$$$| $$|  $$$$$$$| $$     |  $$$$$$$  |  $$$$/| $$|  $$$$$$/| $$  | $$ /$$$$$$$/
//  |_______/  \_______/ \_______/|__/ \_______/|__/      \_______/   \___/  |__/ \______/ |__/  |__/|_______/
//
//

//  Level format is as follows:
//  SOH
//  SI
//  Level's overall height (decimal)
//  ,
//  Level's overall width (decimal)
//  SO
//  EM
//  SI
//  Level's overall amount of ghosts (decimal)
//    now, repeating Level's overall amount of ghosts times :
//  ETB
//  Ghost's beginning position X (decimal)
//  ,
//  Ghost's beginning position Y (decimal)
//  ,
//  Set of ORed attributes (decimal, attr_t)
//    end repeating
//  SO
//  EM
//  SI
//  Level's overall amount of walls (decimal)
//    now, repeating Level's overall amount of walls times :
//  ETB
//  Wall's position X (decimal)
//  ,
//  Wall's position Y (decimal)
//    end repeating
//  SO
//  STX
//  Any set of characters

level load_level(const string & level_filepath, WINDOW * const screen);
bool check_level_integrity(const string & level_filepath);

//
//    /$$$$$$                  /$$
//   /$$__  $$                | $$
//  | $$  \__/  /$$$$$$   /$$$$$$$  /$$$$$$
//  | $$       /$$__  $$ /$$__  $$ /$$__  $$
//  | $$      | $$  \ $$| $$  | $$| $$$$$$$$
//  | $$    $$| $$  | $$| $$  | $$| $$_____/
//  |  $$$$$$/|  $$$$$$/|  $$$$$$$|  $$$$$$$
//   \______/  \______/  \_______/ \_______/
//
//

int main(int, char * argv[]) {
	vector<string> level_filenames;
	for(unsigned int idx = 1; argv[idx]; ++idx) {
		const char * const arg = argv[idx];
		if(*arg == '-' && arg[1]) {
			if(!memcmp(arg, "--help-level-format", 18)) {
				cout << "The level format is as follows:\n"
				        "\n"
                "SOH\n"
                "SI\n"
                "Level's overall height (decimal)\n"
                ",\n"
                "Level's overall width (decimal)\n"
                "SO\n"
                "EM\n"
                "SI\n"
                "Level's overall amount of ghosts (decimal)\n"
                "\tnow, repeating <Level's overall amount of ghosts> times :\n"
                "ETB\n"
                "Ghost's beginning position X (decimal)\n"
                ",\n"
                "Ghost's beginning position Y (decimal)\n"
                ",\n"
                "Set of ORed attributes (decimal, attr_t)\n"
                "\tend repeating\n"
                "SO\n"
                "EM\n"
                "SI\n"
                "Level's overall amount of walls (decimal)\n"
                "\tnow, repeating <Level's overall amount of walls> times :\n"
                "ETB\n"
                "Wall's position X (decimal)\n"
                ",\n"
                "Wall's position Y (decimal)\n"
                "\tend repeating\n"
                "SO\n"
                "STX\n"
                "Any set of characters\n";
				return 0;
			}
		}	else
			level_filenames.emplace_back(arg);
	}

	unique(level_filenames.begin(), level_filenames.end());
	remove_if(level_filenames.begin(), level_filenames.end(), [](const string & level_filename) {
		const bool does_exist = does_file_exist(level_filename.c_str());
		if(!does_exist)
			cout << level_filename << " does not exist.\n";
		return !does_exist;
	});
	remove_if(level_filenames.begin(), level_filenames.end(), [](const string & level_filename) {
		const bool is_a_level = check_level_integrity(level_filename);
		cout << level_filename << (is_a_level ? " is an actual level" : " is not an actual level. Totally skipping this one") << ".\n";
		return !is_a_level;
	});

	level * levels = new level[level_filenames.size()];
	WINDOW * mainscreen = initscr();

	//for(unsigned int idx = 0; idx < level_filenames.size(); ++idx)
		levels[0] = load_level(level_filenames[0], mainscreen);

	levels[0].paint();

	endwin();
	delete[] levels;
	levels = NULL;
	mainscreen = NULL;
}

//
//   /$$$$$$$             /$$$$$$  /$$           /$$   /$$     /$$
//  | $$__  $$           /$$__  $$|__/          |__/  | $$    |__/
//  | $$  \ $$  /$$$$$$ | $$  \__/ /$$ /$$$$$$$  /$$ /$$$$$$   /$$  /$$$$$$  /$$$$$$$   /$$$$$$$
//  | $$  | $$ /$$__  $$| $$$$    | $$| $$__  $$| $$|_  $$_/  | $$ /$$__  $$| $$__  $$ /$$_____/
//  | $$  | $$| $$$$$$$$| $$_/    | $$| $$  \ $$| $$  | $$    | $$| $$  \ $$| $$  \ $$|  $$$$$$
//  | $$  | $$| $$_____/| $$      | $$| $$  | $$| $$  | $$ /$$| $$| $$  | $$| $$  | $$ \____  $$
//  | $$$$$$$/|  $$$$$$$| $$      | $$| $$  | $$| $$  |  $$$$/| $$|  $$$$$$/| $$  | $$ /$$$$$$$/
//  |_______/  \_______/|__/      |__/|__/  |__/|__/   \___/  |__/ \______/ |__/  |__/|_______/
//
//

bool check_level_integrity(const string & level_filepath) {
	ifstream lvl(level_filepath, ios::in | ios::binary);
	bool not_ok = false;
	not_ok |= lvl.get() != CHR_SOH;
	not_ok |= lvl.get() != CHR_SI;
	while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
		not_ok |= !isdigit(lvl.get());
	not_ok |= lvl.get() != ',';
	while(lvl.peek() != CHR_SO && lvl.peek() != ifstream::traits_type::eof())
		not_ok |= !isdigit(lvl.get());
	not_ok |= lvl.get() != CHR_SO;
	not_ok |= lvl.get() != CHR_EM;
	not_ok |= lvl.get() != CHR_SI;

	string amount_of_ghosts_string;
	while(lvl.peek() != CHR_ETB && lvl.peek() != ifstream::traits_type::eof()) {
		char chr = lvl.get();
		not_ok |= !isdigit(chr);
		amount_of_ghosts_string.push_back(chr);
	}
	int amount_of_ghosts_int = atoi(amount_of_ghosts_string.c_str());
	for(int i = 0; i < amount_of_ghosts_int; ++i) {
		not_ok |= lvl.get() != CHR_ETB;
		while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		not_ok |= lvl.get() != ',';
		while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		not_ok |= lvl.get() != ',';
		while(!(lvl.peek() == CHR_ETB || lvl.peek() == CHR_SO) && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		if(i >= amount_of_ghosts_int - 1)
			not_ok |= lvl.get() != CHR_SO;
	}
	not_ok |= lvl.get() != CHR_EM;
	not_ok |= lvl.get() != CHR_SI;

	string amount_of_walls_string;
	while(lvl.peek() != CHR_ETB && lvl.peek() != ifstream::traits_type::eof()) {
		char chr = lvl.get();
		not_ok |= !isdigit(chr);
		amount_of_walls_string.push_back(chr);
	}
	int amount_of_walls_int = atoi(amount_of_walls_string.c_str());
	for(int i = 0; i < amount_of_walls_int; ++i) {
		not_ok |= lvl.get() != CHR_ETB;
		while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		not_ok |= lvl.get() != ',';
		while(!(lvl.peek() == CHR_ETB || lvl.peek() == CHR_SO) && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		if(i >= amount_of_walls_int - 1)
			not_ok |= lvl.get() != CHR_SO;
	}
	not_ok |= lvl.get() != CHR_STX;
	not_ok |= lvl.get() == ifstream::traits_type::eof();
	return !not_ok;
}

level load_level(const string & level_filepath, WINDOW * const screen) {
	ifstream lvl(level_filepath, ios::in | ios::binary);

	vector<pair<unsigned short int, unsigned short int>> walls;
	walls.push_back(make_pair(1, 1));

	return level(vector<ghost>(), walls, make_pair(10, 10), screen);
}
