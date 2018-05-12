#ifndef ENTITY_H
#define ENTITY_H
#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


class Entity
{
public:
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::mat4 rotationMatrix;
	glm::quat pitchQuat;
	glm::quat yawQuat;
	glm::quat rollQuat;
	float pitchAngle;
	float yawAngle;
	float rollAngle;
	glm::vec3 pitchAxis;
	Entity(glm::vec3 position, glm::vec3 orientation);
	~Entity();
	void updatePosition(GLfloat dt);
	void movePositionForward(GLfloat dt);
	void movePositionBackward(GLfloat dt);
	void movePositionLeft(GLfloat dt);
	void movePositionRight(GLfloat dt);
	void increasePitch(GLfloat dt);
	void decreasePitch(GLfloat dt);
	void increaseYaw(GLfloat dt);
	void decreaseYaw(GLfloat dt);
	void increaseRoll(GLfloat dt);
	void decreaseRoll(GLfloat dt);
	void setAcceleration(glm::vec3 acceleration);
	void reset();
};
#endif