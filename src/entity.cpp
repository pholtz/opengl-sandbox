#include "entity.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <math.h>
#include "physics.hpp"

Entity::Entity(glm::vec3 position, glm::vec3 orientation) {
	this->position = position;
	this->orientation = orientation;
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->rollAngle = 0.0f;
	this->pitchAngle = 0.0f;
	this->yawAngle = 0.0f;
}
Entity::~Entity() {}

const float ACCELERATION_METERS_PER_SECOND = 1.0f;

// Apply either zero or constant acceleration to the entity's current velocity, accounting for friction
void Entity::updatePosition(GLfloat dt) {
	this->velocity = physics::updateVelocity(this->velocity, this->acceleration, dt);
	this->acceleration = glm::vec3(0.0f);
	this->velocity = physics::capVelocity(this->velocity);
	this->position = this->position + this->velocity;
}

void Entity::movePositionForward(GLfloat dt) {
	this->acceleration = physics::updateAcceleration(this->acceleration, this->orientation, ACCELERATION_METERS_PER_SECOND);
}

void Entity::movePositionBackward(GLfloat dt) {
	this->acceleration = physics::updateAcceleration(this->acceleration, -this->orientation, ACCELERATION_METERS_PER_SECOND);
}

void Entity::movePositionLeft(GLfloat dt) {
	this->acceleration = physics::updateAcceleration(this->acceleration, -glm::cross(this->orientation, this->up), ACCELERATION_METERS_PER_SECOND);
}

void Entity::movePositionRight(GLfloat dt) {
	this->acceleration = physics::updateAcceleration(this->acceleration, glm::cross(this->orientation, this->up), ACCELERATION_METERS_PER_SECOND);
}

/*
 * Pretty ok vector component visualization
 *            /|
 *           / |
 *          /  |
 *         /   | y
 *        /    |
 *       /   |-|
 * pitch -------
 *        x / z
 *
 * tan(pitch) = y / x
 * pitch = tan-1(y / x)
 */
void Entity::increasePitch(GLfloat dt) {    // Move nose up
	//if(this->pitchAngle > -75.0f) {
		this->pitchAxis = glm::cross(this->orientation, this->up);
		this->orientation = glm::normalize(glm::rotate(this->orientation, 0.0025f, this->pitchAxis));
	//}
	// Recalculate pitch angle for reporting
	float verticalComponent = this->orientation[1];
	float horizontalComponent = sqrt(pow(this->orientation[0], 2) + pow(this->orientation[2], 2));
	this->pitchAngle = glm::degrees(glm::atan(verticalComponent, horizontalComponent));
}

void Entity::decreasePitch(GLfloat dt) {    // Move nose down
	this->pitchAngle = glm::degrees(glm::atan(this->orientation[2], this->orientation[1])) + 90;
	//if(this->pitchAngle < 75.0f) {
		this->pitchAxis = glm::cross(this->orientation, this->up);
		this->orientation = glm::normalize(glm::rotate(this->orientation, -0.0025f, this->pitchAxis));
	//}
	// Recalculate pitch angle for reporting
	float verticalComponent = this->orientation[1];
	float horizontalComponent = sqrt(pow(this->orientation[0], 2) + pow(this->orientation[2], 2));
	this->pitchAngle = glm::degrees(glm::atan(verticalComponent, horizontalComponent));
}

void Entity::increaseYaw(GLfloat dt) {  // Move nose right
	this->orientation = glm::rotate(this->orientation, -0.0025f, this->up);
}

void Entity::decreaseYaw(GLfloat dt) {  // Move nose left
	this->orientation = glm::rotate(this->orientation, 0.0025f, this->up);
}

void Entity::increaseRoll(GLfloat dt) { // Roll clockwise
	this->up = glm::rotate(this->up, 0.0025f, this->orientation);
}

void Entity::decreaseRoll(GLfloat dt) { // Roll counter-clockwise
	this->up = glm::rotate(this->up, -0.0025f, this->orientation);
}

void Entity::setAcceleration(glm::vec3 acceleration) {
	this->acceleration = glm::vec3(acceleration);
}

void Entity::reset() {
	this->position = glm::vec3(0.0f, 0.0f, 10.0f);
	this->orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);
	this->velocity = glm::vec3(0.0f);
	this->acceleration = glm::vec3(0.0f);
}
