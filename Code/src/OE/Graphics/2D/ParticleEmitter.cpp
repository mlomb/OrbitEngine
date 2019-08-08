#include "OE/Graphics/2D/ParticleEmitter.hpp"

#include "OE/Misc/Log.hpp"

namespace OrbitEngine { namespace Graphics {

	ParticleEmitter::ParticleEmitter(ParticleSystem* system) :
		m_System(system),
		m_Time(0),
		m_EmitTimeLast(0),
		m_EmitTimeAccum(0),
		m_EmitPositionLast(Math::Vec2f()),
		m_EmitDistanceAccum(0),
		m_Duration(5),
		m_Loop(false),
		m_TimeRate(0),
		m_DistanceRate(0),
		m_SimulationSpace(SimulationSpace::LOCAL),
		m_Position(0, 0)
	{
	}

	ParticleEmitter::~ParticleEmitter()
	{
		// release all
		for (Particle* particle : m_Particles)
			m_System->releaseParticle(particle);
		m_Particles.clear();
	}

	void ParticleEmitter::update(float deltaTime)
	{
		m_Time += deltaTime;
		if (m_Loop) {
			m_Time = fmod(m_Time, m_Duration);
			m_EmitTimeLast = 0; // TODO: Fix
		}

		if (m_Time < m_Duration)
			emit();
	}

	void ParticleEmitter::initParticle(Particle* particle)
	{
		particle->position = m_Position;
		particle->color = Math::Color(1, 1, 1, 1);
		particle->size = 10;
	}

	void ParticleEmitter::emit()
	{
		m_EmitTimeAccum += m_Time - m_EmitTimeLast;
		m_EmitTimeLast = m_Time;

		m_EmitDistanceAccum += m_Position.distanceTo(m_EmitPositionLast);
		m_EmitPositionLast = m_Position;

		int to_emit = 0;

		if (m_TimeRate > 0) {
			const float timeRate = 1.0f / m_TimeRate;

			to_emit += (int)(m_EmitTimeAccum / timeRate);
			m_EmitTimeAccum = fmod(m_EmitTimeAccum, timeRate);
		}
		if (m_DistanceRate > 0) {
			const float distRate = 1.0f / m_DistanceRate;

			to_emit += (int)(m_EmitDistanceAccum / distRate);
			m_EmitDistanceAccum = fmod(m_EmitDistanceAccum, distRate);
		}

		OE_ASSERT(to_emit >= 0);

		for (int i = 0; i < to_emit; i++) {
			Particle* p = m_System->createParticle();
			if (p) {
				initParticle(p);
				m_Particles.push_back(p);
			}
		}
	}

	void ParticleEmitter::setPosition(const Math::Vec2f& position) { m_Position = position; }
	void ParticleEmitter::setSimulationSpace(SimulationSpace simulationSpace) { m_SimulationSpace = simulationSpace; }
	void ParticleEmitter::setTimeRate(float timeRate) { m_TimeRate = timeRate; }
	void ParticleEmitter::setDistanceRate(float distanceRate) { m_DistanceRate = distanceRate; }
	void ParticleEmitter::setLoop(bool loop) { m_Loop = loop; }
	void ParticleEmitter::setDuration(float duration) { m_Duration = duration; }
} }
