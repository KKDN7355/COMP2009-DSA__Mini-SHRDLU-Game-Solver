#ifndef ACTION_H_
#define ACTION_H_

struct Action {
	int src;
	int dst;
	int heuristic;
	int steps_cost;
	
	bool operator<(const Action a) const {
		if (heuristic < a.heuristic) {
			return true;
		}
		else {
			return false;
		}
	}
};

#endif /* ACTION_H_ */