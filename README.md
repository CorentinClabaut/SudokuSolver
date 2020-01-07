# Sudoku solver optimised for speed 

## What's a Sudoku?

https://en.wikipedia.org/wiki/Sudoku

## Purpose of the project
* Write a clean and well tested Sudoku solver, with efficiency in mind. 
* Play with Optimisation and multithreading while trying to keep the code easy to understand. 
* Play with latest version of C++ as well as libraries like boost & Gtest/GMock.

## Solver algorithm

To solve a grid, the solver keeps track of all the possible values of every cells. 

Whenever a cell is set, all the related cells' possible values are updated accordingly. 

A new cell is set when:
* it only has one possible value left
* it is the only one that can be set to a specific value in one of its sub sections

When no more cells can be set that way, the program needs to make hypothesis to finish solving a grid.

An hypothesis is made by setting a cell with one of its remaining value. 
If the grid end up being invalid after that, the solver goes back to this hypothesis and try another of the remaining possible values, until a correct value is used.

## Project structure
* Solver library - Contains the logic to solve the Sudoku grids
* Test executable - Executable containing the unit and functional tests for the Solver library
* Benchmark executable - Executable using the Solver library to measure how fast it can solve a set of Sudoku grids

## Benchmark

Measure median time to solve different Sudoku grids. 

Each of the grids have 20 randomly chosen original cells set.

## Optimisations

Initial implementation's median time to solve a Sudoku: 28'857 us

| Improvement        | Median (us)           |
| ------------- |:-------------:| 
| Improved the way the detection of wrong new cell set was made      | 11'970 | 
| Used bitset instead of unordered_set in the class tracking the Cell's remaining possibilities       | 5'706      | 
| Used thread pool instead of always creating/deleting new threads | 2'648      | 
| Used Cells instead of shared_ptr of Cells in Grid to avoid indirection | 1'935      |
| Stopped using multithreading because the overhead is too big | 754      | 
| Acquired list of related positions at compile time | 441      | 
| Created lookup table to get a cell's number of remaining possibilities at compile time | 313      | 
| Improved algorithm that detects when a cell is the only one in its subsection that can contain a value | 132      | 


## Technologies

C++17, Boost, GMock/GTest, Google profiling tool, Multithreading, Optimisation


