#include "physics.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace physics {
	// Update velocity independently of time and account for friction
	glm::vec3 updateVelocity(glm::vec3 velocity, glm::vec3 acceleration, float dt) {
		glm::vec3 updatedVelocity = glm::vec3(velocity);
		updatedVelocity = updatedVelocity + (acceleration * dt);
		updatedVelocity = updatedVelocity * FRICTION;
		return updatedVelocity;
	}

	// Update acceleration based on desired rate and direction
	glm::vec3 updateAcceleration(glm::vec3 acceleration, glm::vec3 direction, float rate) {
		glm::vec3 updatedAcceleration = glm::vec3(acceleration);
		updatedAcceleration = updatedAcceleration + (glm::normalize(direction) * rate);
		return updatedAcceleration;
	}

	glm::vec3 capVelocity(glm::vec3 velocity) {
		glm::vec3 cappedVelocity = glm::vec3(velocity);
		// Zero out velocity once it's under the min delta
		if(abs(cappedVelocity[0]) < DELTA_LIMIT) {
			cappedVelocity[0] = 0.0f;
		}
		if(abs(cappedVelocity[1]) < DELTA_LIMIT) {
			cappedVelocity[1] = 0.0f;
		}
		if(abs(cappedVelocity[2]) < DELTA_LIMIT) {
			cappedVelocity[2] = 0.0f;
		}

		// Cap velocity in both directions along each axis
		if(abs(cappedVelocity[0]) > MAX_VELOCITY) {
			if(cappedVelocity[0] > 0) {
				cappedVelocity[0] = MAX_VELOCITY;
			} else {
				cappedVelocity[0] = MAX_VELOCITY * -1.0f;
			}
		}
		if(abs(cappedVelocity[1]) > MAX_VELOCITY) {
			if(cappedVelocity[1] > 0) {
				cappedVelocity[1] = MAX_VELOCITY;
			} else {
				cappedVelocity[1] = MAX_VELOCITY * -1.0f;
			}
		}
		if(abs(cappedVelocity[2]) > MAX_VELOCITY) {
			if(cappedVelocity[2] > 0) {
				cappedVelocity[2] = MAX_VELOCITY;
			} else {
				cappedVelocity[2] = MAX_VELOCITY * -1.0f;
			}
		}
		return cappedVelocity;
	}

	// Camera matrix: Build a matrix to transform coordinates relative to camera location
	glm::mat4 calculateViewMatrix(glm::vec3 position, glm::vec3 orientation, glm::vec3 up) {
		glm::mat4 view;
		view = glm::lookAt(position, position + orientation, up);

	    /*
	    glm::mat4 viewRotation;
	    viewRotation = glm::toMat4(entity->pitchQuat * entity->yawQuat * entity->rollQuat);
	    glm::mat4 viewTranslation;
	    viewTranslation = glm::translate(viewTranslation, entity->position);
	    view = viewTranslation * viewRotation;
	    */

	    /*
	    float cosPitch = glm::cos(glm::radians(entity->pitchAngle));
	    float sinPitch = glm::sin(glm::radians(entity->pitchAngle));
	    float cosYaw = glm::cos(glm::radians(entity->yawAngle));
	    float sinYaw = glm::sin(glm::radians(entity->yawAngle));

	    glm::vec3 xAxis = glm::vec3(cosYaw, 0.0f, -sinYaw);
	    glm::vec3 yAxis = glm::vec3(sinYaw * sinPitch, cosPitch, cosYaw * sinPitch);
	    glm::vec3 zAxis = glm::vec3(sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw);

	    view[0] = glm::vec4(xAxis, -glm::dot(xAxis, entity->position));
	    view[1] = glm::vec4(yAxis, -glm::dot(yAxis, entity->position));
	    view[2] = glm::vec4(zAxis, -glm::dot(zAxis, entity->position));
	    view[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	    */

		return view;
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 calculateProjectionMatrix(float fovDegrees, float ratio, float near, float far) {
		glm::mat4 projection;
	    projection = glm::perspective(glm::radians(fovDegrees), ratio, near, far);
	    return projection;
	}
}