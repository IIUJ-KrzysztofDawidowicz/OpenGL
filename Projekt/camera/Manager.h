#pragma once
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

template <class T> class Manager
{
	map<string, T> mapa;
	vector<T> lista;

public:
	Manager() 
	{
		mapa = map<string, T>();
		lista = vector<T>();
	}

	
	Manager(const int initSize) 
	{
		mapa = map<string, T>();
		lista = vector<T>(initSize);
	}
	~Manager() {}

	T& operator [](const int i)
	{
		return lista[i];
	}

	T& operator [] (const string nazwa)
	{
		return mapa[nazwa];
	}

	//Dodaje nowy element do managera. Zwraca indeks dodanego elementu.
	int Dodaj(const T& t, const string& nazwa)
	{
		mapa.emplace(nazwa, t);
		lista.push_back(t);
		return lista.size();
	}

	void UsunWszystko()
	{
		mapa.clear();
		lista.clear();
	}
};