#ifndef GRAPHICS_PARTICLE_MODULES_HPP
#define GRAPHICS_PARTICLE_MODULES_HPP

#include "OE/Math/Vec2.hpp"
#include "OE/Math/ColorGradient.hpp"

namespace OrbitEngine { namespace Graphics {

	class Texture;
	class Particle;
	class ParticleEmitter;

	/// Base particle module
	class ParticleModule {
	public:
		ParticleModule(ParticleEmitter* emitter);
		virtual ~ParticleModule();

		/// Called when a particle is created
		virtual void initParticle(Particle* particle) { };
		/// Called periodically to update the particles
		virtual void update(float deltaTime) { };

	protected:
		ParticleEmitter* m_Emitter;
	};

	/// Affects the rate of particle creation
	class EmissionModule : public ParticleModule {
	public:
		EmissionModule(ParticleEmitter* emitter);

		void update(float deltaTime) override;

		void setTimeRate(float timeRate) { m_TimeRate = timeRate; }
		void setDistanceRate(float distanceRate) { m_DistanceRate = distanceRate; }

	private:
		/// Number of particles to spawn per unit of time passed
		float m_TimeRate;
		/// Number of particles to spawn per unit of distance travelled
		float m_DistanceRate;

		// internal
		float m_EmitTimeLast;
		float m_EmitTimeAccum;
		Math::Vec2f m_EmitPositionLast;
		float m_EmitDistanceAccum;
	};

	/// Defines the shape from which the particles can be emitted, and their initial direction and speed.
	class ShapeModule : public ParticleModule {
	public:
		enum Shape {
			POINT,
			CIRCLE
		};

		ShapeModule(ParticleEmitter* emitter);

		void setShape(Shape shape) { m_Shape = shape; }
		void setRadius(float radius) { m_Radius = radius; }
		void setInitialSpeed(float speed) { m_InitialSpeed = speed; }
		void setAdditionalVelocity(const Math::Vec2f& velocity) { m_AdditionalVelocity = velocity; }

		void initParticle(Particle* particle) override;
		void update(float deltaTime) override;
	private:
		Shape m_Shape;
		float m_InitialSpeed;
		float m_Radius;
		Math::Vec2f m_AdditionalVelocity; // TODO: Move to its own module
	};

	class SizeModule : public ParticleModule {
	public:
		SizeModule(ParticleEmitter* emitter);

		void setSize(float size) { m_Size = size; }
		void setStretchFactor(float stretchFactor) { m_StretchFactor = stretchFactor; }

		void initParticle(Particle* particle) override;

	private:
		float m_Size;
		float m_StretchFactor;
	};

	class ColorModule : public ParticleModule {
	public:
		ColorModule(ParticleEmitter* emitter);

		void setColor(const Math::Color4f& color) { m_Color.reset(); m_Color.addColorStop(0.0f, color); }
		void setColorGradient(const Math::ColorGradient& gradient) { m_Color = gradient; }

		void update(float deltaTime) override;

	private:
		Math::ColorGradient m_Color;
	};

	class TextureModule : public ParticleModule {
	public:
		TextureModule(ParticleEmitter* emitter);

		void setTexture(Texture* texture) { m_Texture = texture; }

		void initParticle(Particle* particle) override;

	private:
		Texture* m_Texture;
	};
} }

#endif