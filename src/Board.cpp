#include "Board.h"
#include <set>

Board::Board()
{
	// placeholder for testing
	int testBoard[9][9] = 
	{
		{ 5, 3, 0, 0, 7, 0, 0, 0, 0 },
		{ 6, 0, 0, 1, 9, 5, 0, 0, 0 },
		{ 0, 9, 8, 0, 0, 0, 0, 6, 0 },
		{ 8, 0, 0, 0, 6, 0, 0, 0, 3 },
		{ 4, 0, 0, 8, 0, 3, 0, 0, 1 },
		{ 7, 0, 0, 0, 2, 0, 0, 0, 6 },
		{ 0, 6, 0, 0, 0, 0, 2, 8, 0 },
		{ 0, 0, 0, 4, 1, 9, 0, 0, 5 },
		{ 0, 0, 0, 0, 8, 0, 0, 7, 9 }
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
