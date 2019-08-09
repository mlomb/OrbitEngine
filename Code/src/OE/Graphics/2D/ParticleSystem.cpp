#include "OE/Graphics/2D/ParticleSystem.hpp"

#include "OE/Graphics/2D/ParticleEmitter.hpp"

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
		for (ParticleEmitter* emitter : m_Emitters)
			emitter->render(sr);
	}

	Particle* ParticleSystem::createParticle()
	{
		return static_cast<Particle*>(m_Pool->Allocate());
	}

	void ParticleSystem::releaseParticle(Particle* particle)
	{
		m_Pool->Deallocate(static_cast<void*>(particle));
	}

	void ParticleSystem::registerEmitter(ParticleEmitter* emitter)
	{
		m_Emitters.push_back(emitter);
	}

	void ParticleSystem::unregisterEmitter(ParticleEmitter* emitter)
	{
		m_Emitters.erase(std::find(m_Emitters.begin(), m_Emitters.end(), emitter));
	}
} }
