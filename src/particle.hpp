#pragma once

#include <iostream>

#include <glm/glm.hpp>
#include "opengl.hpp"
#include <cgra/cgra_geometry.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>


class Particle
{
public:
	Particle(glm::vec3 l, float lifespan);

	GLuint shader = 0;

	float gravity;
	float yVelocity;

	void update();
	void display(glm::mat4 view, glm::mat4 proj);
	bool isDead();
	float getRandom(float low, float high);

	glm::vec3 origin{ 0.0 };
	glm::vec3 location{ 0.0 };
	glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 acceleration = glm::vec3(0.0, 0.0, 0.0);
	
	//colours for shader
	//39, 110, 5 
	glm::vec4 color{ 0.2295f, 0.08825f, 0.0275f, 1.0f }; //0.2295f, 0.08825f, 0.0275f, 1.0f
	glm::vec4 emColor{0.2295f, 0.08825f, 0.0275f, 1.0f };
	glm::vec4 amColor{ 0.2295f, 0.08825f, 0.0275f, 1.0f };
	glm::vec4 diffColor{ 0.5508f, 0.2118f, 0.066f, 1.0f };
	glm::vec4 specColor{ 0.580594f, 0.223257f, 0.0695701f, 1.0f };
	float shine = 0.7f;

	float origLifespan = 50.0f;
	float startLifespan = 50.0f;
	float lifespan = startLifespan;

	float mass = 1.0f;
	float scale_f = 0.2;
	float wind = 0.04;
	float lrg_wind = 0.0;
	float height = 5.0;
	bool alpha = false;
};