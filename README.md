# Mini-SHRDLU Game Solver

This project is an implementation of the Mini-SHRDLU game, a computer game inspired by the SHRDLU program created in the late 1960s at MIT AI Laboratory. The goal of this project was to design a computer program that can automatically generate a solution to the Mini-SHRDLU game with any board size, n, and number of blocks, k, within certain constraints.

## Overview

The program allows users to interact with a Mini-SHRDLU game, where the goal is to move numbered blocks within an n * n vertically suspended board. The blocks must be placed in columns, next to each other down to the bottom in each column. The program generates an initial state of the board randomly and provides functionalities to execute legal actions, find a sequence of actions to reach a goal state, and display the game board state.

## Features

- **State Representation**: The game state is represented using appropriate data structures.
- **Random Initialization**: Generates a random initial state for the game board.
- **Legal Action Execution**: Executes legal actions to move blocks between columns.
- **Search Algorithm**: Implements an A* search algorithm to find a goal state.
- **Heuristic Functionality**: Includes a heuristic function to prioritize actions for more efficient goal achievement.
- **Supports Multiple Goals**: Handles disjunctive and conjunctive goals.
- **Scalability**: The program is designed to handle larger board sizes and variable block counts.

## Usage

To use the program:

1. Compile the source code using an appropriate compiler for C++, Java, or Python.
2. Run the executable or program file.
3. Follow the on-screen prompts to interact with the Mini-SHRDLU game.

## Requirements

The implementation covers different levels of complexity, from basic functionality to advanced features, as specified in the original assignment requirements:

- **Pass Level**: Basic functionalities including state representation, random initialization, legal action execution, and basic goal achievement.
- **Credit Level**: Addition of heuristic function to improve search algorithm efficiency.
- **Distinction Level**: Solving multiple goals and checking for consistency among conjunctive goals.
- **High Distinction Level**: Generalizing the program for larger board sizes and comprehensive complexity analysis.

## Demonstration

The project includes a demonstration of the program, showcasing its functionalities and adherence to the assignment requirements. The demonstration involves showcasing the implemented features, running the program, and explaining the codebase and design choices.

It also has an A* search feature.

