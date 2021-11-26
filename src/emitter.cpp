#include "emitter.hpp"

//Emitter constructor
Emitter::Emitter(glm::vec3 location) {
	origin = location;
}

//add new particle, add origin with random (small) variation
void Emitter::addParticle() {
	glm::vec3 start = glm::vec3(origin.x + getRandom(0, 0.5), origin.y, origin.z + getRandom(0, 0.5));
	Particle p = Particle(start, fire_height);
	p.shader = shader;
	p.gravity = gravity;
	p.scale_f = scale;
	p.wind = wind;
	p.lrg_wind = lrg_wind;
	p.alpha = alpha;
	//p.startLifespan = fire_height;
	particles.push_back(p);
}

//update each particle
void Emitter::update() {
	if (particles.size() < 500) {
		addParticle();
	}

	std::vector<int> toRemove; ///vector of indices to remove 
	for (int i = 0; i < particles.size(); i++) {
		Particle &p = particles.at(i);
		p.update(); 
		if (p.alpha != alpha) { p.alpha = alpha; }
		if (p.isDead()) {
			toRemove.push_back(i);
		}
	}

	//remove dead particles
	for (int i = toRemove.size() - 1; i >= 0; i--) {
		particles.erase(particles.begin() + toRemove.at(i));
	}
}

//draw all particles
void Emitter::draw(glm::mat4 view, glm::mat4 proj) {
	for (int i = 0; i < particles.size(); i++) {
		Particle &p = particles.at(i);
		p.display(view, proj);
	}	
}

//Helper method to get random float in range
float Emitter::getRandom(float low, float high)
{
	static std::default_random_engine e;
	static std::uniform_real_distribution<> dis(low, high);
	return dis(e);
}