#ifndef GRAPHICS_LIGHT_INL
#define GRAPHICS_LIGHT_INL

namespace OrbitEngine { namespace Graphics {
	/*
	template<class T>
	UniformsPack<GenericLight<T>::Data>* GenericLight<T>::s_LightUniformPack = 0;
	*/
	template<class T>
	GenericLight<T>::GenericLight() {

	}

	template<class T>
	GenericLight<T>::~GenericLight() {

	}

	template<class T>
	void GenericLight<T>::setPosition(Math::Vec3f position)
	{
		p_Data.position = position;
	}

	template<class T>
	Math::Vec3f GenericLight<T>::getPosition()
	{
		return p_Data.position;
	}

	template<class T>
	void GenericLight<T>::setColor(Math::Color color)
	{
		p_Data.color = color;
	}

	template<class T>
	ShaderDefinitions GenericLight<T>::getRequiredDefinitions()
	{
		return {};
	}

	template<class T>
	void GenericLight<T>::fillBuffer(Shader* shader)
	{
		if (s_LightUniformPack == 0)
			s_LightUniformPack = UniformsPack<Data>::Create();

		s_LightUniformPack->setData(p_Data);
		s_LightUniformPack->bind("LightBuffer", shader);
	}
} }

#endif