#ifndef SOLVER_HEU_H_
#define SOLVER_HEU_H_

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Solver.h"

class Solver_Heu : public Solver {
private:
	State* state_initial;
	Goal goal;

	int steps_current;
	int steps_max;
	
	bool goal_found;

	unordered_set<State*> states_visited;
	Search_Node* node_final;

public:
	Solver_Heu(State* s) : Solver(s) {
		state_initial = s;

		steps_current = 0;
		steps_max = 100;

		goal_found = false;

		node_final = new Search_Node{ state_initial, nullptr, steps_current };
	}

	inline virtual void solve() override;

	inline bool search_steps_limited(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max) override;

	inline virtual priority_queue<Action> actions_apply_heuristics(priority_queue<Action>& actions, Search_Node* node, Goal g);

	inline int heuristic(State* s, Goal g);

	inline virtual void final_node_store(Search_Node* node);

	inline Search_Node* get_node_final();
};

inline void Solver_Heu::solve() {
	Search_Node* root = new Search_Node{ state_initial, nullptr, steps_current };
	
	// Just to show the initial state of the puzzle, and to allow for more fun goal selecting.
	cout << "Current Step: " << steps_current << endl;
	state_initial->print();

	// To generate goal(s).
	goal = generate_goal();

	// visited:			already initialised in private
	// node:			made root
	// g:				made goal
	// steps_max:		already initialised in constructor

	if (search_steps_limited(states_visited, root, goal, steps_max)) {
		cout << "Solved and it took " << node_final->steps_total << " steps." << endl;
	}
	else {
		cout << "Goal not found within the " << steps_max << " step limit." << endl;
	}
}

inline bool Solver_Heu::search_steps_limited(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max) {
	// Only print if not the first (because already printed earlier).
	if (node->steps_total != 0) {
		cout << "Current Step: " << node->steps_total << endl;
		node->state->print();
	}

	// Check if goal found.
	if (node->state->get_block(g.row, g.col) == g.block) {
		final_node_store(node);
		return true;
	}

	// Check if steps max hit.
	if (node->steps_total >= steps_max) {
		return false;
	}

	// Putting this state as a visited state.
	visited.insert(node->state);

	// Generating legal actions for the current node.
	node->state->legal_actions_generate();
	priority_queue<Action> legal_actions = node->state->get_legal_actions();

	// Applying heuristics to everything inside the priority_queue before doing any actions.
	priority_queue<Action> legal_actions_w_h = actions_apply_heuristics(legal_actions, node, g);

	// The action that is to be taken.
	Action next_action = legal_actions_w_h.top();
	legal_actions_w_h.pop();

	State* next_state = node->state->next_state(next_action);
	int next_cost = node->steps_total + next_action.steps_cost;

	// Checking to see if already explored.
	if (visited.find(next_state) == visited.end()) {
		Search_Node* next_node = new Search_Node{ next_state, node, next_cost };
		steps_current++;

		if (search_steps_limited(visited, next_node, g, steps_max)) {
			return true;
		}
	}

	return false;
}

inline priority_queue<Action> Solver_Heu::actions_apply_heuristics(priority_queue<Action>& a, Search_Node* node, Goal g) {
	priority_queue<Action> queue_temp;

	while (!a.empty()) {
		Action action_temp = a.top();
		a.pop();

		action_temp.heuristic = heuristic(node->state->next_state(action_temp), g);

		queue_temp.push(action_temp);
	}

	return queue_temp;
}

inline int Solver_Heu::heuristic(State* s, Goal g) {
	int value = 0;

	// Check if target block in target location.
	if (s->get_block(g.row, g.col) == g.block) {
		value = value + 1000000;
	}
	
	// Check if already on top of a col.
	if (s->find_location(g.block).row == BOARDSIZE - 1) {
		value = value + 100000;
	}

	// If not, then build a column of size (BOARDSIZE-1).
	if (s->find_location(g.block).row != BOARDSIZE - 1) {
		int column_highest = 0;

		for (int i = 0; i < BOARDSIZE; i++) {
			// Only count the non-dst cols.
			if (i == s->find_location(g.block).col) {
				continue;
			}

			// Only count the non-full cols.
			if (s->find_number_of_blocks_in_col(i) == BOARDSIZE) {
				continue;
			}

			int column_highest_temp = s->find_number_of_blocks_in_col(i);

			if (column_highest < column_highest_temp) {
				column_highest = column_highest_temp;
			}
		}

		value = value + 10000 * column_highest;
	}
	
	// Check if goal block is free.
	if (s->find_number_of_blocks_above(g.block) == 0) {
		value = value + 1000;
	}

	// If not, then make it free.
	if (s->find_number_of_blocks_above(g.block) != 0) {
		int number = BOARDSIZE - s->find_number_of_blocks_above(g.block);

		value = value + 100 * number;
	}

	// Check if goal location is free.
	if (s->get_block(g.row, g.col) == 0) {
		value = value + 10;
	}

	// If not, then make it free.
	if (s->get_block(g.row, g.col) != 0) {
		int number = BOARDSIZE - s->find_number_of_blocks_above(s->get_block(g.row, g.col));

		value = value + 1 * number;
	}

	return value;
}

inline void Solver_Heu::final_node_store(Search_Node* node) {
	node_final = node;
}

inline Search_Node* Solver_Heu::get_node_final() {
	return node_final;
}

#endif /* SOLVER_HEU_H_ */