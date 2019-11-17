#pragma once
//#include "vector"

//std::vector<int> DeletedNumbers;//Basic memory leak detection

template <typename T>
struct TreeNode
{
	//Creation constructor
	TreeNode(const T& NData) : LeftNode(nullptr), RightNode(nullptr), Data(NData) {}
	~TreeNode() 
	{
		//DeletedNumbers.push_back(Data);//Rmove this when using in a program
		if (LeftNode)
			delete LeftNode;
		if (RightNode)
			delete RightNode;
	}

	//Actual data held by this Node
	T Data;
	//Node with lower value
	TreeNode* LeftNode;
	//Node with higher value
	TreeNode* RightNode;
};

/*Class template for storing data in a binary tree form
i.e. Left nodes hold lower values and Right nodes hold
higher values.

This can be combined with MemoryPool in order to improve performance*/
template <typename T>
class BinaryTree
{

public:

	//Default constructor - nothing special
	BinaryTree() : RootNode(nullptr) {}
	~BinaryTree();

	/*Copying a tree seems like a bad idea for now.
	Pointers will be the same after copying which 
	creates problems after deletion of a node.*/
	BinaryTree(const BinaryTree& Tree) = delete;

	/*Add a new node to the tree.
	Returns false if operator new fails.*/
	bool AddNode(const T& NData);

	/*Get and remove node with the highest data value.
	Will return nullptr if the tree is empty*/
	T* PopHighest();
	/*Get and remove node with the lowset data value.
	Will return nullptr if the tree is empty.*/
	T* PopLowest();

	/*Get a copy of the highest value in the tree.
	Will return nullptr if the tree is empty*/
	T* GetHighest();
	/*Get a copy of the Lowest value in the tree.
	Will return nullptr if the tree is empty*/
	T* GetLowest();

	/*Checks whether the tree is empty i.e. RootNode is nullptr*/
	inline bool IsEmpty() const { return !(bool)RootNode; }

	//Returns height of the tree i.e. number of layers
	unsigned int GetTreeHeight();

	/*Prints the tree in increasing order.
	Form lowest to highest.*/
	void PrintTreeInc();
	/*Prints the tree in decreasing order.
	From highest to lowest.*/
	void PrintTreeDec();

private:

	/*Begining of a tree.
	Is nullptr if the tree is empty.
	*/
	TreeNode<T>* RootNode;

	/*CompareInsertion is a recursive function which are IMO
	a great way to cause stack overflow. Pop functions use while loop
	which I would use normally. Recrusion is here just for the
	sake of the exercise.
	*/

	/*Recursive function that finds an appropriate spot for
	the given data and insertes it there.
	Will return false if new node could not be created, that
	is when operator new fails.*/
	bool CompareInsertion(const T& NData, TreeNode<T>*& Node);

	/*Recursive function. Walks down the tree counting maximum 
	number of layers.
	WARNING - if the tree is too high this function will cause
	stack overflow.*/
	unsigned int CountHeight(TreeNode<T>*& Node);

	/*Recursive function. Used by print function*/
	void PrintInc(TreeNode<T>*& Node);
	/*Recursive function. Used by print function*/
	void PrintDec(TreeNode<T>*& Node);
};

//
//
//IMPLEMENTATION
//
//

template <typename T>
BinaryTree<T>::~BinaryTree()
{
	if (RootNode)
		delete RootNode;
}

template <typename T>
bool BinaryTree<T>::AddNode(const T& NData)
{
	if (RootNode)//Check whether the tree is empty
	{
		return CompareInsertion(NData, RootNode);//Insert data into an appropriate spot
	}
	else
	{
		RootNode = new TreeNode<T>(NData);//Tree is empty so we start a new one

		if (RootNode)//Check if the tree was successfully initialized
			return true;
		return false;
	}
}

template <typename T>
bool BinaryTree<T>::CompareInsertion(const T& NData, TreeNode<T>*& Node)
{
	if (Node->Data > NData)//Check data's value
	{
		if (Node->LeftNode)//Check for another node to compare with
		{
			return CompareInsertion(NData, Node->LeftNode);//Contiune down the tree
		}
		else
		{
			Node->LeftNode = new TreeNode<T>(NData);//End of the tree. Creating a new leaf
			if (Node->LeftNode)//Operator new successfully allocated memory
				return true;
		}
	}
	else if (Node->Data < NData)// Again check data's value
	{
		if (Node->RightNode)//Check for another node to compare with
		{
			return CompareInsertion(NData, Node->RightNode);//Contiune down the tree
		}
		else
		{
			Node->RightNode = new TreeNode<T>(NData);//End of the tree. Creating a new leaf
			if (Node->RightNode)//Operator new successfully allocated memory
				return true;
		}
	}
	return false;//NData has the same value as Node->Data or operator new couldnt allocate memory
}

template <typename T>
T* BinaryTree<T>::PopHighest()
{
	if (RootNode)//Check whether the tree is empty
	{
		TreeNode<T>* Node = RootNode;
		TreeNode<T>* ParentNode = nullptr;

		while (Node->RightNode)//Continue down the tree's right side. The right most value is the highest one
		{
			ParentNode = Node;
			Node = Node->RightNode;
		}

		if (ParentNode)//Check whether RootNode is the one being returned. Only RootNode has no ParentNode
		{
			if (Node->LeftNode)//Nodes form the left might exist so they are moved up in the tree
			{
				ParentNode->RightNode = Node->LeftNode;
				Node->LeftNode = nullptr;//Remove the pointer in order to prevent ChildNode form being deleted
			}
			else
				ParentNode->RightNode = nullptr;//There are no nodes to move so just remove the returned node
		}
		else
		{
			if (RootNode->LeftNode)//ParentNode might still have Child Nodes that hold lower value. They are moved up the tree
			{
				RootNode = RootNode->LeftNode;
				Node->LeftNode = nullptr;//Remove the pointer in order to prevent ChildNode form being deleted
			}
			else
				RootNode = nullptr;//ParentNode has no Child Nodes so just clean the tree
		}

		T Value = Node->Data;
		delete Node;//Clean up

		return &Value;//Return the highest value
	}
	else
		return nullptr;//The tree is empty so there is nothing to return
}

template <typename T>
T* BinaryTree<T>::PopLowest()
{
	if (RootNode)//Check whether the tree is empty
	{
		TreeNode<T>* Node = RootNode;
		TreeNode<T>* ParentNode = nullptr;

		while (Node->LeftNode)//Continue down the tree's left side. The left most value is the lowest one
		{
			ParentNode = Node;
			Node = Node->LeftNode;
		}

		if (ParentNode)//Check whether RootNode is the one being returned. Only RootNode has no ParentNode
		{
			if (Node->RightNode)//Nodes form the right side might exist so they are moved up in the tree
			{
				ParentNode->LeftNode = Node->RightNode;
				Node->RightNode = nullptr;//Remove the pointer in order to prevent ChildNode form being deleted
			}
			else
				ParentNode->LeftNode = nullptr;//There are no nodes to move so just remove the returned node
		}
		else
		{
			if (RootNode->RightNode)//ParentNode might still have Child Nodes that hold lower value. They are moved up the tree
			{
				RootNode = RootNode->RightNode;
				Node->RightNode = nullptr;//Remove the pointer in order to prevent ChildNode form being deleted
			}
			else
				RootNode = nullptr;//ParentNode has no Child Nodes so just clean the tree
		}

		T Value = Node->Data;
		delete Node;//Clean up

		return &Value;//Return the lowest value
	}
	else
		return nullptr;//The tree is empty so there is nothing to return
}

template <typename T>
unsigned int BinaryTree<T>::GetTreeHeight()
{
	if (RootNode)
		return CountHeight(RootNode);

	return 0;
}

template <typename T>
unsigned int BinaryTree<T>::CountHeight(TreeNode<T>*& Node)
{
	unsigned int LayerCountLeft = 1;
	unsigned int LayerCountRight = 1;

	if (Node->LeftNode)
		LayerCountLeft += CountHeight(Node->LeftNode);

	if (Node->RightNode)
		LayerCountRight += CountHeight(Node->RightNode);

	if (LayerCountLeft > LayerCountRight)
		return LayerCountLeft;
	return LayerCountRight;
}

template <typename T>
T* BinaryTree<T>::GetHighest()
{
	if (RootNode)//Check whether the tree is empty
	{
		TreeNode<T>* Node = RootNode;

		while (Node->RightNode)//Continue down the tree's right side. The right most value is the highest one
		{
			Node = Node->RightNode;
		}

		return &Node->Data;//Return the highest value
	}
	else
		return nullptr;//The tree is empty so there is nothing to return
}

template <typename T>
T* BinaryTree<T>::GetLowest()
{
	if (RootNode)//Check whether the tree is empty
	{
		TreeNode<T>* Node = RootNode;

		while (Node->LeftNode)//Continue down the tree's left side. The left most value is the lowest one
		{
			Node = Node->LeftNode;
		}

		return &Node->Data;//Return the lowest value
	}
	else
		return nullptr;//The tree is empty so there is nothing to return
}

template <typename T>
void BinaryTree<T>::PrintTreeInc()
{
	if (RootNode)//Check whether the tree is empty
	{
		PrintInc(RootNode);//Actual print
		printf("\n");
	}
	else
		printf("This tree is empty");
}

template <typename T>
void BinaryTree<T>::PrintTreeDec()
{
	if (RootNode)//Check whether the tree is empty
	{
		PrintDec(RootNode);//Actual print
		printf("\n");
	}
	else
		printf("This tree is empty");
}

template <typename T>
void BinaryTree<T>::PrintInc(TreeNode<T>*& Node)
{
	if (Node->LeftNode)//First print left nodes
		PrintInc(Node->LeftNode);
	printf("%d ", Node->Data);//Print this node
	if (Node->RightNode)//Print right nodes
		PrintInc(Node->RightNode);
}

template <typename T>
void BinaryTree<T>::PrintDec(TreeNode<T>*& Node)
{
	if (Node->RightNode)//First print right nodes
		PrintDec(Node->RightNode);
	printf("%d ", Node->Data);//Print this node
	if (Node->LeftNode)//Print left nodes
		PrintDec(Node->LeftNode);
}