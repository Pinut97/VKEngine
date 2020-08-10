#pragma once

/******** C/C++ HEADER FILES *********/
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include <mutex>
#include <algorithm>
#include <optional>

/******** GLFW HEADERS ***********/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/******** VULKAN HEADERS ***********/
#include <vulkan/vulkan.h>

const bool enableValidationLayers =
#ifdef _DEBUG
	true;
#else
	false;
#endif // DEBUG
