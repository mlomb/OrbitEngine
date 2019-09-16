#include "OE/Graphics/2D/ParticleModules.hpp"

#include <random>

#include "OE/Misc/Log.hpp"

#include "OE/Graphics/2D/ParticleSystem.hpp"
#include "OE/Graphics/2D/ParticleEmitter.hpp"

namespace OrbitEngine { namespace Graphics {

	ParticleModule::ParticleModule(ParticleEmitter* emitter)
		: m_Emitter(emitter)
	{
	}

	ParticleModule::~ParticleModule()
	{
	}

	EmissionModule::EmissionModule(ParticleEmitter* emitter) :
		ParticleModule(emitter),
		m_TimeRate(10),
		m_DistanceRate(0),

		m_EmitTimeLast(0),
		m_EmitTimeAccum(0),
		m_EmitPositionLast(Math::Vec2f()),
		m_EmitDistanceAccum(0)
	{
	}

	void EmissionModule::update(float deltaTime)
	{
		const float time = m_Emitter->getTime();
		const Math::Vec2f& pos = m_Emitter->getPosition();

		if (time > m_Emitter->getDuration())
			return;

		if (time < m_EmitTimeLast) // cycle loop
			m_EmitTimeLast = 0; // TODO: Wrong

		m_EmitTimeAccum += time - m_EmitTimeLast;
		m_EmitTimeLast = time;

		if (m_EmitPositionLast.x == 0.0f && m_EmitPositionLast.y == 0.0f)
			m_EmitPositionLast = pos; // first
		m_EmitDistanceAccum += pos.distanceTo(m_EmitPositionLast);
		m_EmitPositionLast = pos;

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

		m_Emitter->emit(to_emit);
	}

	ShapeModule::ShapeModule(ParticleEmitter* emitter)
		: ParticleModule(emitter),
		m_Shape(Shape::POINT),
		m_Radius(1.0f),
		m_InitialSpeed(5.0f),
		m_AdditionalVelocity(0, 0)
	{
	}

	void ShapeModule::initParticle(Particle* particle)
	{
		Math::Vec2f vel = Math::Vec2f(0, 0);
		Math::Vec2f off = Math::Vec2f(0, 0);
		
		switch (m_Shape) {
		default:
		case POINT: break; // already centered
		case CIRCLE:
			static std::random_device rd;
			static std::mt19937 gen(rd());
			static std::uniform_real_distribution<> dis(0, 1);

			Math::Vec2f dir = Math::Vec2f::FromAngle(dis(gen) * 2.0f * PI);
			vel = dir;
			off = dir * m_Radius;
			break;
		}

		particle->position += off;
		particle->velocity = m_AdditionalVelocity + vel * m_InitialSpeed;
	}

	void ShapeModule::update(float deltaTime)
	{
		const std::vector<Particle*>& particles = m_Emitter->getParticles();
		for (Particle* particle : particles) {
			particle->position += particle->velocity * deltaTime;
		}
	}

	SizeModule::SizeModule(ParticleEmitter* emitter)
		: ParticleModule(emitter),
		m_Size(4),
		m_StretchFactor(1)
	{
	}

	void SizeModule::initParticle(Particle* particle)
	{
		particle->size = m_Size;
		particle->stretch = m_StretchFactor;
	}

	ColorModule::ColorModule(ParticleEmitter* emitter)
		: ParticleModule(emitter), m_Color()
	{
		setColor(Math::Color4f(1, 1, 1, 1));
	}

	void ColorModule::update(float deltaTime)
	{
		const std::vector<Particle*>& particles = m_Emitter->getParticles();
		for (Particle* particle : particles) {
			particle->color = m_Color.evaluate(particle->time);
		}
	}

	TextureModule::TextureModule(ParticleEmitter* emitter)
		: ParticleModule(emitter), m_Texture(NULL)
	{
	}

	void TextureModule::initParticle(Particle* particle)
	{
		particle->texture = m_Texture;
	}
} }