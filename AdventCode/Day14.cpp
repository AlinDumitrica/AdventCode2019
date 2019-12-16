#include "main.h"
#include<stdio.h> 
#include<string.h> 
#include<map>
#include<list>

#define FUEL ((int('F') << 24) + (int('U') << 16) + (int('E') << 8) + (int('L') << 0))
#define ORE  ((int('O') << 16) + (int('R') << 8) + (int('E') << 0))
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#define min(a,b)            (((a) < (b)) ? (a) : (b))

struct Component
{
	int64 name = 0;
	int64 count = 0;
	int64 unused = 0;
	Component() {};
	Component(int64 _name, int _count) :name(_name), count(_count) {}
};

struct Reaction
{
	std::list<Component*> components;
	Component* result;
};

std::map<int64, Reaction*> reactions;


typedef std::pair<int64, Reaction*> reactionPair;
typedef std::pair<int64, Component> componentPair;

char* GetComponent(char* token, Component &component)
{
	int value = 0, name = 0;
	if (token != NULL)
	{
		component.count = atoi(token);
	}

	token = strtok(0, ", =>\n");
	if (token != NULL)
	{
		for (int index = 0; index < strlen(token); ++index)
			component.name = (component.name << 8) + int(token[index]);
	}

	if (component.name == FUEL)
	{
		int a = 10;
	}
	token = strtok(0, ", =>\n");
	return token;
}

int64 GetOreCount(std::map<int64, Component> &needList)
{
	int changes = 1;
	while (changes != 0)
	{
		changes = 0;
		for (auto it = needList.begin(); it != needList.end(); ++it)
		{
			if (it->first != ORE)
			{
				Reaction* reaction = reactions[it->first];
				if (it->second.count == 0)
					continue;

				int64 needed = max(it->second.count - it->second.unused, 0);
				int64 overflow = max(it->second.unused - it->second.count, 0);
				int64 overproduced = ceil((double)(needed) / reaction->result->count) * reaction->result->count - needed;

				it->second.unused = max(overflow + overproduced, 0);
				it->second.count = 0;

				for (auto it2 = reaction->components.begin(); it2 != reaction->components.end(); ++it2)
				{
					int64 overflowMaterials = (*it2)->count * ceil((double)(needed) / reaction->result->count);
					Component *component;

					if (needList.find((*it2)->name) != needList.end())
						component = &(needList.find((*it2)->name)->second);
					else
						component = new Component((*it2)->name, 0);

					component->count += overflowMaterials;

					if (needList.find((*it2)->name) == needList.end())
						needList.insert(componentPair((*it2)->name, *component));
				}

				changes++;
			}
		}
	}

	for (auto it = needList.begin(); it != needList.end(); ++it)
	{
		if (it->first == ORE)
			return it->second.count;
	}
	return -1;
}

CREATE_TEST(14)
{
    FILE *file = fopen("day14.txt", "r");

	char buf[512];
	

	while (fgets(buf, sizeof buf, file) != NULL) 
	{
		Reaction* reaction = new Reaction();
		char * token = strtok(buf, ", =>\n");
		while (token != NULL)
		{
			Component* component = new Component();
			token = GetComponent(token, *component);
			reaction->components.push_back(component);
		}
		Component* component = reaction->components.back();
		reaction->result = component;
		reactions.insert(reactionPair(component->name, reaction));
		reaction->components.pop_back();
	}

	//2267486
	int64 index = 2267000, result = 0;
    //while(result < 1000000000000)
	{
		std::map<int64, Component> needList;
		Component *component = new Component(FUEL, 2267486);
		needList.insert(componentPair(FUEL, *component));
		result = GetOreCount(needList);
	}


	cout << index - 2;
	//cout << needList.begin()->second.count<<endl;
	//cout << needList.begin()->second.unused << endl;
	//cout << NZVS;
}
