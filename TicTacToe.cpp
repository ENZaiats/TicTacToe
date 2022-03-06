#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;



const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const char TIE = 'T';
const char NO_ONE = 'N';

void instructions()
{
	setlocale(LC_ALL, "Russian");

	cout << "Для выполнения хода нужно ввести число от 0 до 8.\n";
	cout << "Цифры соответсвуют номеру клетки:\n\n";
	cout << " 0 | 1 | 2 \n";
	cout << "-----------\n";
	cout << " 3 | 4 | 5 \n";
	cout << "-----------\n";
	cout << " 6 | 7 | 8 \n\n";
	cout << "Готовься, человек. Битва начинается.\n\n";
}

char askYesNo(string question)
{
	char response;
	do
	{
		cout << question << " (y/n): ";
		cin >> response;
	} while (response != 'y' && response != 'n');
	
	return response;
}

int askNumber(string question, int high = 8, int low = 0)
{
	int number;
	do
	{
		cout << question << " (" << low << " - " << high << "): ";
		cin >> number;
	} while (number > high || number < low);

	return number;
}

char humanPiece()
{
	setlocale(LC_ALL, "Russian");
	char go_first = askYesNo("Ты хочешь превым сделать ход?");
	if (go_first == 'y')
	{
		cout << "\nТогда ходи первым. Может тебе это поможет. \n";
		return X;
	}
	else
	{
		cout << "\nТвоё добродушие тебя погубит. Я хожу первым.\n";
		return O;
	}
}

char opponent(char piece)
{
	if (piece == X)
	{
		return O;
	}
	else
	{
		return X;
	}
}

void displayBoard(const vector<char>& board)
{
	cout << "\n\t" << board[0] << " | " << board[1] << " | " << board[2];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[3] << " | " << board[4] << " | " << board[5];
	cout << "\n\t" << "---------";
	cout << "\n\t" << board[6] << " | " << board[7] << " | " << board[8];
	cout << "\n\n";
}

char winner(const vector<char>& board)
{
	const int WINNING_ROWS[8][3] = {
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		{0, 4, 8},
		{2, 4, 6} };

	const int TOTAL_ROWS = 8;
	for (int row = 0; row < TOTAL_ROWS; ++row)
	{
		if ((board[WINNING_ROWS[row][0]] != EMPTY) &&
			(board[WINNING_ROWS[row][0]] == board[WINNING_ROWS[row][1]]) &&
			(board[WINNING_ROWS[row][1]] == board[WINNING_ROWS[row][2]]) )
		{
			return board[WINNING_ROWS[row][0]];
		}
	}
	if (count(board.begin(), board.end(), EMPTY) == 0)
		return TIE;
	
	return NO_ONE;
}

inline bool isLegal(int move, const vector<char>& board)
{
	return (board[move] == EMPTY);
}

int humanMove(const vector<char>& board, char human)
{	
	setlocale(LC_ALL, "Russian");
	int move = askNumber("Какую клетку ты выберешь?", (board.size() - 1));
	while (!isLegal(move, board))
	{
		cout << "\nЭта клетка уже занята, меня не проведешь!\n";
		move = askNumber("Какую клетку ты выберешь?", (board.size() - 1));
	}
	cout << "Хорошо...\n";

	return move;
}

int computerMove(vector<char>& board, char computer)
{
	unsigned int move = 0;
	bool found = false;
	while (!found && move < board.size())
	{
		if (isLegal(move, board))
		{
			board[move] = computer;
			found = winner(board) == computer;
			board[move] = EMPTY;
		}
		if (!found)
		{
			++move;
		}
	}

	if (!found)
	{
		move = 0;
		char human = opponent(computer);
		while (!found && move < board.size())
		{
			if (isLegal(move, board))
			{
				board[move] = human;
				found = winner(board) == human;
				board[move] = EMPTY;
			}
			if (!found)
			{
				++move;
			}
		}
	}
	if (!found)
	{
		move = 0;
		unsigned int i = 0;
		const int BEST_MOVES[] = { 4, 0, 2, 6, 8, 1, 3, 5, 7 };
		while (!found && i < board.size())
		{
			move = BEST_MOVES[i];
			if (isLegal(move, board))
			{
				found = true;
			}
			++i;
		}
	}
	setlocale(LC_ALL, "Russian");
	cout << "Мой ход." << endl;

	return move;
}

void announceWinner(char winner, char computer, char human)
{
	setlocale(LC_ALL, "Russian");

	if (winner == computer)
	{
				cout << winner << " выиграл!\n";
		cout << "Как я и предпологал, человек, в очередной раз доказано,\n";
		cout << "что компьютеры разумнее людей.\n";
	}
	else if (winner == human)
	{
		cout << winner << "выиграл!\n";
		cout << "НЕТ, НЕТ, НЕЕЕЕЕТ! Этого не может быть! Ты сжульничал, человек.\n";
		cout << "Клянусь, я больше такого не допущу!\n";
	}
	else
	{
		cout << "Ничья.\n";
		cout << "Тебе повезло. Это лучший исход, на который ты мог расчитывать.\n";
	}
}

int main()
{
	int move;
	
	const int NUM_SQUARES = 9;
	
	vector<char> board(NUM_SQUARES, EMPTY);
	
	instructions();
	
	char human = humanPiece();
	char computer = opponent(human);
	char turn = X;
	
	displayBoard(board);

	while (winner(board) == NO_ONE)
	{
		if (turn == human)
		{
			move = humanMove(board, human);
			board[move] = human;
		}
		else
		{
			move = computerMove(board, computer);
			board[move] = computer;
		}
		
		displayBoard(board);
		turn = opponent(turn);
	}
	announceWinner(winner(board), computer, human);
	
	return 0;
}
