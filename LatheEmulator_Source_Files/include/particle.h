#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#include "shader.h"
#include "model.h"
#include "cylinder.h"
#include <math.h>

using namespace std;

struct Particle {
public:
	bool active;
	glm::vec3 scale;

	glm::vec3 position;

	glm::vec3 direction;

	// the velocity of particle shooting
	// the velocity of particle dieing (together with lifetime)
	float velocity;

	float speed;

	float lifetime;
	glm::vec3 color;

	Particle(bool a, glm::vec3 p, glm::vec3 s, glm::vec3 d, float v, float l, glm::vec3 c)
	{
		active = a;
		position = p;
		scale = s;
		direction = d;
		velocity = v;
		lifetime = l;
		color = c;
	}
};

class ParticleSystem {
public:
	// X is the knife position
	// Y is the largest slice in range (still knife position, but currently not fixed)
	glm::vec3 position;

	float gravity = -9.8f;

	int particleNum = 200;
	
	vector<Particle> particles;

	bool spawn = false; // Knife is cutting
	bool hold = true; // Neither left nor right
	bool direction = false; // Left is false, right is true

	ParticleSystem(Knife knife, int type) {
		position = knife.position;
		position.y -= 0.5f;

		for (int i = 0; i < particleNum; i++)
		{
			float scaleX = 0.01f;
			float scaleY = float(rand() % 300) / 10000.0f;
			float scaleZ = scaleY;
			glm::vec3 scale(scaleX, scaleY, scaleZ);

			float particleDirX = float((rand() % 50) - 26.0f) * 10.0f;
			particleDirX *= direction ? 1.0f : -1.0f;
			float particleDirY = float((rand() % 50) - 26.0f) * 10.0f;
			float particleDirZ = float((rand() % 50) - 26.0f) * 10.0f;
			glm::vec3 particleDir = normalized3D(glm::vec3(particleDirX, particleDirY, particleDirZ));

			float velocity = float(rand() * 0.005);
			float lifetime = float(rand() * 0.00001);
			glm::vec3 color = normalized3D(glm::vec3(1.0f, 1.0f, 1.0f));

			particles.push_back(Particle(true, position, scale, particleDir, velocity, lifetime, color));
		}
	}

	glm::vec3 normalized3D(glm::vec3 pos)
	{
		glm::vec3 temp;
		
		temp.x = (float)(pos.x / (sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)));
		temp.y = (float)(pos.y / (sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)));
		temp.z = (float)(pos.z / (sqrtf(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)));
		return temp;
	}

	void rebornParticle(int idx, glm::vec3 pos)
	{
		float scaleX = float(rand() % 300) / 7500.0f;
		float scaleY = float(rand() % 300) / 7500.0f;
		float scaleZ = scaleY;
		glm::vec3 scale(scaleX, scaleY, scaleZ);

		float particleDirX = float((rand() % 50) - 26.0f) * 10.0f * (direction ? 1.0f : -1.0f);
		float particleDirY = float((rand() % 50) - 26.0f) * 10.0f;
		float particleDirZ = float((rand() % 50) - 26.0f) * 10.0f;
		glm::vec3 particleDir = normalized3D(glm::vec3(particleDirX, particleDirY, particleDirZ));
		float velocity = float(rand() * 0.015);
		float lifetime = float(rand() * 0.00001);
		glm::vec3 color = normalized3D(glm::vec3(1.0f, 1.0f, 1.0f));


		pos.y -= 0.5f;
		particles[idx].active = true;
		particles[idx].position = pos;
		particles[idx].scale = scale;
		particles[idx].direction = particleDir;
		particles[idx].velocity = velocity;
		particles[idx].lifetime = lifetime;
		particles[idx].color = color;
	}
	
};

#endif