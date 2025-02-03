#pragma once
#include <glad/glad.h>
#include <glfw/glfw3.h>

#include <iostream>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Utils/SingleDelegate.h>
#include <Utils/MultiDelegate.h>
#include "Utils/Logger.h"

#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

using std::string;
using std::vector;
using std::deque;
using std::pair;
using std::map;
using std::unordered_map;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::translate;
using glm::rotate;
using glm::scale;
