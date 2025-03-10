/*
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "constants.h"
#include <Windows.h>
#include "shaderprogram.h"
#include "pacman.h"
#include "ghost.h"
#include "lodepng.h"


int map[88][70] = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,4,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,4,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,1,1,1,1,1,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,1,1,1,1,1,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,1,1,1,1,1,1,0,0,2,0,0,1,1,1,1,0,0,2,0,0,1,1,1,1,1,1,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,0,0,6,2,2,2,2,2,6,2,2,2,6,2,2,2,2,6,2,2,2,2,2,6,0,0,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,0,0,2,0,0,1,1,1,1,1,0,0,2,0,0,1,1,1,1,1,1,0,0,2,0,0,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,2,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,2,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 9,2,2,2,2,2,2,2,2,2,2,2,2,2,8,5,2,2,2,2,2,2,2,2,6,0,0,1,0,0,2,2,2,9,6,9,2,2,2,2,0,0,1,0,0,6,2,2,2,2,2,2,2,2,5,8,2,2,2,2,2,2,2,2,2,2,2,2,2,9 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,0,0,2,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,0,0,6,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,6,0,0,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,3,0,0,1,0,0,1,0,0,2,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,2,0,0,1,0,0,1,0,0,3,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,2,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,2,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,4,0,0,1,1,1,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,1,1,1,0,0,4,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,5,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,5,2,2,2,2,2,2,2,2,5,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,5,0,0,1 },
	{ 1,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,1 },
	{ 1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1 },
	{ 1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1 },
	{ 1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,1 },
	{ 1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1 },
	{ 1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,1 },
	{ 1,0,0,5,2,2,5,2,2,3,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,5,0,0,1,0,0,1,0,0,5,2,2,3,2,2,3,2,2,5,2,2,5,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,2,0,0,1,0,0,1,0,0,2,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,3,0,0,1,0,0,1,0,0,3,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1,0,0,1,0,0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,2,0,0,1 },
	{ 1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1,1,1,1,0,0,3,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,3,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1 },
	{ 1,0,0,5,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,5,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,3,2,2,5,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

int map_copy[88][70];
float aspectRatio = 1;

PacMan player;
Ghost ghost[4];

ShaderProgram* sp = nullptr;

GLuint tex_wall, tex_candy, tex_ghosts[4];

std::vector <float> candyVertices;
std::vector <float> candyNormals;
std::vector <float> candyTexCoords;

std::vector <float> mapVertices;
std::vector <float> mapNormals;
std::vector <float> mapTexCoords;

glm::vec4 lightSources[210] = {};

GLuint vbo[6];

UINT_PTR id_start_chase;

//Error processing callback procedure
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Procedura obs³ugi klawiatury
void key_callback(GLFWwindow* window, int key,
	int scancode, int action, int mods) {

	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {

			player.next_move = 0;
		}
		if (key == GLFW_KEY_RIGHT) {

			player.next_move = 2;
		}
		
		if (key == GLFW_KEY_UP) {

			player.next_move = 1;
		}
		if (key == GLFW_KEY_DOWN) {

			player.next_move = 3;
		}

	}
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Load into computer's memory
	std::vector<unsigned char> image;   //Allocate a vector for image storage
	unsigned width, height;   //Variables for image size
	//Read image
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import into graphics card's memory
	glGenTextures(1, &tex); //Initialize one handle
	glBindTexture(GL_TEXTURE_2D, tex); //Activate the handle
	//Import image into graphics card's memory associated with the handle
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

bool readOBJ(const char* filename, std::vector <float> &vertices, std::vector <float> &normals, std::vector <float> &texCoords) {
	FILE* fptr = nullptr;
	
	if (fopen_s(&fptr, filename, "r") == 0) {

		std::vector <float> OBJVert_temp;
		std::vector <float> OBJVert;
		std::vector <float> OBJNorm_temp;
		std::vector <float> OBJNorm;
		std::vector <float> OBJTex_temp;
		std::vector <float> OBJTex;
		
		while (1) {
			char liner[255]{};
			float temp[3]{};

			int res = fscanf_s(fptr, "%s ", &liner, 3);
			
			if (res == EOF) {
				break;
			}

			if (strcmp(liner, "v") == 0) {

				fscanf_s(fptr, "%f %f %f\n", &temp[0], &temp[1], &temp[2]);

				OBJVert_temp.push_back(temp[0]);
				OBJVert_temp.push_back(temp[1]);
				OBJVert_temp.push_back(temp[2]);
				OBJVert_temp.push_back(1.0f);
			}
			else if (strcmp(liner, "vn") == 0) {
				
				fscanf_s(fptr, "%f %f %f\n", &temp[0], &temp[1], &temp[2]);
				OBJNorm_temp.push_back(temp[0]);
				OBJNorm_temp.push_back(temp[1]);
				OBJNorm_temp.push_back(temp[2]);
				OBJNorm_temp.push_back(0.0f);
			}
			else if (strcmp(liner, "vt") == 0) {
				
				fscanf_s(fptr, "%f %f\n", &temp[0], &temp[1]);
				OBJTex_temp.push_back(temp[0]);
				OBJTex_temp.push_back(temp[1]);
			}
			else if (strcmp(liner, "f") == 0) {
				int vertex[3]{};
				int texCoord[3]{};
				int normal[3]{};

				fscanf_s(fptr, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertex[0], &texCoord[0], &normal[0], &vertex[1], &texCoord[1], &normal[1], &vertex[2], &texCoord[2], &normal[2]);

				for (int i = 0; i < 4; i++) {
					OBJVert.push_back(OBJVert_temp[((vertex[0] - 1) * 4) + i]);
					OBJNorm.push_back(OBJNorm_temp[((normal[0] - 1) * 4) + i]);
				}

				for (int i = 0; i < 4; i++) {
					OBJVert.push_back(OBJVert_temp[((vertex[1] - 1) * 4) + i]);
					OBJNorm.push_back(OBJNorm_temp[((normal[1] - 1) * 4) + i]);
				}

				for (int i = 0; i < 4; i++) {
					OBJVert.push_back(OBJVert_temp[((vertex[2] - 1) * 4) + i]);
					OBJNorm.push_back(OBJNorm_temp[((normal[2] - 1) * 4) + i]);
				}

				for (int i = 0; i < 2; i++) {
					OBJTex.push_back(OBJTex_temp[((texCoord[0] - 1) * 2) + i]);
				}

				for (int i = 0; i < 2; i++) {
					OBJTex.push_back(OBJTex_temp[((texCoord[1] - 1) * 2) + i]);
				}

				for (int i = 0; i < 2; i++) {
					OBJTex.push_back(OBJTex_temp[((texCoord[2] - 1) * 2) + i]);
				}
			}
		}

		fclose(fptr);

		vertices = OBJVert;
		normals = OBJNorm;
		texCoords = OBJTex;
	}
	else {
		printf("Problem opening the file: %s\n", filename);
		return 1;
	}
	
	return 0;
}

void prepereMap() {

	int i = 0;

	for (int z = 0; z < 88; z++) {
		for (int x = 0; x < 70; x++) {
			
			if (map_copy[z][x] == NULL) {
				map_copy[z][x] = map[z][x];
			}
			else {
				map[z][x] = map_copy[z][x];
			}
			
			if (map[z][x] > 2 && map[z][x] < 6) {
				lightSources[i] = glm::vec4(x * (-0.2f), 1.0f, z * (-0.2f), 0.0f);
				i++;
			}
		}
	}
}

//Initialization code procedure
void initOpenGLProgram(GLFWwindow* window) {

	prepereMap();

	initShaders();
	//************Place any code here that needs to be executed once, at the program start************
	glClearColor(0, 0, 0, 1); //Set color buffer clear color
	glEnable(GL_DEPTH_TEST); //Turn on pixel depth test based on depth buffer
	
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, key_callback);
	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");
	
	player.tex = readTexture("pacman_skin.png");
	tex_wall = readTexture("wall.png");
	tex_candy = readTexture("candy_skin.png");
	ghost[0].tex = readTexture("blinky.png");
	ghost[1].tex = readTexture("pinky.png");
	ghost[2].tex = readTexture("inky.png");
	ghost[3].tex = readTexture("clyde.png");
	ghost[0].tex_run_away = tex_wall;

	readOBJ("pacman_half.obj", player.vert, player.norm, player.texCo);
	readOBJ("map.obj", mapVertices, mapNormals, mapTexCoords);
	readOBJ("candy.obj", candyVertices, candyNormals, candyTexCoords);
	readOBJ("ghost.obj", ghost[0].vert, ghost[0].norm, ghost[0].texCo);

	player.init();
	ghost[0].init();

	for (int i = 1; i < 4; i++) {
		ghost[i].vert = ghost[0].vert;
		ghost[i].norm = ghost[0].norm;
		ghost[i].texCo = ghost[0].texCo;
		ghost[i].tex_run_away = tex_wall;
		
		ghost[i].init();
	}

	glGenBuffers(6, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, candyVertices.size() * sizeof(float), &candyVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, candyNormals.size() * sizeof(float), &candyNormals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, candyTexCoords.size() * sizeof(float), &candyTexCoords[0], GL_STATIC_DRAW);


	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, mapVertices.size() * sizeof(float), &mapVertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, mapNormals.size() * sizeof(float), &mapNormals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, mapTexCoords.size() * sizeof(float), &mapTexCoords[0], GL_STATIC_DRAW);
	
	sp->use();
}

//Release resources allocated by the program
void freeOpenGLProgram(GLFWwindow* window) {
	freeShaders();
	delete sp;
	glUnmapNamedBuffer(vbo[0]);
	glUnmapNamedBuffer(vbo[1]);
	glUnmapNamedBuffer(vbo[2]);

	glUnmapNamedBuffer(vbo[3]);
	glUnmapNamedBuffer(vbo[4]);
	glUnmapNamedBuffer(vbo[5]);

	//************Place any code here that needs to be executed once, after the main loop ends************
}

//Drawing procedure
void drawScene(GLFWwindow* window) {
	//************Place any code here that draws something inside the window******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear color and depth buffers

	glm::mat4 P = glm::perspective(glm::radians(50.0f), aspectRatio, 1.0f, 50.0f); //Compute projection matrix
	glm::mat4 V = glm::lookAt(glm::vec3(player.x, 7.0f, player.z-4.0f), glm::vec3(player.x, 0.0f, player.z), glm::vec3(0.0f, 1.0f, 0.0f)); //Compute view matrix

	glm::mat4 M = glm::mat4(1.0f);
	
	glUniform4f(sp->u("cameraPos"), player.x, 7.0f, player.z - 4.0f, 1.0f);
	
	glUniform4fv(sp->u("lights"), 210, glm::value_ptr(lightSources[0]));
	
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));

	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));

	
	player.draw(sp, M);


	//BEGIN ghosts
	for (int i = 0; i < 4; i++) {
		ghost[i].draw(sp);
	}
	//END ghosts

	
	//BEGIN MAP
	M = glm::mat4(1.0f);

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, 0);
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, 0);
	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, 0);
	
	glUniform1i(sp->u("textureMap0"), 2);
	glActiveTexture(GL_TEXTURE2); //Assign texture tex0 to the 0-th texturing unit
	glBindTexture(GL_TEXTURE_2D, tex_wall);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)mapVertices.size());
	//END MAP

	int i = 0;

	for (int z = 0; z < 88; z++) {
		for (int x = 0; x < 70; x++) {
			if (map[z][x] > 2 && map[z][x] < 6) {
				
				//lightSources.push_back(glm::vec4(x * (-0.2f), 1.0f, z * (-0.2f), 0.0f));
				lightSources[i] = glm::vec4(x * (-0.2f), 1.0f, z * (-0.2f), 0.0f);
				i++;
				
				M = glm::mat4(1.0f);

				M = glm::translate(M, glm::vec3(x * -0.2f, 0.0f, z * -0.2f));

				if (map[z][x] != 3 && map[z][x] != 5) {
					M = glm::scale(M, glm::vec3(2.5f, 2.5f, 2.5f));
				}

				glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

				glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

				glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, 0);
				//glEnableVertexAttribArray(sp->a("vertex"));

				glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

				glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, 0);
				//glEnableVertexAttribArray(sp->a("normal"));

				glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);

				glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, 0);
				//glEnableVertexAttribArray(sp->a("texCoord0"));
				glUniform1i(sp->u("textureMap0"), 2);
				glActiveTexture(GL_TEXTURE2); //Assign texture tex0 to the 0-th texturing unit
				glBindTexture(GL_TEXTURE_2D, tex_candy);

				glDrawArrays(GL_TRIANGLES, 0, (GLsizei)candyVertices.size());
			}
		}
	}

	glUniform1i(sp->u("numberOfLights"), i);

	glDisableVertexAttribArray(sp->a("vertex")); //Disable sending data to the attribute vertex
	glDisableVertexAttribArray(sp->a("texCoord0")); //Disable sending data to the attribute texCoord0
	glDisableVertexAttribArray(sp->a("normal")); //Disable sending data to the attribute normal

	glfwSwapBuffers(window); //Copy back buffer to the front buffer
}

void resetGame(GLFWwindow* window) {
	player.reset();

	ghost[0].reset(34.0f * -0.2f, 30.0f * -0.2f);
	ghost[1].reset(34.0f * -0.2f, 39.0f * -0.2f);
	ghost[2].reset(30.0f * -0.2f, 39.0f * -0.2f);
	ghost[3].reset(38.0f * -0.2f, 39.0f * -0.2f);

	ghost[0].left = true;
	ghost[1].up = true;
	ghost[2].right = true;
	ghost[3].left = true;
}

void restartGame(GLFWwindow* window) {
	prepereMap();

	player.reset();

	ghost[0].reset(34.0f * -0.2f, 30.0f * -0.2f);
	ghost[1].reset(34.0f * -0.2f, 39.0f * -0.2f);
	ghost[2].reset(30.0f * -0.2f, 39.0f * -0.2f);
	ghost[3].reset(38.0f * -0.2f, 39.0f * -0.2f);

	ghost[0].left = true;
	ghost[1].up = true;
	ghost[2].right = true;
	ghost[3].left = true;

	glUniform1i(sp->u("numberOfLights"), 210);

	drawScene(window);

	PlaySound(TEXT("intro.wav"), NULL, SND_SYNC | SND_FILENAME);
}

void CALLBACK end_chase(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) {

	player.chase = false;

	for (int i = 0; i < 4; i++) {
		ghost[i].end_run_away();
	}

	KillTimer(NULL, id_start_chase);
	PlaySound(nullptr, nullptr, 0);
}

int main(void)
{
	GLFWwindow* window; //Pointer to object that represents the application window

	glfwSetErrorCallback(error_callback);//Register error processing callback procedure

	if (!glfwInit()) { //Initialize GLFW library
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(800, 800, "PAC-MAN", NULL, NULL);  //Create a window 500pxx500px titled "OpenGL" and an OpenGL context associated with it. 

	if (!window) //If no window is opened then close the program
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Since this moment OpenGL context corresponding to the window is active and all OpenGL calls will refer to this context.
	glfwSwapInterval(1); //During vsync wait for the first refresh

	GLenum err;
	if ((err = glewInit()) != GLEW_OK) { //Initialize GLEW library
		fprintf(stderr, "Can't initialize GLEW: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Call initialization procedure

	//Main application loop

	resetGame(window);

	glUniform1i(sp->u("numberOfLights"), 210);
	drawScene(window);

	printf("%d\n", PlaySound(TEXT("intro.wav"), NULL, SND_SYNC | SND_FILENAME));
	
	glfwSetTime(0); //clear internal timer
	while (!glfwWindowShouldClose(window)) //As long as the window shouldnt be closed yet...
	{
		player.collision(map, lightSources);
		player.move();

		
		for (int i = 0; i < 4; i++) {
			ghost[i].collision(map, player.px_on_map, player.pz_on_map);
			ghost[i].move();

			if (ghost[i].pz_on_map == player.pz_on_map && (ghost[i].px_on_map + 1 == player.px_on_map || ghost[i].px_on_map - 1 == player.px_on_map))
			{
				if (ghost[i].run_away) {
					ghost[i].reset(34.0f * -0.2f, 39.0f * -0.2f);
				}
				else {
					player.lives--;
					
					PlaySound(TEXT("death.wav"), NULL, SND_SYNC | SND_FILENAME);

					if (player.lives == 0) {
						restartGame(window);
						break;
					}
					resetGame(window);
					
					break;
				}
				
			}
			else if ((ghost[i].pz_on_map + 1 == player.pz_on_map || ghost[i].pz_on_map - 1 == player.pz_on_map) && ghost[i].px_on_map == player.px_on_map)
			{
				if (ghost[i].run_away) {
					ghost[i].reset(34.0f * -0.2f, 39.0f * -0.2f);
				}
				else {
					player.lives--;

					PlaySound(TEXT("death.wav"), NULL, SND_SYNC | SND_FILENAME);
					
					if (player.lives == 0) {
						restartGame(window);
						break;
					}
					resetGame(window);
					
					break;
				}
			}
		}

		if (player.power_up == true){
			KillTimer(NULL, id_start_chase);
			id_start_chase = SetTimer(NULL, 0, 10000, (TIMERPROC)&end_chase);
			player.power_up = false;

			PlaySound(nullptr, nullptr, 0);
			PlaySound(TEXT("pacman_chase.wav"), NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);

			for (int i = 0; i < 4; i++) {
				ghost[i].start_run_away();
			}
		}

		if (player.score == 210) {
			PlaySound(TEXT("sound_of_glory.wav"), NULL, SND_SYNC | SND_FILENAME);
			break;
		}

		//printf("time: %.4f\n", player.start_timer);
		//printf("elapsed time: %.4f\n", glfwGetTime() - player.start_timer);
		
		glfwSetTime(0); //clear internal timer
		drawScene(window); //Execute drawing procedure
		glfwPollEvents(); //Process callback procedures corresponding to the events that took place up to now
	}
	
	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Delete OpenGL context and the window.
	glfwTerminate(); //Free GLFW resources
	exit(EXIT_SUCCESS);
}
