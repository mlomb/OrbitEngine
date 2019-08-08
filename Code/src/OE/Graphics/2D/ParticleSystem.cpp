#include "OE/Graphics/2D/ParticleSystem.hpp"

#include "OE/Graphics/2D/ParticleEmitter.hpp"
#include "OE/Graphics/2D/SpriteRenderer.hpp"

namespace OrbitEngine { namespace Graphics {
	ParticleSystem::ParticleSystem(int maxParticles)
	{
		m_Pool = new Memory::MemoryPool(sizeof(Particle), maxParticles);
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_Pool;
	}

	void ParticleSystem::update(float deltaTime)
	{
		for (ParticleEmitter* emitter : m_Emitters)
			emitter->update(deltaTime);
	}

	void ParticleSystem::render(SpriteRenderer& sr)
	{
		for (ParticleEmitter* emitter : m_Emitters) {
			for (Particle* p : emitter->m_Particles) { // should we rely on this direct accesor?
				sr.bindTexture(NULL);
				sr.bindColor(p->color);
				sr.rect(p->position, Math::Vec2f(p->size, p->size));
			}
		}
	}

	ParticleEmitter* ParticleSystem::createEmitter()
	{
		ParticleEmitter* emitter = new ParticleEmitter(this);
		m_Emitters.push_back(emitter);
		return emitter;
	}

	Particle* ParticleSystem::createParticle()
	{
		return static_cast<Particle*>(m_Pool->Allocate());
	}

	void ParticleSystem::releaseParticle(Particle* particle)
	{
		m_Pool->Deallocate(static_cast<void*>(particle));
	}
} }
