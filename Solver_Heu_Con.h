#ifndef SOLVER_HEU_CON_H_
#define SOLVER_HEU_CON_H_

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Solver_Heu.h"

class Solver_Heu_Con : public Solver_Heu {
private:
	State* state_initial;
	priority_queue<Goal> goals;

	int steps_current;
	int steps_max;

	bool goal_found;

	unordered_set<State*> states_visited;
	Search_Node* node_final;
	
	int number_of_goals;

	vector<Goal> goals_completed;

public:
	Solver_Heu_Con(State* s) : Solver_Heu(s) {
		state_initial = s;

		steps_current = 0;
		steps_max = 100;

		goal_found = false;

		node_final = new Search_Node{ state_initial, nullptr, steps_current };

		number_of_goals = 0;
	}

	inline void solve() override;

	inline priority_queue<Goal> generate_goal_multiple_con();

	inline Goal generate_goal_con();

	inline int heuristic_goal(State* s, Goal g);

	inline bool solve_attempt(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max);

	inline priority_queue<Action> actions_apply_heuristics(priority_queue<Action>& actions, Search_Node* node, Goal g) override;

	inline int heuristic_con(State* s, Goal g);
};

inline void Solver_Heu_Con::solve() {
	Search_Node* root = new Search_Node{ state_initial, nullptr, steps_current };
	
	// Just to show the initial state of the puzzle, and to allow for more fun goal selecting.
	cout << "Current Step: " << steps_current << endl;
	state_initial->print();
	
	// To generate goal(s).
	goals = generate_goal_multiple_con();
	
	// To address possible loss of data.
	number_of_goals = static_cast<int>(goals.size());

	for (int i = 0; i < number_of_goals; i++) {
		Goal goal;
		goal = goals.top();
		goal.print();

		// visited:			already initialised in private
		// node:			made root
		// g:				made goal
		// steps_max:		already initialised in constructor

		if (solve_attempt(states_visited, root, goal, steps_max)) {
			goals_completed.push_back(goals.top());
			goals.pop();

			node_final = get_node_final();
			root = node_final;
		}
		else {
			break;
		}
	}

	if (number_of_goals != goals_completed.size()) {
		cout << "Could not solve for all goals." << endl;
	}
	else {
		cout << "Solved and it took " << node_final->steps_total << " steps." << endl;
	}
}

inline priority_queue<Goal> Solver_Heu_Con::generate_goal_multiple_con() {
	cout << "How many conjunctive goals will you have? ";
	cin >> number_of_goals;

	vector<Goal> goals_temp_one;			// To store user input goals.
	queue<int> blocks_temp;					// To store the block numbers still available to use.

	priority_queue<Goal> goals_temp_two;	// To store user input goals AND other goals.

	// Get all user goals first.
	for (int i = 0; i < number_of_goals; i++) {
		Goal goal_temp;

		cout << "Conjunctive Goal [" << i + 1 << "]: " << endl;
		goal_temp = generate_goal_con();
		goals_temp_one.push_back(goal_temp);
	}

	for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
		bool block_in_use = false;

		for (int j = 0; j < goals_temp_one.size(); j++) {
			// This means that this block number is in use.
			if (i + 1 == goals_temp_one.at(j).block) {
				block_in_use = true;
				break;
			}
		}

		if (block_in_use == false) {
			blocks_temp.push(i + 1);
		}
	}

	// Establish foundation with non-user goals.
	State state_temp;
	state_temp.generate_state_blank();
	
	// Inputting goals.
	for (int i = 0; i < goals_temp_one.size(); i++) {
		state_temp.place_goal(goals_temp_one.at(i));
	}

	// Building foundation.
	for (int col = 0; col < BOARDSIZE; col++) {
		// Getting all the rows that are free.
		queue<int> queue_temp;
		queue_temp = state_temp.find_rows_of_free_spots_below_highest_block(col);

		// To address possible loss of data.
		int queue_temp_size = static_cast<int>(queue_temp.size());

		if (queue_temp.size() == 0) {
			continue;
		}

		for (int i = 0; i < queue_temp_size; i++) {
			// Time to make goals.
			Goal goal_temp;

			goal_temp.block = blocks_temp.front();
			blocks_temp.pop();

			goal_temp.row = queue_temp.front();
			queue_temp.pop();

			goal_temp.col = col;

			goal_temp.print();

			goals_temp_one.push_back(goal_temp);
		}
	}

	vector<Goal> goals_temp_three;

	// Transferring objects from vector to priority_queue. Apply heuristics.
	while (!goals_temp_one.empty()) {
		Goal goal_temp;
		goal_temp = goals_temp_one.back();

		goals_temp_one.pop_back();

		goal_temp.heuristic = heuristic_goal(state_initial, goal_temp);
		goals_temp_two.push(goal_temp);
		goals_temp_three.push_back(goal_temp);
	}

	return goals_temp_two;
}

inline Goal Solver_Heu_Con::generate_goal_con() {
	bool goal_shares_block;
	bool goal_shares_location;

	Goal goal_temp;
	
	// Check this goal against all goals so far... if they are the same, then do it again.
	do {
		goal_shares_block = false;
		goal_shares_location = false;

		priority_queue<Goal> goals_temp;
		
		goals_temp = goals;

		goal_temp.input();

		for (int i = 0; i < goals.size(); i++) {
			while (!goals_temp.empty()) {
				Goal goal_to_check = goals_temp.top();
				goals_temp.pop();

				if (goal_temp.block == goal_to_check.block) {
					goal_shares_block = true;
					break;
				}
				if (goal_temp.row == goal_to_check.row && goal_temp.col == goal_to_check.col) {
					goal_shares_location = true;
					break;
				}
			}
		}

		if (goal_shares_block == true) {
			cout << "Error: Input shares same block as previous goal." << endl;
		}
		if (goal_shares_location == true) {
			cout << "Error: Input shares same location as previous goal." << endl;
		}
	} while (goal_shares_block == true || goal_shares_location == true);

	goal_temp.print();

	return goal_temp;
}

inline int Solver_Heu_Con::heuristic_goal(State* s, Goal g) {
	int value = 0;

	// More points if the goal.row is lower.
	int number_one = BOARDSIZE - g.row;

	value = value + 100 * number_one;

	// More points if the goal col is different to target col.
	if (g.col != s->find_location(g.block).col) {
		value = value + 10;
	}

	// More points if the goal location has fewer blocks above it.
	int number_two = BOARDSIZE - s->find_number_of_blocks_above(s->get_block(g.row, g.col));

	value = value + 1 * number_two;

	return value;
}

inline bool Solver_Heu_Con::solve_attempt(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max) {
	if (search_steps_limited(states_visited, node, g, steps_max)) {
		return true;
	}
	else {
		return false;
	}
}

inline priority_queue<Action> Solver_Heu_Con::actions_apply_heuristics(priority_queue<Action>& a, Search_Node* node, Goal g) {
	priority_queue<Action> queue_temp;

	while (!a.empty()) {
		Action action_temp = a.top();
		a.pop();

		action_temp.heuristic = heuristic_con(node->state->next_state(action_temp), g);
		action_temp.heuristic = action_temp.heuristic + heuristic(node->state->next_state(action_temp), g);

		queue_temp.push(action_temp);
	}

	return queue_temp;
}

inline int Solver_Heu_Con::heuristic_con(State* s, Goal g) {
	int value = 0;
	
	// Check previous goals.
	for (int i = 0; i < goals_completed.size(); i++) {
		Goal temp_goal;

		temp_goal = goals_completed.at(i);

		if (s->get_block(temp_goal.row, temp_goal.col) == temp_goal.block) {
			value = value + 1000000;
		}
	}

	return value;
}

#endif /* SOLVER_HEU_CON_H_ */