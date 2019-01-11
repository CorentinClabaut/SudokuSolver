#include "GridLoader.hpp"

using namespace sudoku;

Grid GridLoader::GetHardcodedGrid() const
{
    Grid grid(9);

    grid.m_Cells[0][3]->SetValue(9);
    grid.m_Cells[0][5]->SetValue(2);
    grid.m_Cells[1][1]->SetValue(4);
    grid.m_Cells[1][7]->SetValue(5);
    grid.m_Cells[2][2]->SetValue(2);
    grid.m_Cells[2][6]->SetValue(3);
    grid.m_Cells[3][0]->SetValue(2);
    grid.m_Cells[3][8]->SetValue(7);
    grid.m_Cells[4][3]->SetValue(4);
    grid.m_Cells[4][4]->SetValue(5);
    grid.m_Cells[4][5]->SetValue(6);
    grid.m_Cells[5][0]->SetValue(6);
    grid.m_Cells[5][8]->SetValue(9);
    grid.m_Cells[6][2]->SetValue(7);
    grid.m_Cells[6][6]->SetValue(8);
    grid.m_Cells[7][1]->SetValue(3);
    grid.m_Cells[7][7]->SetValue(4);
    grid.m_Cells[8][3]->SetValue(2);
    grid.m_Cells[8][5]->SetValue(7);

    return grid;
}
