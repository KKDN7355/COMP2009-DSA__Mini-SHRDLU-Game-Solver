#ifndef SOLVER_HEU_DIS_H_
#define SOLVER_HEU_DIS_H_

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Solver_Heu.h"

class Solver_Heu_Dis : public Solver_Heu {
private:
	State* state_initial;
	priority_queue<Goal> goals;

	int steps_current;
	int steps_max;

	bool goal_found;

	unordered_set<State*> states_visited;
	Search_Node* node_final;
	
	int number_of_goals;

public:
	Solver_Heu_Dis(State* s) : Solver_Heu(s) {
		state_initial = s;

		steps_current = 0;
		steps_max = 100;

		goal_found = false;

		node_final = new Search_Node{ state_initial, nullptr, steps_current };

		number_of_goals = 0;
	}

	inline void solve() override;
	
	inline priority_queue<Goal> generate_goal_multiple_dis();
	
	inline bool solve_attempt(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max);
};

inline void Solver_Heu_Dis::solve() {
	Search_Node* root = new Search_Node{ state_initial, nullptr, steps_current };
	
	// Just to show the initial state of the puzzle, and to allow for more fun goal selecting.
	cout << "Current Step: " << steps_current << endl;
	state_initial->print();

	// To generate goal(s).
	goals = generate_goal_multiple_dis();

	for (int i = 0; i < number_of_goals; i++) {
		Goal goal;
		goal = goals.top();
		goal.print();
		
		// visited:			already initialised in private
		// node:			made root
		// g:				made goal
		// steps_max:		already initialised in constructor

		if (solve_attempt(states_visited, root, goal, steps_max)) {
			cout << "This goal was achieved:" << endl;
			goal.print();
			break;
		}
		else {
			goals.pop();
		}
	}

	node_final = get_node_final();

	if (goals.size() == 0) {
		cout << "None of the goals were found within steps limit." << endl;
	}
	else {
		cout << "Solved and it took " << node_final->steps_total << " steps." << endl;
	}
}

inline priority_queue<Goal> Solver_Heu_Dis::generate_goal_multiple_dis() {
	cout << "How many disjunctive goals will you have?" << endl;
	cin >> number_of_goals;

	priority_queue<Goal> goals_temp;

	for (int i = 0; i < number_of_goals; i++) {
		Goal goal_temp;

		cout << "Disjunctive Goal [" << i + 1 << "]: " << endl;
		goal_temp = generate_goal();
		goals_temp.push(goal_temp);
	}

	return goals_temp;
}

inline bool Solver_Heu_Dis::solve_attempt(unordered_set<State*>& visited, Search_Node* node, Goal g, int steps_max) {
	if (search_steps_limited(states_visited, node, g, steps_max)) {
		return true;
	}
	else {
		return false;
	}
}

#endif /* SOLVER_HEU_DIS_H_ */