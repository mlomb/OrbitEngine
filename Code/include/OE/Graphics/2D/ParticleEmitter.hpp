#ifndef GRAPHICS_PARTICLE_EMITTER_HPP
#define GRAPHICS_PARTICLE_EMITTER_HPP

#include <vector>

#include "OE/Graphics/2D/ParticleSystem.hpp"

namespace OrbitEngine { namespace Graphics {
	
	class ParticleSystem;

	enum class SimulationSpace {
		/// Particles will stay relative to the emitter
		LOCAL,
		/// Particles positions will decouple from the emitter once created
		WORLD
	};

	class ParticleEmitter {
	public:

		void setPosition(const Math::Vec2f& position);
		void setSimulationSpace(SimulationSpace simulationSpace);
		void setTimeRate(float timeRate);
		void setDistanceRate(float distanceRate);
		void setLoop(bool loop);
		void setDuration(float duration);
		
	private:
		friend class ParticleSystem;
		/// Use ParticleSystem::createEmitter to create an emitter
		ParticleEmitter(ParticleSystem* system);
		~ParticleEmitter();

		void update(float deltaTime);
		void initParticle(Particle* particle);
		void emit();

		ParticleSystem* m_System;
		std::vector<Particle*> m_Particles;

		/// Current time
		float m_Time;

		float m_EmitTimeLast;
		float m_EmitTimeAccum;
		Math::Vec2f m_EmitPositionLast;
		float m_EmitDistanceAccum;
	private:
		//////////////
		//  TIMING  //
		//////////////
		/// Length of time while emitting particles.
		/// If Loop is enabled then the duration of one cycle.
		float m_Duration;
		/// Repeat after the duration
		bool m_Loop;

		//////////////
		// EMISSION //
		//////////////
		/// Number of particles to spawn per unit of time passed
		float m_TimeRate;
		/// Number of particles to spawn per unit of distance travelled
		float m_DistanceRate;

		//////////////
		// POSITION //
		//////////////
		SimulationSpace m_SimulationSpace;
		Math::Vec2f m_Position;
	};
} }

#endif