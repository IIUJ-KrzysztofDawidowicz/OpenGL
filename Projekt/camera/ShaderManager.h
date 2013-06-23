#pragma once
#include "Manager.h"
#include "Shader.h"

class ShaderManager: public Manager<Shader>
{
	ShaderManager(){}
public:
	static ShaderManager& getInstance() 
	{
		static ShaderManager instance;
		return instance;
	}
	~ShaderManager()
	{
		UsunWszystko();
	}

	void UsunWszystko()
	{
		for(int i=0; i<lista.size(); ++i)
			lista[i].Delete();
		lista.clear();
		mapa.clear();
	}
};