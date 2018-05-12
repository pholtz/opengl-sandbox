OpenGL Spaceflight Simulator
Stage 1 - Garbage Detail

## Dependencies
* glew
* glfw
* glm
* soil
* freetype (2.8.1)
	* libpng
	* harfbuzz
	* zlib
* assimp

## TODO
* Calculate bounding spheres for all entities on load
* Octree data structure for faster collision detection
	* Tree can manage universe zones as well
		* Print zones in top right corner to leave bottom for hud
	* Use tree for frustum culling
* Post processing shake on player collision
* Mouselook sinusoid
* Make a menu with separate 3D text objects and selectors
* Enable backface culling

## Attribution
* Special thanks goes to Joey DeVries and the resources at [Learn OpenGL](https://learnopengl.com)
