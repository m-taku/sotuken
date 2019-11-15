#include "stdafx.h"
#include "Carving.h"
#include <time.h>


Carving::Carving()
{
	static int f_init = false;
	// ˆê“x‚¾‚¯‚Ì‰Šú‰»
	if (!f_init) {
		srand((unsigned int)time(NULL));
		f_init = true;
	}
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