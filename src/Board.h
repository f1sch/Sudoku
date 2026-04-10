#pragma once
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
	Board();

	Cell GetCell(int row, int col) const;
	void SetCell(int row, int col, int num);
	bool IsSolved() const;
private:
	Cell m_cells[9][9];
};