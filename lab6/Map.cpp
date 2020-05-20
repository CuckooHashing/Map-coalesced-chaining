#include "Map.h"
#include <iostream>
#include "MapIterator.h"


Map::Map()
{//theta(m)
	this->m = 11;
	this->hashTable = new Node[this->m];
	this->len = 0;
	this->firstFree = 0;
	TElem empty;
	empty.first = EMPTY, empty.second = EMPTY;
	for (int i = 0; i < this->m; i++)
	{
		this->hashTable[i].info = empty;
	}
}

TValue Map::add(TKey c, TValue v)
{//O(m)
	if (this->len == this->m)
		this->resize();
	int pos = this->h(c);
	if (this->hashTable[pos].info.first == EMPTY || this->hashTable[pos].info.first == DELETED)
	{
		if (pos == this->firstFree)
			changeFirstFree();
		this->hashTable[pos].info.first = c;
		this->hashTable[pos].info.second = v;
		this->len++;
		return NULL_TVALUE;
	}
	else
	{
		int current = pos;
		if (this->hashTable[current].info.first == c)
		{
			TValue temp = this->hashTable[current].info.second;
			this->hashTable[current].info.second = v;
			return temp;
		}
		while (this->hashTable[current].next != -1)
		{
			if (this->hashTable[current].info.first == c)
			{
				TValue temp = this->hashTable[current].info.second;
				this->hashTable[current].info.second = v;
				return temp;
			}
			current = this->hashTable[current].next;
		}
		this->hashTable[this->firstFree].info.first = c;
		this->hashTable[this->firstFree].info.second = v;
		this->len++;
		this->hashTable[current].next = this->firstFree;
		this->changeFirstFree();
		return NULL_TVALUE;
	}
	
}

TValue Map::search(TKey c) const
{//O(m)
	int pos = this->h(c);
	if (this->hashTable[pos].info.first == EMPTY || this->hashTable[pos].info.first == DELETED)
		return NULL_TVALUE;
	else
	{
		int current = pos;
		if (this->hashTable[current].info.first == c)
		{
			return this->hashTable[current].info.second;
		}
		while (this->hashTable[current].next != -1)
		{
			if (this->hashTable[current].info.first == c)
			{
				return this->hashTable[current].info.second;
			}
			current = this->hashTable[current].next;
		}
		return NULL_TVALUE;
	}
	
}

TValue Map::remove(TKey c)
{//O(m)
	int pos = this->h(c);
	int prev = -1;
	/*if (this->hashTable[pos].info.first == EMPTY || this->hashTable[pos].info.first == DELETED)
		return NULL_TVALUE;
	else
	{
		int current = pos;
		if (this->hashTable[current].info.first == c)
		{
			TValue temp = this->hashTable[current].info.second;
			this->hashTable[current].info.first = DELETED;
			this->hashTable[current].info.second = DELETED;
			this->len--;
			changeFirstFree();
			return temp;
		}
		while (this->hashTable[current].next != -1)
		{
			if (this->hashTable[current].info.first == c)
			{
				TValue temp = this->hashTable[current].info.second;
				this->hashTable[current].info.first = DELETED;
				this->hashTable[current].info.second = DELETED;
				this->len--;
				changeFirstFree();
				return temp;
			}
			current = this->hashTable[current].next;
		}
		return NULL_TVALUE;
	}*/
	TValue removed;
	int current = 0;
	while (current < this->m && prev == -1)
	{
		if (this->hashTable[current].next == pos)
			prev = current;
		else
			current++;
	}
	while (pos != -1 && this->hashTable[pos].info.first != c)
	{
		prev = pos;
		pos = this->hashTable[pos].next;
	}
	if (pos == -1)
		return NULL_TVALUE;
	else
	{
		bool over = false;
		do
		{
			int p = this->hashTable[pos].next;
			int pp = pos;
			while (p != -1 && this->h(this->hashTable[p].info.first) != pos)
			{
				pp = p;
				p = this->hashTable[p].next;
			}
			if (p == -1)
				over = true;
			else
			{
				this->hashTable[pos] = this->hashTable[p];
				prev = pp;
				pos = p;
			}
		} while (!(over));
		if (prev != -1)
			this->hashTable[prev].next = this->hashTable[pos].next;
		this->hashTable[pos].info.first = DELETED;
		removed = this->hashTable[pos].info.second;
		this->hashTable[pos].next = -1;
		if (this->firstFree > pos)
			this->firstFree = pos;
		this->len--;
		return removed;
	}
}


int Map::size() const
{//theta(1)
	return this->len;
}

bool Map::isEmpty() const
{//theta(1)
	return this->len == 0;
}

MapIterator Map::iterator() const
{//theta(1)
	return MapIterator(*this);
}

int Map::addIfNotPresent(Map & map)
{//theta(m)
	int number = 0;
	for (int i = 0; i < map.m; i++)
	{
		if (map.hashTable[i].info.first != EMPTY && map.hashTable[i].info.first != DELETED)
		{
			int pos = this->internalSearch(map.hashTable[i].info.first);
			if (pos != EMPTY)
			{
				if (this->len == this->m)
					this->resize();
				number++;
				if (this->hashTable[pos].info.first == EMPTY || this->hashTable[pos].info.first == DELETED)
				{
					this->hashTable[pos].info = map.hashTable[i].info;
					this->len++;
				}
				else
				{
					this->hashTable[this->firstFree].info = map.hashTable[i].info;
					this->hashTable[pos].next = this->firstFree;
					this->len++;
					this->changeFirstFree();
				}

			}
		}
	}
	return number;
}

Map::~Map()
{//theta(1)
	delete this->hashTable;
}

void Map::resize()
{//O(m)	
	Node* temp = new Node[2 * this->m];
	int first = 0;
	this->m *= 2;
	for (int i = 0; i < this->m; i++)
	{
		temp[i].info.first = EMPTY;
		temp[i].info.second = EMPTY;
	}
	for (int i = 0; i < this->m/2; i++)
	{
		int pos = this->h(this->hashTable[i].info.first);
		if (temp[pos].info.first == EMPTY || temp[pos].info.first == DELETED)
		{
			if (pos == first)
			{
				first++;
				while (first < this->m && temp[first].info.first != EMPTY && temp[first].info.first != DELETED)
					first++;
			}
			temp[pos].info = this->hashTable[i].info;
		}
		else
		{
			int current = pos;
			while (temp[current].next != -1)
				current = temp[current].next;
			temp[first].info = this->hashTable[i].info;
			temp[current].next = first;
			first++;
			while (first < this->m && temp[first].info.first != EMPTY && temp[first].info.first != DELETED)
				first++;
			
		}

	}
	delete[] this->hashTable;
	this->hashTable = temp;
	this->firstFree = -1;
	this->changeFirstFree();
}

void Map::changeFirstFree()
{//O(m)
	this->firstFree++;
	while (this->firstFree < this->m && this->hashTable[this->firstFree].info.first != EMPTY && this->hashTable[this->firstFree].info.first != DELETED)
		this->firstFree++;
}

int Map::internalSearch(TKey c)
{//O(m)
	int pos = this->h(c);
	if (this->hashTable[pos].info.first == EMPTY || this->hashTable[pos].info.first == DELETED)
		return pos;
	else
	{
		int current = pos;
		int prev = -1;
		if (this->hashTable[current].info.first == c)
		{
			return EMPTY;
		}
		while (this->hashTable[current].next != -1)
		{
			if (this->hashTable[current].info.first == c)
			{
				return EMPTY;
			}
			prev = current;
			current = this->hashTable[current].next;
		}
		return current;
	}
}
