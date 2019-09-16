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
		auto it = m_Emitters.begin();
		while (it != m_Emitters.end()) {
			ParticleEmitter* emitter = *it;
			if (emitter->m_ShouldBeDeleted && emitter->m_Time >= emitter->m_Duration && emitter->m_Particles.size() == 0) {
				delete emitter;
				it = m_Emitters.erase(it);
			}
			else {
				emitter->update(deltaTime);
				++it;
			}
		}
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
