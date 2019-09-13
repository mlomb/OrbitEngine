#include "OE/Graphics/2D/ParticleEmitter.hpp"

#include "OE/Graphics/2D/ParticleModules.hpp"
#include "OE/Graphics/2D/SpriteRenderer.hpp"

namespace OrbitEngine { namespace Graphics {

	ParticleEmitter::ParticleEmitter(ParticleSystem* system) :
		m_System(system),
		m_Time(0),
		m_Duration(5),
		m_Loop(false),
		m_SimulationSpace(SimulationSpace::LOCAL),
		m_Position(0, 0),
		m_InitialLifetime(5.0f),

		// Modules
		emissionModule(this),
		shapeModule(this),
		sizeModule(this),
		colorModule(this),
		textureModule(this)
	{
		m_Modules.push_back(&emissionModule);
		m_Modules.push_back(&shapeModule);
		m_Modules.push_back(&sizeModule);
		m_Modules.push_back(&colorModule);
		m_Modules.push_back(&textureModule);

		m_System->registerEmitter(this);
	}

	ParticleEmitter::~ParticleEmitter()
	{
		// release all
		for (Particle* particle : m_Particles)
			m_System->releaseParticle(particle);
		m_Particles.clear();

		m_System->unregisterEmitter(this);
	}

	void ParticleEmitter::update(float deltaTime)
	{
		m_Time += deltaTime;
		if (m_Loop && m_Time >= m_Duration)
			m_Time = fmod(m_Time, m_Duration);

		// update lifetime & release particles
		auto it = m_Particles.begin();
		while(it != m_Particles.end()) {
			Particle* p = *it;
			p->time -= deltaTime * (1.0f / p->life);

			if (p->time <= 0.0f) {
				m_System->releaseParticle(p);
				it = m_Particles.erase(it);
			}
			else
				++it;
		}

		// update modules
		for (ParticleModule* module : m_Modules)
			module->update(deltaTime);
	}

	void ParticleEmitter::render(SpriteRenderer& sr)
	{
		sr.resetTransform();

		for (Particle* p : m_Particles) { // should we rely on this direct accesor?
			Math::Vec2f pos, size;
			size.x = p->size * p->stretch;
			size.y = p->size;
			pos = p->position;

			if (m_SimulationSpace == SimulationSpace::LOCAL)
				pos += m_Position;

			sr.setTransform(Math::Mat4::Rotation(p->velocity.angle(), Math::Vec3f(0, 0, -1)) * Math::Mat4::Translation(pos));
			sr.bindTexture(p->texture);
			sr.bindColor(p->color);
			sr.rect(size * -0.5f, size);
		}
	}

	void ParticleEmitter::emit(unsigned int particles)
	{
		for (int i = 0; i < particles; i++) {
			Particle* p = m_System->createParticle();
			if (p) {
				if (m_SimulationSpace == SimulationSpace::WORLD)
					p->position = m_Position;
				p->life = m_InitialLifetime;
				p->time = 1.0f;

				for (ParticleModule* module : m_Modules)
					module->initParticle(p);
				m_Particles.push_back(p);
			}
		}
	}

	void ParticleEmitter::setInitialLifetime(float lifetime)
	{
		m_InitialLifetime = lifetime;
	}

	void ParticleEmitter::setPosition(const Math::Vec2f& position) { m_Position = position; }
	void ParticleEmitter::setSimulationSpace(SimulationSpace simulationSpace) { m_SimulationSpace = simulationSpace; }
	void ParticleEmitter::setLoop(bool loop) { m_Loop = loop; }
	void ParticleEmitter::setDuration(float duration) { m_Duration = duration; }
} }
