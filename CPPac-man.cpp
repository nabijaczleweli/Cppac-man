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
#include <thread>
#include <algorithm>
#include <tui.h>
#include "utils.hpp"
#include "level.hpp"
#include "pacman.hpp"
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
//  ,
//  Path to library with ghost's logic (string, (isalnum || isspace || isprint))
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
//  EM
//  SI
//  Pacman's beginning position X (decimal)
//  ,
//  Pacman's beginning position Y (decimal)
//  SO
//  STX
//  Any set of characters (string, at least 1)

level load_level(const string & level_filepath, WINDOW *& screen);
bool check_level_integrity(const string & level_filepath);
bool check_level_integrity(const level & lvl);
constexpr bool ispath(int ch);

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
			if(!memcmp(arg, "--help", 6)) {
				if(!arg[7]) {
					cout << "Usage: " << argv[0] << " [arguments] level...\n"
					        "Arguments:\n"
					        "\t--help: display this help screen.\n"
					        "\t--help-level-format: display information about format of levels.";
					return 0;
				} else if(!memcmp(arg + 6, "-level-format", 12)) {
					cout << "The level format is as follows:\n\nSOH\nSI\nLevel's overall height (decimal)\n,\nLevel's overall width (decimal)\nSO\nEM\nSI\nLevel's overall amount of ghosts (decimal)\n\tnow,"
					        " repeating Level's overall amount of ghosts times :\nETB\nGhost's beginning position X (decimal)\n,\nGhost's beginning position Y (decimal)\n,\nSet of ORed attributes (decimal"
					        ", attr_t)\n,\nPath to library with ghost's logic (string, (isalnum || isspace || isprint))\n  end repeating\nSO\nEM\nSI\nLevel's overall amount of walls (decimal)\n\tnow, "
        	        "repeating Level's overall amount of walls times :\nETB\nWall's position X (decimal)\n,\nWall's position Y (decimal)\n  end repeating\nSO\nEM\nSI\nPacman's beginning position X "
        	        "(decimal)\n,\nPacman's beginning position Y (decimal)\nSO\nSTX\nAny set of characters (string, at least 1)\n";
					return 0;
				}
			}
		}	else
			level_filenames.emplace_back(arg);
	}

	if(!level_filenames.size()) {
		cout << "You did NOT pass any level paths!\n"
		        "You need to pass at least one of them.";
		return 0;
	}

	WINDOW * mainscreen = initscr();
	if(has_colors())
		start_color();
	clearok(mainscreen, true);
	level_filenames.erase(unique(level_filenames.begin(), level_filenames.end()), level_filenames.end());
	level_filenames.erase(remove_if(level_filenames.begin(), level_filenames.end(), [](const string & level_filename) {
		const bool is_nonexistant = !does_file_exist(level_filename.c_str());
		if(is_nonexistant)
			addstr((level_filename + " does not exist. Totally skipping this one.\n").c_str());
		return is_nonexistant;
	}), level_filenames.end());
	level_filenames.erase(remove_if(level_filenames.begin(), level_filenames.end(), [](const string & level_filename) {
		const bool is_a_level = check_level_integrity(level_filename);
		addstr((level_filename + (is_a_level ? " is a level" : " is not an actual level. Totally skipping this one") + ".\n").c_str());
		return !is_a_level;
	}), level_filenames.end());
	if(!level_filenames.size()) {
		refresh();
		getch();
		endwin();
		cout << "No levels left! Quitting!";
		return 0;
	}
	vector<level> levels;
	for(const string & level_filename : level_filenames) {
		levels.push_back(load_level(level_filename, mainscreen));
		if(!check_level_integrity(levels.back())) {
			addstr((level_filename + " is corrupt. Totally skipping this one.\n").c_str());
			levels.pop_back();
			level_filenames.erase(find(level_filenames.begin(), level_filenames.end(), level_filename));
		}
	}
	refresh();
	getch();
	if(!level_filenames.size() || !levels.size()) {
		endwin();
		cout << "No levels left! Quitting!";
		return 0;
	}

	clear();
	levels.at(0).paint();
	refresh();
	getch();

	endwin();
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

constexpr bool ispath(int ch) {
	return isalnum(ch) | isspace(ch) | isprint(ch);
}

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
		while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !isdigit(lvl.get());
		not_ok |= lvl.get() != ',';
		while(!(lvl.peek() == CHR_ETB || lvl.peek() == CHR_SO) && lvl.peek() != ifstream::traits_type::eof())
			not_ok |= !ispath(lvl.get());
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
	not_ok |= lvl.get() != CHR_EM;

	not_ok |= lvl.get() != CHR_SI;
	while(lvl.peek() != ',' && lvl.peek() != ifstream::traits_type::eof())
		not_ok |= !isdigit(lvl.get());
	not_ok |= lvl.get() != ',';
	while(lvl.peek() != CHR_SO && lvl.peek() != ifstream::traits_type::eof())
		not_ok |= !isdigit(lvl.get());
	not_ok |= lvl.get() != CHR_SO;

	not_ok |= lvl.get() != CHR_STX;
	not_ok |= lvl.get() == ifstream::traits_type::eof();
	return !not_ok;
}

level load_level(const string & level_filepath, WINDOW *& screen) {
	ifstream lvl(level_filepath, ios::in | ios::binary);
	vector<pair<unsigned int, unsigned int>> walls;
	vector<ghost> ghosts;

	lvl.ignore(2);
	pair<unsigned int, unsigned int> level_size;
	{
		string level_size_x_string;
		while(isdigit(lvl.peek()))
			level_size_x_string.push_back(lvl.get());
		level_size.first = atoi(level_size_x_string.c_str());
	}
	lvl.ignore(1);
	{
		string level_size_y_string;
		while(isdigit(lvl.peek()))
			level_size_y_string.push_back(lvl.get());
		level_size.second = atoi(level_size_y_string.c_str());
	}

	lvl.ignore(3);

	unsigned int amount_of_ghosts;
	{
		string amount_of_ghosts_string;
		while(isdigit(lvl.peek()))
			amount_of_ghosts_string.push_back(lvl.get());
		amount_of_ghosts = atoi(amount_of_ghosts_string.c_str());
	}
	for(unsigned int i = 0; i < amount_of_ghosts; ++i) {
		lvl.ignore(1);
		pair<int, int> ghost_position;
		{
			string ghost_position_x_string;
			while(isdigit(lvl.peek()))
				ghost_position_x_string.push_back(lvl.get());
			ghost_position.first = atoi(ghost_position_x_string.c_str());
		}
		lvl.ignore(1);
		{
			string ghost_position_y_string;
			while(isdigit(lvl.peek()))
				ghost_position_y_string.push_back(lvl.get());
			ghost_position.second = atoi(ghost_position_y_string.c_str());
		}
		lvl.ignore(1);
		attr_t ghost_attributes;
		{
			string ghost_attributes_string;
			while(isdigit(lvl.peek()))
				ghost_attributes_string.push_back(lvl.get());
			ghost_attributes = atoi(ghost_attributes_string.c_str());
		}
		lvl.ignore(1);
		string ghost_logic_path;
		while(lvl.peek() != CHR_SO)
			ghost_logic_path.push_back(lvl.get());
		ghosts.emplace_back(ghost_attributes, ghost_position, ghost_logic_path.c_str(), screen);
	}

	lvl.ignore(3);

	unsigned int amount_of_walls;
	{
		string amount_of_walls_string;
		while(isdigit(lvl.peek()))
				amount_of_walls_string.push_back(lvl.get());
		amount_of_walls = atoi(amount_of_walls_string.c_str());
	}
	for(unsigned int i = 0; i < amount_of_walls; ++i) {
		lvl.ignore(1);
		unsigned int wall_position_x;
		{
			string wall_position_x_string;
			while(isdigit(lvl.peek()))
				wall_position_x_string.push_back(lvl.get());
			wall_position_x = atoi(wall_position_x_string.c_str());
		}
		lvl.ignore(1);
		unsigned int wall_position_y;
		{
			string wall_position_y_string;
			while(isdigit(lvl.peek()))
				wall_position_y_string.push_back(lvl.get());
			wall_position_y = atoi(wall_position_y_string.c_str());
		}
		walls.emplace_back(wall_position_x, wall_position_y);
	}
	unsigned int pacman_pos_x;
	{
		string pacman_pos_x_string;
		while(isdigit(lvl.peek()))
			pacman_pos_x_string.push_back(lvl.get());
		pacman_pos_x = atoi(pacman_pos_x_string.c_str());
	}
	unsigned int pacman_pos_y;
	{
		string pacman_pos_y_string;
		while(isdigit(lvl.peek()))
			pacman_pos_y_string.push_back(lvl.get());
		pacman_pos_y = atoi(pacman_pos_y_string.c_str());
	}

	return level(ghosts, walls, level_size, screen, pacman(make_pair(pacman_pos_x, pacman_pos_y), screen));
}

bool check_level_integrity(const level & lvl) {
	bool not_ok = false;
	const unsigned int level_width = lvl.size.first;
	const unsigned int level_height = lvl.size.second;
	if(!lvl.places_of_walls)
		throw bad_alloc();
	for(unsigned int i = 0; i < lvl.amount_walls; ++i) {
		not_ok |= lvl.places_of_walls[i][0] >= level_width;
		not_ok |= lvl.places_of_walls[i][1] >= level_height;
	}
	if(!lvl.ghosts)
		throw bad_alloc();
	for(unsigned int i = 0; i < lvl.amount_ghosts; ++i) {
		not_ok |= lvl.ghosts[i].beginning_position().first >= level_width;
		not_ok |= lvl.ghosts[i].beginning_position().second >= level_height;
		not_ok |= lvl.ghosts[i].current_position().first >= level_width;
		not_ok |= lvl.ghosts[i].current_position().second >= level_height;
		not_ok |= !ghost::check_ghost_dynamic_integrity(lvl.ghosts[i]);
	}
	if(!lvl.pac_man)
		throw bad_alloc();  // Any better exception here?
	not_ok |= lvl.pac_man->beginning_position().first >= level_width;
	not_ok |= lvl.pac_man->beginning_position().second >= level_height;
	not_ok |= lvl.pac_man->actual_position().first >= level_width;
	not_ok |= lvl.pac_man->actual_position().second >= level_height;
	return !not_ok;
}