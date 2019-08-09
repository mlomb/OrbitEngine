#ifndef GRAPHICS_PARTICLE_SYSTEM_HPP
#define GRAPHICS_PARTICLE_SYSTEM_HPP

#include <vector>

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Color.hpp"

#include "OE/Memory/MemoryPool.hpp"

namespace OrbitEngine { namespace Graphics {
	
	class Texture;
	class SpriteRenderer;
	class ParticleEmitter;

	struct Particle {
		float life = 1.0f; // in seconds
		float time = 1.0f; // [0, 1]

		float size = 1.0f, stretch = 1.0f;
		Math::Vec2f position = Math::Vec2f(0, 0);
		Math::Color4f color = Math::Color4f(1, 1, 1, 1);
		Texture* texture = NULL;

		Math::Vec2f velocity = Math::Vec2f(0, 0);
	};

	class ParticleSystem {
	public:
		ParticleSystem(int maxParticles = 1000);
		~ParticleSystem();

		void update(float deltaTime);
		void render(SpriteRenderer& sr);

		Particle* createParticle();
		void releaseParticle(Particle* particle);

		void registerEmitter(ParticleEmitter* emitter);
		void unregisterEmitter(ParticleEmitter* emitter);

	private:
		Memory::MemoryPool* m_Pool;
		std::vector<ParticleEmitter*> m_Emitters;
	};
} }

#endif