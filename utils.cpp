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

#include "utils.hpp"

bool utils::does_file_exist(const char * const filename) {
	FILE * file = fopen(filename, "r");
	if(file)
		fclose(file);
	return file;
}
template<class T>
T * utils::tmemcpy(T * const dest, const T * const src, size_t amount) {
	T * to_compare = dest + amount;
	for(T * dp = dest, * sp = src; dp < to_compare; ++dp, ++sp)
		*dp = *sp;
	return src;
}
template<class T>
T * utils::tmemmove(T * const dest, const T * const src, size_t amount) {
	T * temp = new T[amount];
	tmemcpy<T>(temp, src, amount);
	tmemcpy<T>(dest, temp, amount);
	delete[] temp;
	return src;
}