#ifndef _Game_h_
#define _Game_h_

using namespace std;

class Strategy
{
public:
	Strategy(int size)
	{
		ShowDetails = true;
		boardSize = size;
		int temp = ((size * size) - 1);
		for(spacing = 0; temp > 0; spacing++) temp /= 10;
	}

	bool isSolveable(Node* n);
	bool isGameWinningState(vector<int> v);
	virtual Node* findSolution(Node* n) = 0;

	int spacing;
	int boardSize;
	bool ShowDetails;
};

class Astar : public Strategy
{
public:
	Astar(int size) : Strategy(size){}

	Node* findSolution(Node* n);
	virtual int Heuristic(Node* n) = 0;
};

class UniformCostSearch : public Astar
{
public:
	UniformCostSearch(int size) : Astar(size){}

	int Heuristic(Node* n);
};

class AMisplaced : public Astar
{
public:
	AMisplaced(int size) : Astar(size){}

	int Heuristic(Node* n);
};

class AManhattan : public Astar
{
public:
	AManhattan(int size) : Astar(size){}

	int Heuristic(Node* n);
};

class Game
{
public:
	Game()
	{
		running = true;
		showSolution = false;
		size = 3;
		currentMove = 0;
		setdigits();
		node = new Node(size);
		strategy = new AManhattan(size);
		curStrategy = 3;
		strategies.push_back("Zero?");
		strategies.push_back("Uniform Cost Search (worst)");
		strategies.push_back("A star Misplaced (better)");
		strategies.push_back("A star Manhattan (best)");
		for(int i = 0; i < size * size; i++)
			node->State.push_back(i);
		node->State[size * size - 1] = -1;
	}

	void setStrategy(int n)
	{
		delete strategy;
		switch(n)
		{
			case 1:
				strategy = new UniformCostSearch(size);
				break;
			case 2:
				strategy = new AMisplaced(size);
				break;
			case 3:
				strategy = new AManhattan(size);
				break;
			default:
				break;
		}
	}	

	void Play()
	{
		while(running)
		{
			Display();
			showSolutionSteps();
			Input();
		}
	}

	void getSolutionSteps(){
		Node *result;
		strategy->ShowDetails = false;
		result = strategy->findSolution(node);
		SolutionMoves = result->Moves;
		strategy->ShowDetails = true;
	}

	void showSolutionSteps(){
		if(showSolution){
			if(SolutionMoves.empty()){
				cout << "Computing... ";
				getSolutionSteps();
				cout << "Done! ";
			}
			else if(SolutionMoves[0] == currentMove){
				SolutionMoves.erase(SolutionMoves.begin());
			}else{
				cout << "Computing... ";
				getSolutionSteps();
				cout << "Done! ";
			}
			printMoves(SolutionMoves);
		}else{
			SolutionMoves.clear();
		}
	}

	void setdigits()
	{
		int temp = ((size * size) - 1);
		for(digits = 0; temp > 0; digits++) temp /= 10;
	}

	void Play(vector<int> state)
	{
		node->State = state;
		Play();
	}

	void remake()
	{
		system("CLS");
		cout << "Enter new size" << endl;
		cin >> size;
		setdigits();
		delete node;
		delete strategy;
		node = new Node(size);
		strategy = new AManhattan(size);
		for(int i = 0; i < size * size; i++)
			node->State.push_back(i);
		node->State[size * size - 1] = -1;
	}

	void Display()
	{
		system("CLS");
		cout << "Current Strategy: " << strategies[curStrategy] << endl;
		cout << "h : for help" << endl;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				cout << left << setw(digits) << node->State[size * i + j] + 1 << " ";
			cout << endl;
		}

	}

	void Input()
	{
		char showTrace = ' ';
		switch(getch())
		{
			case 'w':
				moveUp();
				currentMove = 1;
				break;
			case 's':
				moveDown();
				currentMove = 3;
				break;
			case 'a':
				moveLeft();
				currentMove = 2;
				break;
			case 'd':
				moveRight();
				currentMove = 4;
				break;
			case '0':
				running = 0;
				break;
			case '1':
				setStrategy(1);
				curStrategy = 1;
				break;
			case '2':
				setStrategy(2);
				curStrategy = 2;
				break;
			case '3':
				setStrategy(3);
				curStrategy = 3;
				break;
			case 'z':
				if(showSolution) showSolution = false;
				else showSolution = true;
				break;
			case 'x':
				inputCustom8Puzzle();
				break;
			case 'm':
				Mix();
				break;
			case 'S':
				Node *result;
				while(showTrace != 'y' && showTrace != 'Y' && showTrace != 'n' && showTrace != 'N'){
					cout << "Do you want to see full trace? (y/n)" << endl;
					cin >> showTrace;
				}
				if(showTrace == 'y' || showTrace == 'Y'){
					strategy->ShowDetails = true;
				}else strategy->ShowDetails = false;
				result = strategy->findSolution(node);
				if(result->Weight > 0)
				{
					cout << "To solve this problem the search algorithm expanded a total of " << result->expandedNodes << " nodes." << endl;
					cout << "The maximum number of nodes in the queue at any one time was " << result->MaxNodesInQueue << endl;
					cout << "The depth of the goal node was " << result->Weight << endl;
					cout << endl;
					cout << endl;
					printMoves(result->Moves);
					system("PAUSE");
				}
				break;
			case 'R':
				ResetPuzzle();
				break;
			case 'G':
				if(size == 3) setHardestPuzzle();
				break;
			case '?':
				remake();
				break;
			case 'h':
				Help();
				break;
			default:
				currentMove = 0;
				break;
		}
	}

	void Help()
	{
		system("CLS");
		cout << "w : (Up)    Swap '0' with the # above" << endl;
		cout << "s : (Down)  Swap '0' with the # below" << endl;
		cout << "a : (Left)  Swap '0' with the # left" << endl;
		cout << "d : (Right) Swap '0' with the # right" << endl;
		cout << "0 : Quit" << endl;
		cout << "*Path finding strategies:" << endl;
		cout << "\t1 : Uniform Cost Search          (worst)" << endl;
		cout << "\t2 : Misplaced Tile Heuristic     (better)" << endl;
		cout << "\t3 : Manhattan Distance Heuristic (best)" << endl;
		cout << "z : Show real-time shortest solution" << endl;
		cout << "\t - Based on path finding strategy used" << endl;
		cout << "x : Input custom 3x3 board" << endl;
		cout << "m : Mix puzzle with _ random moves" << endl;
		cout << "\t - <any key> : Skip watching/waiting for mix to finish" << endl;
		cout << "S : Solve" << endl;
		cout << "R : Reset board to solved state" << endl;
		cout << "G : Set 1 of 2 hardest possible puzzle states for 8 puzzle" << endl;
		cout << "? : Change puzzle size" << endl;
		cout << "h : Show this help menu" << endl;
		system("PAUSE");
	}

	void printMoves(vector<int> v)
	{
		int s = v.size();
		cout << "Moves : " << s << "\n\n";
		for(int i = 0; i < s; i++){
			switch(v[i]){
				case 1:
					cout << "up ";
					break;
				case 2:
					cout << "left ";
					break;
				case 3:
					cout << "down ";
					break;
				case 4:
					cout << "right ";
					break;
				default:
					cout << "issue: printMoves: " << v[i] << endl;
					break;
			}
		}
		cout << endl;
	}
	int findIndex(vector<int> v)
	{
		for (int i = 0; i < size * size; i++)
			if(v[i] == -1)
				return i;
	}

	void moveUp()
	{
		node->moveUp();
	}
	void moveLeft()
	{
		node->moveLeft();
	}
	void moveDown()
	{
		node->moveDown();
	}
	void moveRight()
	{
		node->moveRight();
	}

	void setHardestPuzzle() /// only for size 3
	{
		if(size != 3) return;
		switch(rand() % 2)
		{
			case 0:
				node->State[0] = 7;
				node->State[1] = 5;
				node->State[2] = 6;
				node->State[3] = 1;
				node->State[4] = 4;
				node->State[5] = 3;
				node->State[6] = 2;
				node->State[7] = -1;
				node->State[8] = 0;
				break;
			case 1:
				node->State[0] = 5;
				node->State[1] = 3;
				node->State[2] = 6;
				node->State[3] = 7;
				node->State[4] = 4;
				node->State[5] = -1;
				node->State[6] = 2;
				node->State[7] = 1;
				node->State[8] = 0;
				break;
			default:
				break;
		}
	}

	void ResetPuzzle()
	{
		node->State.clear();
		for(int i = 0; i < size * size; i++)
			node->State.push_back(i);
		node->State[size * size - 1] = -1;
	}
	void inputCustom8Puzzle(){
		system("CLS");
		int value;
		size = 3;
		node->boardSize = 3;
		node->State.clear();
		cout << "        Enter your puzzle, use a zero to represent the blank" << endl;
		cout << "        Enter the first row, use space or tabs between numbers     ";
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
		cout << "        Enter the second row, use space or tabs between numbers    ";
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
		cout << "        Enter the third row, use space or tabs between numbers     ";
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
		cin >> value; node->State.push_back(value - 1);
	}

	void Mix()
	{
		int mixMoves, lastMove = 0;
		bool skip = false;
		cout << "Enter # of moves to mix: " << endl;
		cin >> mixMoves;
		for(;mixMoves > 0; mixMoves--)
		{
			if(_kbhit())
				skip = true;
			if(!skip)
				Display();	
			switch(rand() % 4 + 1)
			{
				case 1:
					if(lastMove != 3) moveUp();
					else mixMoves++;
					break;
				case 2:
					if(lastMove != 4) moveLeft();
					else mixMoves++;
					break;
				case 3:
					if(lastMove != 1) moveDown();
					else mixMoves++;
					break;
				case 4:
					if(lastMove != 2) moveRight();
					else mixMoves++;
					break;
				default:
					cout << "\nissue with random numbers gen in Mix()" << endl;
					system("PAUSE");
					break;
			}
		}
	}
	Node *node;
	Strategy *strategy;
	vector<int> SolutionMoves;
	vector<string>  strategies;
	bool running, showSolution;
	int digits, size, currentMove, curStrategy;
};

#endif //_Game_h_