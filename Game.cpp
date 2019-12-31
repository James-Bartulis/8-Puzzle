using namespace std;

bool Strategy::isSolveable(Node* n)
{
	if(boardSize == 3){
		int inversions = 0;
		for(int i = 0; i < (boardSize * boardSize) - 1; i++)
			for(int j = i + 1; j < boardSize * boardSize; j++)
				if(n->State[i] + 1 && n->State[j] + 1 && n->State[i] > n->State[j]) /// values stored are (-1...8)
					inversions++;
		return (inversions % 2 == 0);
	}
	return true;
}

bool Strategy::isGameWinningState(vector<int> v)
{
	for(int i = 0; i < (boardSize * boardSize - 1); i++)
		if(v[i] != i)
			return false;
	return true;
}

Node* Astar::findSolution(Node* n)
{
	if(!isSolveable(n)){
		cout << "No Solution: " << endl;
		system("PAUSE");
		return n;
	}

	Node* CurrentNode;
	int maxnodesinqueue = 0;
	int expandednodes = 0;
	int counter = 0;
	unordered_map<string, bool> historyMap;
	auto comp = [] (const Node *A,const Node *B) { return A->Weight + A->H > B->Weight + B->H;};
	priority_queue<Node*, vector<Node*>, decltype(comp)> Q(comp);
	n->Moves.clear();
	n->Weight = 0;
	Q.push(n);

	if(ShowDetails){
		system("CLS");
		cout << "Expanding state ";
		for (int i = 0; i < boardSize * boardSize; i++)
		{
			if(i != 0 && (i + 1) % boardSize == 1 )
				cout << setw(0) << "                ";
			if(n->State[i] != -1)
				cout << left << setw(spacing) << n->State[i] + 1 << " ";
			else
				cout << left << setw(spacing) << "b ";
			if(i != 0 && (i - (boardSize - 2)) % boardSize == 1)
				cout << endl;
		}
		cout << "\n\n";
	}

	while(!Q.empty())
	{
		if(_kbhit())
			ShowDetails = false;
		CurrentNode = Q.top();
		if(Q.size() > maxnodesinqueue)
			maxnodesinqueue = Q.size();
		Q.pop();
		if(isGameWinningState(CurrentNode->State))
		{	
			if(ShowDetails){
				cout << "The best state to expand with a g(n) = " << CurrentNode->Weight << " and h(n) = " << CurrentNode->H << " is...\n";
				CurrentNode->print(CurrentNode->State);
				cout << "\n\nGOAL!!\n\n";
			}
			CurrentNode->expandedNodes = expandednodes;
			CurrentNode->MaxNodesInQueue = maxnodesinqueue;
			return CurrentNode;
		}
		if(CurrentNode != n && ShowDetails)
		{
			cout << "The best state to expand with a g(n) = " << CurrentNode->Weight << " and h(n) = " << CurrentNode->H << " is...\n";
			CurrentNode->print(CurrentNode->State);
			cout << "Expanding this node...\n\n";
		}
		/// expanding start
		counter = 0;
		string entry;
		entry = "";
		for(int i = 0; i < boardSize* boardSize; i++){
			entry.push_back((CurrentNode->State[i] + 1) + '!');
		}
		// cout << entry << endl;
		historyMap.insert(make_pair(entry, true));
		while(!CurrentNode->Children.empty())
			CurrentNode->Children.pop_back();
		CurrentNode->Children.push_back(new Node(CurrentNode));
		CurrentNode->Children[counter++]->moveUp();
		CurrentNode->Children.push_back(new Node(CurrentNode));
		CurrentNode->Children[counter++]->moveLeft();
		CurrentNode->Children.push_back(new Node(CurrentNode));
		CurrentNode->Children[counter++]->moveDown();
		CurrentNode->Children.push_back(new Node(CurrentNode));
		CurrentNode->Children[counter++]->moveRight();
		int childrenSize = CurrentNode->Children.size();
		for(int i = 0; i < childrenSize; i++){
			if(CurrentNode->Children[i] != NULL){
				entry = "";
				for(int j = 0; j < boardSize* boardSize; j++){
					entry.push_back((CurrentNode->Children[i]->State[j] + 1) + '!');
				}
				// cout << i << " : " << entry << endl;
				auto search = historyMap.find(entry);
				if(search != historyMap.end()){
					CurrentNode->Children[i] = NULL;
				}
			}
		}
		/// expanding end
		expandednodes += 4;
		for (int i = 0; i < CurrentNode->Children.size(); i++)
		{
			if(CurrentNode->Children[i] != NULL)
			{
				Heuristic(CurrentNode->Children[i]);
				Q.push(CurrentNode->Children[i]);
			}
		}
	}
	return NULL;
}
int UniformCostSearch::Heuristic(Node* n)
{
	return 0;
}

int AMisplaced::Heuristic(Node* n)
{
	int addedWeight = -1;
	for(int i = 0; i < boardSize * boardSize; i++)
		if(n->State[i] != i)
			addedWeight++;
	n->H = addedWeight;
	return addedWeight;
}

int AManhattan::Heuristic(Node* n)
{
	int addedWeight = 0;
	int temp, k, l;
	for(int i = 0; i < boardSize; i++)
		for(int j = 0; j < boardSize; j++)
		{
			temp = n->State[boardSize * i + j];
			if(temp != -1)
			{
				k = temp / boardSize;
				l = temp % boardSize;
				addedWeight+= abs(k - i) + abs(l - j); 
			}
		}
	n->H = addedWeight;
	return addedWeight;
}