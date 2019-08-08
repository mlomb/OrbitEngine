#ifndef GRAPHICS_PARTICLE_SYSTEM_HPP
#define GRAPHICS_PARTICLE_SYSTEM_HPP

#include <vector>

#include "OE/Math/Vec2.hpp"
#include "OE/Math/Color.hpp"

#include "OE/Memory/MemoryPool.hpp"

namespace OrbitEngine { namespace Graphics {
	
	class SpriteRenderer;
	class ParticleEmitter;

	struct Particle {
		float remainingLife;

		float size;
		Math::Vec2f position;
		Math::Color color;

		Math::Vec2f velocity;
		Math::Vec2f acceleration;
	};

	class ParticleSystem {
	public:
		ParticleSystem(int maxParticles = 1000);
		~ParticleSystem();

		void update(float deltaTime);
		void render(SpriteRenderer& sr);

		ParticleEmitter* createEmitter();

		Particle* createParticle();
		void releaseParticle(Particle* particle);

	private:
		Memory::MemoryPool* m_Pool;
		std::vector<ParticleEmitter*> m_Emitters;
	};
} }

#endif