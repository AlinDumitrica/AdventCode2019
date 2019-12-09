#include "main.h"

#define GET_CODE(code) (int(code[0]) << 16 | int(code[1]) << 8 | int(code[2]))
#define YOU GET_CODE("YOU")
#define SAN GET_CODE("SAN")

struct Node
{
	int code;
	int level = 0;
	Node* parent = nullptr;
	bool visited = false;
	std::deque<Node*> childs;
	Node(int _code): code(_code) { };
};
std::vector<Node> nodes;

Node& GetOrCreateNode(int code)
{
	for (int index = 0; index < nodes.size(); ++index)
	{
		if (code == nodes[index].code) 
			return nodes[index];
	}

	nodes.emplace_back(code);
	return nodes.back();
}

void UpdateDepth(Node* node, int depth)
{
	node->level = depth;
	for (int index = 0; index < node->childs.size(); ++index)
	{
		if (node->childs[index]->level != 0)
		{
			cout << "Cicle detected." << endl;
		}
		UpdateDepth(node->childs[index], depth + 1);
	}
}

void FindSOL(Node* node, int depth)
{
	if (node == nullptr || node->visited)
		return;

	node->visited = true;
	if (node->code == SAN)
		cout << "Second Part:" << depth - 2 << endl;

	FindSOL(node->parent, depth + 1);

	for (int index = 0; index < node->childs.size(); ++index)
	{
		FindSOL(node->childs[index], depth + 1);
	}
}

CREATE_TEST(6)
{
	nodes.reserve(2000);
	int sum = 0;
	FILE *file = fopen("day6.txt", "r");
	char point1[4], point2[4];
	 
	while (fscanf(file, "%3s)%3s", &point1[0], &point2[0]) == 2)
	{
		int code1 = GET_CODE(point1);
		int code2 = GET_CODE(point2);
		Node &parentNode = GetOrCreateNode(code1);
		Node &childNode = GetOrCreateNode(code2);
		parentNode.childs.push_back(&childNode);
		childNode.parent = &parentNode;
	}

	for (int index = 0; index < nodes.size(); ++index)
	{
		if (!nodes[index].parent)
		{
			UpdateDepth(&nodes[index], 0);
		}
	}

	for (int index = 0; index < nodes.size(); ++index)
	{
		sum += nodes[index].level;
	}

	cout <<"First Part:" << sum << endl;

	for (int index = 0; index < nodes.size(); ++index)
	{
		if (nodes[index].code == YOU)
		{
			FindSOL(&nodes[index], 0);
			break;
		}
	}
}
