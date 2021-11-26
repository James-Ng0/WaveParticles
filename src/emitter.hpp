#pragma once

#include <iostream>

#include "particle.hpp"
#include <vector>
#include <glm/glm.hpp>

class Emitter {
public:
	std::vector<Particle> particles;
	glm::vec3 origin;
	GLuint shader = 0;
	float gravity = 0.1;

	Emitter(glm::vec3 location);
	void addParticle();
	void update();
	void draw(glm::mat4 view, glm::mat4 proj);
	float getRandom(float low, float high);

	float scale = 0.2;
	float wind = 0.04;
	float lrg_wind = 0.0;
	float fire_height = 5.0;
	bool alpha = false;
};