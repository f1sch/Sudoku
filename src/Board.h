#pragma once
#include "SudokuBoards.h"

#include <set>

struct Cell
{
	int number;
	std::set<int> note;
	bool canEdit;
};

class Board
{
public:
	explicit Board(const Board9x9& sudoku);

	Cell getCell(int row, int col) const;
	void setCell(int row, int col, int num);
	bool isSolved() const;
private:
	Cell m_cells[9][9];
};