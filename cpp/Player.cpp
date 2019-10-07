#include "Player.h"


void Player::computeRightUp() {
	right = Point(-direction.y, direction.x, 0);
	right.normalize();
	up = Point(&right);
	up.crossProduct(&direction);
}

void Player::copy(const Player* player) {
	direction.copy(&player->direction);
	position.copy(&player->position);
	right.copy(&player->right);
	up.copy(&player->up);
}

void Player::moveTo(real x, real y, real z) {
	position = Point(x, y, z);
}

void Player::moveTo(const Point* position) {
	this->position.copy(position);
}

void Player::lookAt(real x,real y, real z) {
	direction = Point(x, y, z);
	direction.substract(&position);
	direction.normalize();
	computeRightUp();
}

void Player::lookAt(const Point* point) {
	direction.copy(point);
	direction.substract(&position);
	direction.normalize();
	computeRightUp();
}

void Player::rotate(real t1, real t2) {
}

void Player::rotateAroundZ(real t) {
	real s = sin(t);
	real c = cos(t);
	direction = Point(direction.x * c 
			- direction.y * s,
			direction.x * s
			+ direction.y * c,
			direction.z);
	computeRightUp();
}

void Player::rotateAroundRight(real t) {
	real s = sin(t);
	real c = cos(t);
}
