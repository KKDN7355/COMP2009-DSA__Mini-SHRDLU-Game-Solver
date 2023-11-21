#ifndef SEARCH_NODE_H_
#define SEARCH_NODE_H_

#include "State.h"

struct Search_Node {
	State* state;
	Search_Node* parent;
	int steps_total;
	int heuristic;

	inline bool operator<(const Search_Node node) const {
		if (heuristic < node.heuristic) {
			return true;
		}
		else {
			return false;
		}
	}
};

#endif /* SEARCH_NODE_H_ */