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
#include <array>
#include <chrono>
#include <map>

/******** GLFW HEADERS ***********/
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

/******** VULKAN HEADERS ***********/
#include <vulkan/vulkan.h>

/******** GLM HEADERS ***********/
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/******** STB HEADERS ***********/
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>

const bool enableValidationLayers =
#ifdef _DEBUG
	true;
#else
	false;
#endif // DEBUG
