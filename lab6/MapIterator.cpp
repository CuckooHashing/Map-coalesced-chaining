#include "MapIterator.h"



MapIterator::MapIterator(const Map& mp): mp{mp}
{//O(m)
	this->current = 0;
	while (this->mp.hashTable[current].info.first == EMPTY || this->mp.hashTable[current].info.first == DELETED)
		this->current++;
}

void MapIterator::first()
{//O(m)
	this->current = 0;
	while (this->mp.hashTable[current].info.first == EMPTY || this->mp.hashTable[current].info.first == DELETED)
		this->current++;
}

void MapIterator::next()
{//O(m)
	if (valid())
	{
		this->current++;
		while (this->mp.hashTable[current].info.first == EMPTY || this->mp.hashTable[current].info.first == DELETED)
			current++;
	}
	else
		throw std::exception();
}

bool MapIterator::valid() const
{//theta(1)
	return this->current < this->mp.m;
}


TElem MapIterator::getCurrent() const
{//theta(1)
	if (valid())
		return mp.hashTable[this->current].info;
	else
		throw std::exception();
}

MapIterator::~MapIterator()
{
}
