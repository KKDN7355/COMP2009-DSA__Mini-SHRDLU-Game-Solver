#ifndef SOLVER_HEU_A_STAR_H_
#define SOLVER_HEU_A_STAR_H_

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Solver_Heu.h"

// This is to allow for comparisons of (Search_Node*)s in the priority_queue.
struct CompareNodes {
	bool operator()(const Search_Node* lhs, const Search_Node* rhs) {
		return lhs->steps_total > rhs->steps_total;
	}
};

class Solver_Heu_A_Star : public Solver_Heu {
private:
	State* state_initial;
	Goal goal;

	int steps_current;
	int steps_max;

	bool goal_found;

	unordered_set<State*> states_visited;
	Search_Node* node_final;

	priority_queue<Search_Node*, vector<Search_Node*>, CompareNodes> nodes_to_search;

public:
	Solver_Heu_A_Star(State* s) : Solver_Heu (s) {
		state_initial = s;

		steps_current = 0;
		steps_max = 100;

		goal_found = false;

		node_final = new Search_Node{ state_initial, nullptr, steps_current };
	}

	inline void solve() override;
	inline bool search_steps_limited_a_star(unordered_set<State*>& visited, priority_queue<Search_Node*, vector<Search_Node*>, CompareNodes>& nodes_to_search, Goal g, int steps_max);
};

inline void Solver_Heu_A_Star::solve() {	
	Search_Node* root = new Search_Node{ state_initial, nullptr, steps_current };
	nodes_to_search.push(root);

	// Just to show the initial state of the puzzle, and to allow for more fun goal selecting.
	cout << "Current Step: " << steps_current << endl;
	root->state->print();

	// To generate goal(s).
	goal = generate_goal();

	if (search_steps_limited_a_star(states_visited, nodes_to_search, goal, steps_max)) {
		cout << "Solved and it took " << steps_current << " steps." << endl;
		node_final = get_node_final();
		node_final->state->print();
	}
	else {
		cout << "Goal not found within step limit." << endl;
	}
}

inline bool Solver_Heu_A_Star::search_steps_limited_a_star(unordered_set<State*>& visited, priority_queue<Search_Node*, vector<Search_Node*>, CompareNodes>& nodes_to_search, Goal g, int steps_max) {
	while (!nodes_to_search.empty()) {
		Search_Node* node = nodes_to_search.top();
		nodes_to_search.pop();

		steps_current = node->steps_total;

		// Only print if not the first (because already printed earlier).
		if (steps_current != 0) {
			cout << "Current Step: " << node->steps_total << endl;
			nodes_to_search.top()->state->print();
		}

		// Check if goal found.
		if (node->state->get_block(g.row, g.col) == g.block) {
			final_node_store(node);
			return true;
		}

		// Check if steps max hit, if so skip.
		if (steps_current >= steps_max) {
			continue;
		}

		// Putting this state as a visited state.
		visited.insert(node->state);

		// Generating legal actions for the current node.
		node->state->legal_actions_generate();
		priority_queue<Action> legal_actions = node->state->get_legal_actions();

		// Applying heuristics to everything inside the priority_queue before doing any actions.
		priority_queue<Action> legal_actions_w_h = actions_apply_heuristics(legal_actions, node, g);

		while (!legal_actions_w_h.empty()) {
			// The action that is to be taken.
			Action next_action = legal_actions_w_h.top();
			legal_actions_w_h.pop();

			State* next_state = node->state->next_state(next_action);
			int next_cost = node->steps_total + next_action.steps_cost;
			int next_heuristic = node->heuristic;

			if (visited.find(next_state) == visited.end()) {
				Search_Node* next_node = new Search_Node{ next_state, node, next_cost, next_heuristic };
				nodes_to_search.push(next_node);
			}
		}
	}

	return false;
}

#endif /* SOLVER_HEU_A_STAR_H_ */