#ifndef PHYSICS_H
#define PHYSICS_H
#include <vector>
#include <string>
#include <glm/glm.hpp>


namespace physics {
	const float MAX_VELOCITY = 2.0f;
	const float METERS_PER_FRAME = 0.1f;
	const float FRICTION = 0.99f;
	const float DELTA_LIMIT = 0.000001f;
	glm::vec3 updateVelocity(glm::vec3 velocity, glm::vec3 acceleration, float dt);
	glm::vec3 updateAcceleration(glm::vec3 acceleration, glm::vec3 direction, float rate);
	glm::vec3 capVelocity(glm::vec3 velocity);
	glm::mat4 calculateViewMatrix(glm::vec3 position, glm::vec3 orientation, glm::vec3 up);
	glm::mat4 calculateProjectionMatrix(float fovDegrees, float ratio, float near, float far);
}
#endif