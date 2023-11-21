#ifndef GOAL_H_
#define GOAL_H_

#include "Constants.h"

struct Goal {
	int block;
	int row;
	int col;
	int heuristic;
	
	Goal() {
		block = NULL;
		row = NULL;
		col = NULL;
		heuristic = NULL;
	}

	inline bool operator==(Goal g) {
		if (g.block == block && g.row == row && g.col == col) {
			return false;
		}

		return true;
	}

	inline bool operator!=(Goal g) {
		if (g.block == block && g.row == row && g.col == col) {
			return true;
		}

		return false;
	}

	inline bool operator<(const Goal g) const {
		if (heuristic < g.heuristic) {
			return true;
		}
		else {
			return false;
		}
	}

	inline virtual void input();
	// To input a singular goal.

	inline virtual void print();
	// To just print the details of the goal, including heuristic if available.
};

inline void Goal::input() {
	cout << "State your goal_blo: ";
	cin >> block;

	cout << "State your goal_row: ";
	cin >> row;

	cout << "State your goal_col: ";
	cin >> col;
}

inline void Goal::print() {
	if (heuristic != NULL) {
		cout << "Goal (" << block << "," << row << "," << col << ") with a h of " << heuristic << endl;
	}
	else {
		cout << "Goal (" << block << "," << row << "," << col << ")" << endl;
	}
	cout << endl;
}

#endif /* GOAL_H_ */