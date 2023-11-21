/*
I hereby certify that no other part of this submission has been copied from any
other sources, including the Internet, books or other student’s work, or
generated from generative AI tools, such as ChatGPT except the ones I have
listed below:

I hold a copy of this assignment that I can produce if the original is lost or
damaged.
*/

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

using namespace std;

#include "Solver.h"
#include "Solver_Heu.h"
#include "Solver_Heu_Dis.h"
#include "Solver_Heu_Con.h"
#include "Solver_Heu_A_Star.h"

int main() {
	srand((unsigned)time(0));

	cout << "1) Singular Goal" << endl;
	cout << "2) Singular Goal w/ Heuristics" << endl;
	cout << "3) Disjunctive Goals" << endl;
	cout << "4) Conjunctive Goals" << endl;
	cout << "5) Conjunctive Goals on an n*n Board" << endl;
	cout << "6) Singular Goal w/ A* Search" << endl;
	cout << "7) Quit" << endl;
	cout << "====================================" << endl;

	cout << "Choose a level from above to run: ";

	int ans;

	cin >> ans;

	switch (ans) {
		case 1:
		{
			State* state = new State;
			Solver solver(state);
			solver.solve();
			break;
		}
		case 2:
		{
			State* state = new State;
			Solver_Heu solver_heu(state);
			solver_heu.solve();
			break;
		}
		case 3:
		{
			State* state = new State;
			Solver_Heu_Dis solver_heu_dis(state);
			solver_heu_dis.solve();
			break;
		}
		case 4:
		{
			State* state = new State;
			Solver_Heu_Con solver_heu_con(state);
			solver_heu_con.solve();
			break;
		}
		case 5:
		{
			cout << "Quitting..." << endl;
			break;
		}
		case 6:
		{
			State* state = new State;
			Solver_Heu_A_Star solver_heu_a_star(state);
			solver_heu_a_star.solve();
			break;
		}
		case 7:
		{
			cout << "Quitting..." << endl;
			break;
		}
	}
	
	return 0;
}