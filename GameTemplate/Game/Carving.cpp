#include "stdafx.h"
#include "Carving.h"
#include <time.h>


Carving::Carving()
{
	srand((unsigned int)time(NULL));
}


Carving::~Carving()
{
}


void Carving::AddItem(std::string name, int value)
{
	m_item.insert(std::make_pair(name, value));
}


std::string Carving::GetItem()
{
	int s,sub = 100;

	s = rand() % 100;

	for (auto itr = m_item.begin(); itr != m_item.end(); ++itr)
	{
		sub -= itr->second;
		if (s >= sub)
		{
			return itr->first.c_str();
			break;
		}
	}
	
	std::string error = "ƒGƒ‰[";

	return error;
}