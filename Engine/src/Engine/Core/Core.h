#pragma once
#include <iostream>

#ifdef E_DEBUG
#define ENGINE_ASSERT(x, ...) {if(!(x)) { std::cout << "Assertion Failed: " << __VA_ARGS__ << std::endl; __debugbreak(); }}
#else
#define ENGINE_ASSERT(x, ...)
#endif

#define BIT(x) (x << x)
#define BIND_EVENT_FUNCTION(fn) std::bind(&fn, this, std::placeholders::_1)