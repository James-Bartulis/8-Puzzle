#ifndef _Node_h_
#define _Node_h_

using namespace std;

class Node
{
public:
	Node(int size)
	{
		boardSize = size;
		int temp = ((size * size) - 1);
		for(spacing = 0; temp > 0; spacing++) temp /= 10;
		Weight = 0;
		expandedNodes = 0;
		MaxNodesInQueue = 0;
	}
	Node(Node* n)
	{
		this->boardSize = n->boardSize;
		this->spacing = n->spacing;
		expandedNodes = 0;
		MaxNodesInQueue = 0;
		this->Weight = n->Weight + 1;
		this->State = n->State;
		this->Moves = n->Moves;
	}
	~Node()
	{
		State.clear();
		Moves.clear();
	}

	int findIndex(const vector<int> &v)
	{
		for (int i = 0; i < boardSize * boardSize; i++)
			if(v[i] == -1)
				return i;
	}

	void moveUp()
	{
		int index = findIndex(State);
		if(index - boardSize >= 0)
		{
			swap(index, index - boardSize);
			Moves.push_back(1);
		}
	}
	void moveLeft()
	{
		int index = findIndex(State);
		if(index - 1 >= 0 && (index - 1) / boardSize == index / boardSize)
		{
			swap(index, index - 1);
			Moves.push_back(2);
		}
	}
	void moveDown()
	{
		int index = findIndex(State);
		if(index + boardSize < boardSize * boardSize)
		{
			swap(index, index + boardSize);
			Moves.push_back(3);
		}
	}
	void moveRight()
	{
		int index = findIndex(State);
		if(index / boardSize == (index + 1) / boardSize)
		{
			swap(index, index + 1);
			Moves.push_back(4);
		}
	}

	void Display()
	{
		system("CLS");
		for (int i = 0; i < boardSize; i++)
		{
			for (int j = 0; j < boardSize; j++)
				cout << State[boardSize * i + j] + 1;
			cout << endl;
		}
	}

	void print(vector<int> v)
	{
		int s = v.size(), temp;
		for(int i = 0; i < boardSize; i++)
		{
			cout << string(16,' ');
			for(int j = 0; j < boardSize; j++)
			{
				temp = v[boardSize*i + j] + 1;
				if(temp == 0)
					cout << left << setw(spacing) << 'b' << " ";
				else
					cout << left << setw(spacing) << temp << " ";
			}
			if(i < boardSize - 1)
				cout << endl;
		}
	}

	void swap(const int &a, const int &b)
	{
		int temp = State[a];
		State[a] = State[b];
		State[b] = temp;
	}

	vector<Node*> Children;
	vector<int> State;
	vector<int> Moves;
	int spacing;
	int boardSize;
	int expandedNodes;
	int MaxNodesInQueue;
	int Weight;
	int H;
};

#endif //_Node_h_