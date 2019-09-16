#ifndef GRAPHICS_PARTICLE_EMITTER_HPP
#define GRAPHICS_PARTICLE_EMITTER_HPP

#include <vector>

#include "OE/Graphics/2D/ParticleSystem.hpp"
#include "OE/Graphics/2D/ParticleModules.hpp"

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
		ParticleEmitter(ParticleSystem* system);
		~ParticleEmitter();

		void autoDeleteWhenDone();
		void emit(unsigned int particles);

		void setInitialLifetime(float lifetime);
		void setPosition(const Math::Vec2f& position);
		void setSimulationSpace(SimulationSpace simulationSpace);
		void setLoop(bool loop);
		void setDuration(float duration);

		float getTime() const { return m_Time; }
		float getDuration() const { return m_Duration; }
		Math::Vec2f getPosition() const { return m_Position; }
		const std::vector<Particle*>& getParticles() const { return m_Particles; }
		
		EmissionModule emissionModule;
		ShapeModule shapeModule;
		SizeModule sizeModule;
		ColorModule colorModule;
		TextureModule textureModule;

	private:
		std::vector<ParticleModule*> m_Modules;

		friend class ParticleSystem;

		void update(float deltaTime);
		void render(SpriteRenderer& sr);

		ParticleSystem* m_System;
		std::vector<Particle*> m_Particles;

		/// Current time
		float m_Time;
		/// Should be deleted after its done?
		bool m_ShouldBeDeleted;
	private:
		//////////////
		//  TIMING  //
		//////////////
		/// Length of time while emitting particles.
		/// If Loop is enabled then the duration of one cycle.
		float m_Duration;
		/// Repeat after the duration
		bool m_Loop;
		/// Initial lifetime of the particles
		float m_InitialLifetime;
		/// 
		SimulationSpace m_SimulationSpace;
		/// Position of the emitter
		Math::Vec2f m_Position;
	};
} }

#endif