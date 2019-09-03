#include "Board.h"


Board::Board()
{
	Pieces_front = NULL;
	Pieces_back = NULL;
	boardSize = 4;
	smallestNumer = 2;
}

Board::~Board()
{

}

void Board::SetRandomScores()
{
	SetNextRandomPiece();
	SetNextRandomPiece();
	///* initialize random seed: */
	//srand(time(NULL));

	//int iter = 0;
	///* generate secret number between 1 and 10: */
	//do
	//{
	//	int x, y;
	//	//if (iter == 0)
	//	//{
	//	//	x = 2;
	//	//	y = 3;
	//	//}
	//	//else
	//	//{
	//	//	x = 3;
	//	//	y = 3;
	//	//}
	//	x = rand() % boardSize;
	//	y = rand() % boardSize;

	//	Piece* p = GetPiece(x, y);
	//	p->SetScore(2);
	//	iter++;
	//} while (iter < 2);

}

Piece* Board::GetPiece(int x, int y)
{
	Piece* temp = nullptr;

	if (x >= boardSize || y >= boardSize || x < 0 || y < 0) {
		return temp;
	}

	temp = Pieces_front;

	for (int i = 0; i < y * boardSize + x; i++)
	{
		temp = temp->nextPiece;
	}

	return temp;
}

void Board::DrawCoordinates()
{
	Piece* temp = Pieces_front;
	int itr = 0;
	do
	{
		temp->DrawCoordinates();

		if (itr == boardSize - 1)
		{
			cout << endl;
			itr = 0;
		}
		else
		{
			cout << " ";
			itr++;
		}

		temp = temp->nextPiece;

	} while (temp != nullptr);
}

void Board::DrawScores()
{
	Piece* temp = Pieces_front;
	int itr = 0;
	do
	{
		temp->DrawScore();

		if (itr == boardSize - 1)
		{
			cout << endl;
			itr = 0;
		}
		else
		{
			cout << " ";
			itr++;
		}

		temp = temp->nextPiece;

	} while (temp != nullptr);
}

void Board::Swipe(Direction direction)
{
	if (direction != Unknown)
	{
		ProcessRow(direction);
	}

	SetNextRandomPiece();
}

void Board::GenerateBoard()
{
	int y = 0;
	int x = 0;

	Piece* temp;
	Piece* prev_piece;

	Pieces_front = new  Piece(x, y);
	temp = Pieces_front;
	prev_piece = Pieces_front;

	x++;

	for (int i = 0; i < boardSize * boardSize; i++)
	{
		if (i == 0) { continue; } //we already created this piece....
		if (i % boardSize == 0) { y++; x = 0; } //simulate new row

		temp = new Piece(x, y);
		temp->SetScore(0);
		temp->SetId(i);
		temp->prevPiece = prev_piece;

		prev_piece->nextPiece = temp;
		prev_piece = temp;

		x++;
	}

	Pieces_back = temp;
	//temp = Pieces_front;

	//do
	//{
	//	if (Pieces_back == nullptr)
	//	{
	//		prev_piece;
	//	}
	//	temp = temp->nextPiece;

	//} while (temp != nullptr);
}

void Board::ProcessRow(Direction dir)
{
	Piece* curPiece = Pieces_front;
	Piece* nextPiece = Pieces_front;

	int curPieceGetAfter = 0;
	int nextPieceGetAfter = 0;

	/* remember that they are all linked left to right
				[0, 0][1, 0][2, 0][3, 0]
				[0, 1][1, 1][2, 1][3, 1]
				[0, 2][1, 2][2, 2][3, 2]
				[0, 3][1, 3][2, 3][3, 3]

				[0, 0][1, 0][2, 0][3, 0][0, 1][1, 1][2, 1][3, 1][0, 2][1, 2][2, 2][3, 2][0, 3][1, 3][2, 3][3, 3]
	*/

	//iterate through the board to do the processing
	int innerIter = 0;
	int outerIter = 0;

	//prep the loop
	switch (dir)
	{
	case Right:
		nextPiece = Pieces_front->GetAfter(1);
		curPiece = Pieces_front;
		break;
	case Down:
		nextPiece = Pieces_front->GetAfter(boardSize);
		curPiece = Pieces_front;
		break;
	case Left:
		nextPiece = Pieces_back->GetAfter(-1);
		curPiece = Pieces_back;
		break;
	case Up:
		nextPiece = Pieces_back->GetAfter(-boardSize);
		curPiece = Pieces_back;
		break;
	}
	   
	do
	{
		innerIter = 0;
		do
		{
			if (curPiece == nextPiece)
			{
				break;
			}

			if (nextPiece->GetScore() == 0)
			{
				nextPiece->SetScore(curPiece->GetScore());
				curPiece->SetScore(0);
			}
			else if (curPiece->GetScore() != 0 && nextPiece->GetScore() != 0 && curPiece->GetScore() == nextPiece->GetScore())
			{
				nextPiece->SetScore(curPiece->GetScore() * 2);
				curPiece->SetScore(0);
			}
			else
			{
				//else check if any valid moves exist??
			}
			

			curPiece = nextPiece;
			switch (dir)
			{
			case Right:
				nextPiece = nextPiece->GetAfter(1);
				break;
			case Down:
				nextPiece = nextPiece->GetAfter(boardSize);
				break;
			case Left:
				nextPiece = nextPiece->GetAfter(-1);
				break;
			case Up:
				nextPiece = nextPiece->GetAfter(-boardSize);
				break;
			}

			innerIter++;

		} while (innerIter < boardSize - 1);

		outerIter++;

		switch (dir)
		{
		case Right:
			curPiece = Pieces_front->GetAfter(boardSize * outerIter);
			nextPiece = curPiece->GetAfter(1);
			break;
		case Down:
			curPiece = Pieces_front->GetAfter(outerIter);
			nextPiece = curPiece->GetAfter(boardSize);
			break;
		case Left:
			curPiece = Pieces_back->GetAfter(-boardSize * outerIter);
			nextPiece = curPiece->GetAfter(-1);
			break;
		case Up:
			curPiece = Pieces_back->GetAfter(-outerIter -1);
			nextPiece = curPiece->GetAfter(-boardSize);
			break;
		}
		
		
	} while (outerIter < boardSize);
}

void Board::SetNextRandomPiece()
{
	bool isSet = false;

	while (!isSet)
	{
		srand(time(NULL));

		Piece* t = GetPiece(rand() % boardSize, rand() % boardSize);
		if (t->GetScore() == 0)
		{
			t->SetScore(smallestNumer);
			isSet = true;
		}
	}
}

bool Board::ProcessNumber(int num)
{
	if (num == 1 ||	num % 2 == 0)
	{
		return true;
	}

	return false;
}