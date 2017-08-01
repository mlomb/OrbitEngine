#ifndef GRAPHICS_PIPELINE_HPP
#define GRAPHICS_PIPELINE_HPP

#include "OE/Graphics/API/Shader.hpp"

namespace OrbitEngine {	namespace Graphics {
	class Pipeline {
	public:

		virtual void flush() = 0;

		void setShader(Shader* shader);

		static Pipeline* Create();
	private:

	protected:
		Pipeline() {};

		Shader* p_Shader = 0;
	};
} }

#endif