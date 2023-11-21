#ifndef SOLVER_H_
#define SOLVER_H_

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Action.h"
#include "Block.h"
#include "Goal.h"
#include "Search_Node.h"
#include "State.h"

class Solver {
private:
	State* state_initial;
	Goal goal;

	int steps_current;
	int steps_max;

	bool goal_found;

	unordered_set<State*> states_visited;
	Search_Node* node_final;

public:
	Solver(State* s) {
		state_initial = s;

		steps_current = 0;
		steps_max = 100;

		goal_found = false;

		node_final = new Search_Node{ state_initial, nullptr, steps_current };
	}

	inline virtual void solve();
	
	inline Goal generate_goal();

	inline virtual bool search_steps_limited(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max);

	inline virtual void final_node_store(Search_Node* node);

	inline virtual Search_Node* get_node_final();
};

inline void Solver::solve() {
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

inline Goal Solver::generate_goal() {
	Goal goal_temp;
	goal_temp.input();
	goal_temp.print();

	return goal_temp;
}

inline bool Solver::search_steps_limited(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max){
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

	// Check if steps_max hit.
	if (node->steps_total >= steps_max) {
		return false;
	}

	// Putting this state as a visited state.
	visited.insert(node->state);

	// Generating legal actions for the current node.
	node->state->legal_actions_generate();
	priority_queue<Action> legal_actions = node->state->get_legal_actions();

	// The action that is to be taken.
	Action next_action = legal_actions.top();
	legal_actions.pop();

	State* next_state = node->state->next_state(next_action);
	int next_cost = node->steps_total + next_action.steps_cost;

	// Check to see if already visited.
	if (visited.find(next_state) == visited.end()) {
		Search_Node* next_node = new Search_Node{ next_state, node, next_cost };

		if (search_steps_limited(visited, next_node, g, steps_max)) {
			return true;
		}
	}

	return false;
}

inline void Solver::final_node_store(Search_Node* node) {
	node_final = node;
}

inline Search_Node* Solver::get_node_final() {
	return node_final;
}

#endif /* SOLVER_H_ */