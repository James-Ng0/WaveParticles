#include "particle_system.hpp"

//default constructor
ParticleSystem::ParticleSystem() {}

//constructor
ParticleSystem::ParticleSystem(GLuint shader)
{
	//generates [density] number of particle systems in range (0, [radius])
	while (systems.size() < density) {
		systems.push_back(Emitter(glm::vec3(glm::vec3(getRandom(x - radius, x + radius), y - 0.5, getRandom(z - radius, z)))));
		//systems.push_back(Emitter(glm::vec3(x, y, z)));
	}

	//adds one emitter to each particle system
	for (int i = 0; i < systems.size(); i++) {
		Emitter& e = systems.at(i);
		e.shader = shader;
		e.scale = scale_f;
		e.wind = wind;
		e.addParticle();
	}
	//sets shader
	this->shader = shader;
}

//updates all emitters in all particle systems
void ParticleSystem::update() {
	//update all emitters
	for (int i = 0; i < systems.size(); i++) {
		systems.at(i).update();
	}

	//if emitter is empty, remove it
	std::vector<int> toRemove;
	for (int i = 0; i < systems.size(); i++) {
		if (systems.at(i).particles.size() == 0) {
			toRemove.push_back(i);
		}
	}
	for (int i = toRemove.size() - 1; i >= 0; i--) {
		systems.erase(systems.begin() + toRemove.at(i));
	}

	//generate new emitters
	while (systems.size() < density) {
		Emitter newEm = Emitter(glm::vec3(getRandom(x - radius, x + radius), y - 0.5, getRandom(z - radius, z)));
		//Emitter newEm = Emitter(glm::vec3(x, y, z));

		newEm.shader = shader;
		newEm.addParticle();
		systems.push_back(newEm);
	}

	//on radius increase, reduce density to allow generation in wider range
	if (prevRadius != radius) {
		density /= 2;
		prevRadius = radius;
	}

	//on change, reduce number of systems according to density
	if (systems.size() > density) {
		for (int i = density; i < systems.size(); i++) {
			systems.erase(systems.begin() + i);
		}
	}
	
	//on change, reduce radius of systems
	for (int i = 0; i < systems.size(); i++) {
		if (systems.at(i).origin.x > radius || systems.at(i).origin.z > radius) {
			systems.erase(systems.begin() + i);
			prevRadius = radius;
		}
	}
	
}

//draw all emitters in particle system
void ParticleSystem::draw(glm::mat4 view, glm::mat4 proj) {
	float scalar = 3;

	glm::mat4 translated = translate(view, glm::vec3(x, y, z - (2.5 * scalar)));
	translated = scale(translated, glm::vec3(0.5 * scalar, 0.5 * scalar, 5.0 * scalar));

	glUseProgram(logShader); // load shader and variables
	glUniformMatrix4fv(glGetUniformLocation(logShader, "uProjectionMatrix"), 1, false, value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(logShader, "uModelViewMatrix"), 1, false, value_ptr(translated));
	glUniform3fv(glGetUniformLocation(logShader, "uColor"), 1, value_ptr(glm::vec3(0.388, 0.192, 0)));
	cgra::drawCylinder();
	
	//translate, rotate, scale second log to be leaning on other 
	glm::mat4 translated2 = translate(view, glm::vec3(x - (2.8 * scalar), y - (0.3 * scalar), z));
	translated2 = rotate(translated2, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	translated2 = rotate(translated2, glm::radians(-20.0f), glm::vec3(1.0, 0.0, 0.0));
	translated2 = scale(translated2, glm::vec3(0.5 * scalar, 0.5 * scalar, 5.0 * scalar));

	glUniformMatrix4fv(glGetUniformLocation(logShader, "uModelViewMatrix"), 1, false, value_ptr(translated2));
	cgra::drawCylinder();

	for (int i = 0; i < systems.size(); i++) {
		systems.at(i).draw(view, proj);
	}
}

//Helper method to get random number in range
float ParticleSystem::getRandom(float low, float high)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(low, high);
	return dis(e);
}

//Helper method to set parameters from application.cpp
void ParticleSystem::parameters(float radius, float wind, float density, float scale, float lrg_wind, float fire_height, bool alpha)
{
	this->radius = radius;
	this->wind = wind;
	this->density = density;
	this->scale_f = scale;

	//update values in emitters
	for (int i = 0; i < systems.size(); i++) {
		Emitter& e = systems.at(i);
		e.scale = scale;
		e.wind = wind;
		e.lrg_wind = lrg_wind;
		e.fire_height = fire_height;
		e.alpha = alpha;
	}
}
