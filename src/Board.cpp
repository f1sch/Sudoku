#include "Board.h"

#include <vector>

Board::Board()
{
	// placeholder for testing
	//int testBoard[9][9] = 
	//{
	//	{ 5, 3, 0, 0, 7, 0, 0, 0, 0 },
	//	{ 6, 0, 0, 1, 9, 5, 0, 0, 0 },
	//	{ 0, 9, 8, 0, 0, 0, 0, 6, 0 },
	//	{ 8, 0, 0, 0, 6, 0, 0, 0, 3 },
	//	{ 4, 0, 0, 8, 0, 3, 0, 0, 1 },
	//	{ 7, 0, 0, 0, 2, 0, 0, 0, 6 },
	//	{ 0, 6, 0, 0, 0, 0, 2, 8, 0 },
	//	{ 0, 0, 0, 4, 1, 9, 0, 0, 5 },
	//	{ 0, 0, 0, 0, 8, 0, 0, 7, 9 }
	//};
	int testBoard[9][9] = 
	{
		{ 0, 2, 3, 4, 5, 6, 7, 8, 9 },
		{ 4, 5, 6, 7, 8, 9, 1, 2, 3 },
		{ 7, 8, 9, 1, 2, 3, 4, 5, 6 },
		{ 2, 3, 1, 6, 7, 4, 8, 9, 5 },
		{ 8, 7, 5, 9, 1, 2, 3, 6, 4 },
		{ 6, 9, 4, 5, 3, 8, 2, 1, 7 },
		{ 3, 1, 7, 2, 6, 5, 9, 4, 8 },
		{ 5, 4, 2, 8, 9, 7, 6, 3, 1 },
		{ 9, 6, 8, 3, 4, 1, 5, 7, 2 }
	};
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			m_cells[r][c].number = testBoard[r][c];
			m_cells[r][c].note.clear();
			m_cells[r][c].canEdit = (testBoard[r][c] == 0);
		}
	}
}


Cell Board::GetCell(int row, int col) const
{
	return m_cells[row][col];
}

void Board::SetCell(int row, int col, int num)
{
	if (m_cells[row][col].canEdit)
	{
		m_cells[row][col].number = num;
	}
}

bool Board::IsSolved() const
{
	std::vector<std::vector<bool>> rows(9, std::vector<bool>(9, false));
	std::vector<std::vector<bool>> cols(9, std::vector<bool>(9, false));
	std::vector<std::vector<bool>> block(9, std::vector<bool>(9, false));

	for (int row = 0; row < 9; ++row)
	{
		for (int col = 0; col < 9; ++col)
		{
			int currentCellNumber = m_cells[row][col].number;
			if (currentCellNumber == 0)
				return false;

			int digitIndex = currentCellNumber - 1;
			int blockIndex = (row / 3) * 3 + (col / 3);
			
			if (rows[row][digitIndex] ||
				cols[col][digitIndex] ||
				block[blockIndex][digitIndex])
			{
				return false;
			}

			rows[row][digitIndex] = true;
			cols[col][digitIndex] = true;
			block[blockIndex][digitIndex] = true;
		}
	}
	return true;
}
