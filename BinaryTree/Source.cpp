#pragma once
#include "stdio.h"
#include "Tree.h"
#include "vector"

int main()
{
	BinaryTree<int>* Tree = new BinaryTree<int>();

	std::vector<int> GivenNumbers = { 0, -5, 10, 7, -4, 3, -2, 1, -1, -76, 23, -234, 54, 345, 7, -54, 83, 9, -11, 11, 71, -76};

	for (int& Number : GivenNumbers)
		Tree->AddNode(Number);

	printf("Layers: %d\n", Tree->GetTreeHeight());

	Tree->PrintTreeDec();

	int Highest = *Tree->PopHighest();
	printf("H: %d\n", Highest);

	Tree->PrintTreeInc();

	delete Tree;

	/*for (int& Number : GivenNumbers)
	{
		bool Persists = true;
		for (int& DelNumber : DeletedNumbers)
		{
			if (DelNumber == Number)
			{
				Persists = false;
				break;
			}
		}

		if(Persists)
			printf("Number %d persists\n", Number);
	}*/

	return 0;
}