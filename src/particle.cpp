#include "Particle.hpp"

//Perlin imports
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

//Particle constructor
Particle::Particle(glm::vec3 l, float ls) {
	origin = l;
	location = l;
	height = ls;
	//startLifespan = ls;
	//lifespan = ls;
}

//update particle
void Particle::update()
{	
	// perlin noise for N(p(t) - kTj) (with random constant [0, 1] k)
	glm::vec3 scaledDown = location - (glm::vec3(getRandom(0, 1), getRandom(0, 1), getRandom(0, 1)) * lifespan * gravity); 
	float N = stb_perlin_noise3(scaledDown.x, scaledDown.y, scaledDown.z); 

	float windL = lrg_wind;
	float C = height;
	float windS = dot((location - origin), glm::vec3(-gravity)) * C * N;
	float k = wind;
	float deltaV = (k * gravity) + windL + windS;
	//float deltaV = windS;

	glm::vec3 scaledDownZ = location - (glm::vec3(getRandom(0, 1), getRandom(0, 1), getRandom(0, 1)) * lifespan * gravity);
	float NZ = stb_perlin_noise3(scaledDownZ.x, scaledDownZ.y, scaledDownZ.z);

	float windLZ = lrg_wind;
	float CZ = C;
	float windSZ = dot((location - origin), glm::vec3(-gravity)) * CZ * NZ;
	float kZ = wind;
	float deltaVZ = (k * gravity) + windLZ + windSZ;
	//float deltaVZ = N;

	acceleration = glm::vec3(deltaV, gravity, deltaVZ);
	acceleration = normalize(acceleration);
	velocity += acceleration;
	velocity = normalize(velocity);
	float velCap = 7.5f - (scale_f * 7.5f);
	velocity /= velCap; //adjust according to scale
	location += velocity;

	/*if (origLifespan != startLifespan) {
		lifespan += abs(origLifespan - startLifespan);
		origLifespan = startLifespan;
	}*/
	lifespan--;
}

//draw particle
void Particle::display(glm::mat4 view, glm::mat4 proj)
{
	glm::mat4 translated = translate(view, location);

	// opacity
	if (alpha) {
		color.a = (lifespan / startLifespan); 
		emColor.a = color.a;
		amColor.a = color.a;
		diffColor.a = color.a;
		specColor.a = color.a;
	}

	emColor.x = (lifespan / startLifespan);
	emColor.y = (lifespan / startLifespan) / 2;

	float scaleX = 0.67 + 0.4 * sin(3.14 * ((lifespan / startLifespan) / 24));
	float scaleY = (lifespan / startLifespan);// *0.032;
	scaleX *= scale_f; //scale down further 
	scaleY *= scale_f;
	translated = scale(translated, glm::vec3(scaleX, scaleY, scale_f));

	glUseProgram(shader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(shader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(shader, "uModelViewMatrix"), 1, false, value_ptr(translated));
	glUniform4fv(glGetUniformLocation(shader, "baseColor"), 1, value_ptr(color));
	glUniform4fv(glGetUniformLocation(shader, "emColor"), 1, value_ptr(emColor));
	glUniform4fv(glGetUniformLocation(shader, "amColor"), 1, value_ptr(amColor));
	glUniform4fv(glGetUniformLocation(shader, "diffColor"), 1, value_ptr(diffColor));
	glUniform4fv(glGetUniformLocation(shader, "specColor"), 1, value_ptr(specColor));
	glUniform1f(glGetUniformLocation(shader, "shininess"), shine);

	cgra::drawSphere();
}

//check if particle is dead
bool Particle::isDead() {
	if (lifespan <= 0) {
		return true;
	}
	else {
		return false;
	}
}

//Helper method to get random float in range 
float Particle::getRandom(float low, float high)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(low, high);
	return dis(e);
}