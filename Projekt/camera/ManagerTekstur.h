#pragma once
#include "Tekstury.h"
#include "Manager.h"

class ManagerTekstur: public Manager<Tekstura>
{
	ManagerTekstur(){};
public:
	static ManagerTekstur& getInstance()
	{
		static ManagerTekstur instancja;
		return instancja;
	}

	void UnbindAll()
	{
		for(int i = 0; i<lista.size(); ++i)
			lista[i].Unbind();
	}
};
