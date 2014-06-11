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

#ifndef RUNTIME__DL_LS_HPP
#define RUNTIME__DL_LS_HPP

#ifdef _WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#else
	#include <dlfcn.h>
#endif

typedef void * dllhandle;

constexpr dllhandle load_library(const char * const libpath, const int open_params) {
	return libpath ?
#ifdef _WIN32
	LoadLibrary(libpath) + (open_params - open_params)
#else
	dlopen(libpath, open_params)
#endif
	: NULL;
}

constexpr void * get_library_function_address(const dllhandle dll, const char * const function_name) {
	return (dll && function_name) ?
#ifdef _WIN32
	(void *)GetProcAddress((HMODULE)dll, function_name)
#else
	dlsym(dll, function_name);
#endif
	: NULL;
}

template<class Ret, class... Args>
constexpr auto get_library_function(const dllhandle dll, const char * const function_name) -> Ret(*)(Args...) {
	return (Ret(*)(Args...))get_library_function_address(dll, function_name);
}

void unload_library(const dllhandle dll) {
	if(!dll)
		return;
#ifdef _WIN32
	FreeLibrary((HMODULE)dll);
#else
	dlclose(dll);
#endif
}

#endif  // RUNTIME__DL_LS_HPP
