#include "FoundCellsEnqueuer.hpp"

#include "FoundCells.hpp"

using namespace sudoku;

FoundCellsEnqueuerImpl::FoundCellsEnqueuerImpl(std::shared_ptr<FoundCells> foundCells) :
    m_FoundCells(foundCells)
{}

void FoundCellsEnqueuerImpl::Enqueue(SharedCell cell)
{
    std::lock_guard<std::mutex> l(m_FoundCells->m_Mutex);
    m_FoundCells->m_Queue.push(cell);
}
