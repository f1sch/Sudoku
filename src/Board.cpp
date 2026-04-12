#include "Board.h"
#include "SudokuBoards.h"

#include <vector>

Board::Board(const Board9x9& sudoku)
{
	for (int r = 0; r < 9; ++r)
	{
		for (int c = 0; c < 9; ++c)
		{
			m_cells[r][c].number = sudoku[r][c];
			m_cells[r][c].note.clear();
			m_cells[r][c].canEdit = (sudoku[r][c] == 0);
		}
	}
}


Cell Board::getCell(int row, int col) const
{
	return m_cells[row][col];
}

void Board::setCell(int row, int col, int num)
{
	if (m_cells[row][col].canEdit)
	{
		m_cells[row][col].number = num;
	}
}

bool Board::isSolved() const
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
