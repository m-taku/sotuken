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
	int s, count = 0;

	s = rand() % 100;

	for (auto itr = m_item.begin(); itr != m_item.end(); ++itr)
	{
		for (int i = 0; i < itr->second; i++)
		{
			if (count == s)
			{
				return itr->first.c_str();
			}
			count++;
		}
	}
	
	std::string error = "ƒGƒ‰[";

	return error;
}