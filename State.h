#ifndef STATE_H_
#define STATE_H_

#include "Constants.h"

class State {
private:
	vector<int> blocks;
	int grid[BOARDSIZE][BOARDSIZE];
	Block block;
	priority_queue<Action> actions;

public:
	State() {
		for (int i = 0; i < NUMBER_OF_BLOCKS; i++) {
			blocks.push_back(i + 1);
		}
		for (int i = 0; i < BOARDSIZE * BOARDSIZE - NUMBER_OF_BLOCKS; i++) {
			blocks.push_back(0);
		}
		randomise();
		push_down();
	}

	inline virtual bool operator==(State s) {
		for (int i = 0; i < BOARDSIZE; i++) {
			for (int j = 0; j < BOARDSIZE; j++) {
				if (s.grid[i][j] != grid[i][j]) {
					return false;
				}
			}
		}
		return true;
	}

	inline virtual bool operator!=(State s) {
		for (int i = 0; i < BOARDSIZE; i++) {
			for (int j = 0; j < BOARDSIZE; j++) {
				if (s.grid[i][j] != grid[i][j]) {
					return true;
				}
			}
		}
		return false;
	}

	// Prints the board.
	inline virtual void print();
	
	// Randomises the board.
	inline virtual void randomise();
	
	// Pushes all the numbers down.
	inline virtual void push_down();
	
	// Removes top block from the source column.
	inline virtual int remove_block(int col);
	
	// Inserts the value onto the top of the destination column.
	inline virtual bool insert_block(int col, int value);
	
	// Combines remove_block and insert_block.
	inline virtual bool move_block(int source, int destination);
	
	// Function to generate all legal actions and put them in a container.
	inline virtual void legal_actions_generate();
	

	inline virtual priority_queue<Action> get_legal_actions();

	inline virtual Block find_location(int i);
	inline virtual int find_top_block(int col);
	inline virtual int find_number_of_blocks_above(int i);
	inline virtual int get_block(int row, int col);

	inline virtual State* next_state(Action a);

	inline virtual int find_number_of_blocks_in_col(int i);

	inline virtual void generate_state_blank();
	inline virtual void place_goal(Goal g);
	inline virtual queue<int> find_rows_of_free_spots_below_highest_block(int col);
};

inline void State::print() {
	for (int i = 0; i < BOARDSIZE; i++) {
		for (int j = 0; j < BOARDSIZE; j++) {
			cout << " " << grid[BOARDSIZE - i - 1][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

inline void State::randomise() {
	for (int row = 0; row < BOARDSIZE; row++) {
		for (int col = 0; col < BOARDSIZE; col++) {
			int temp_index = rand() % blocks.size();

			grid[row][col] = blocks.at(temp_index);

			blocks.erase(blocks.begin() + temp_index);
		}
	}
}

inline void State::push_down() {
	for (int col = 0; col < BOARDSIZE; col++) {
		queue<int> temp_queue;

		for (int row = 0; row < BOARDSIZE; row++) {
			if (grid[row][col] != 0) {
				temp_queue.push(grid[row][col]);
			}
		}

		while (temp_queue.size() < BOARDSIZE) {
			temp_queue.push(0);
		}

		for (int row = 0; row < BOARDSIZE; row++) {
			grid[row][col] = temp_queue.front();
			temp_queue.pop();
		}
	}
}

inline int State::remove_block(int col) {
	for (int row = BOARDSIZE - 1; row > -1; row--) {
		if (grid[row][col] != 0) {
			int temp = grid[row][col];
			grid[row][col] = 0;
			return temp;
		}
	}

	return -1;
}

inline bool State::insert_block(int col, int value) {
	// Check if the value is -1 as this means the source col was empty.
	if (value == -1) {
		return false;
	}
	else {
		grid[BOARDSIZE - 1][col] = value;
		push_down();
		return true;
	}
}

inline bool State::move_block(int source, int destination) {
	// First check if the destination col is full.
	if (grid[BOARDSIZE - 1][destination] != 0) {
		return false;
	}
	else {
		return insert_block(destination, remove_block(source));
	}

}

inline void State::legal_actions_generate() {
	vector<int> col_src;
	vector<int> col_top_block;
	vector<int> col_dst;

	priority_queue<Action> temp;

	swap(actions, temp);

	// Finding legal source columns. Just has to be a non-empty column.
	for (int col = 0; col < BOARDSIZE; col++) {
		// Scanning bottom rows for non-zeroes.
		if (grid[0][col] != 0) {
			col_src.push_back(col);
		}
	}

	// Finding legal destination columns. Just has to be a non-full column.
	for (int col = 0; col < BOARDSIZE; col++) {
		// Scanning top rows for zeroes.
		if (grid[BOARDSIZE - 1][col] == 0) {
			col_dst.push_back(col);
		}
	}

	// Storing all legal actions inside a priority queue.
	for (int i = 0; i < col_src.size(); i++) {
		for (int j = 0; j < col_dst.size(); j++) {
			// Check to make sure that col_src and col_dst are different.
			if (col_src.at(i) != col_dst.at(j)) {
				Action action {
					action.src = col_src.at(i),
					action.dst = col_dst.at(j),
					action.heuristic = 0,
					action.steps_cost = 1
				};

				actions.push(action);
			}
		}
	}
}

inline priority_queue<Action> State::get_legal_actions() {
	return actions;
}

inline Block State::find_location(int i) {
	for (int row = 0; row < BOARDSIZE; row++) {
		for (int col = 0; col < BOARDSIZE; col++) {
			if (grid[row][col] == i) {
				block.block = i;
				block.row = row;
				block.col = col;
			}
		}
	}

	return block;
}

inline int State::find_top_block(int col) {
	for (int row = BOARDSIZE - 1; row > -1; row--) {
		if (grid[row][col] != 0) {
			return grid[row][col];
		}
	}
	return -1;
}

inline int State::find_number_of_blocks_above(int i) {
	int number = 0;

	for (int row = BOARDSIZE - 1; row > find_location(i).row; row--) {
		if (grid[row][find_location(i).col] != 0) {
			number++;
		}
	}
	return number;
}

inline int State::get_block(int row, int col) {
	return grid[row][col];
}

inline State* State::next_state(Action a) {
	State* new_state = new State(*this);

	if (new_state->move_block(a.src, a.dst)) {
		return new_state;
	}
	return this;
}

inline int State::find_number_of_blocks_in_col(int col) {
	int number = 0;

	for (int row = 0; row < BOARDSIZE; row++) {
		if (grid[row][col] != 0) {
			number++;
		}
	}
	return number;
}

inline void State::generate_state_blank() {
	for (int row = 0; row < BOARDSIZE; row++) {
		for (int col = 0; col < BOARDSIZE; col++) {
			grid[row][col] = 0;
		}
	}
}

inline void State::place_goal(Goal g) {
	grid[g.row][g.col] = g.block;
}

inline queue<int> State::find_rows_of_free_spots_below_highest_block(int col) {
	queue<int> queue_temp;
	int number = 0;
	int highest_row = 0;

	// First, find the highest block's row.
	for (int row = BOARDSIZE - 1; row > -1; row--) {
		if (grid[row][col] != 0) {
			highest_row = row;
			break;
		}
	}

	// Then start counting the zeroes from there to the bottom.
	if (highest_row != 0) {
		for (int row = highest_row; row > -1; row--) {
			if (grid[row][col] == 0) {
				queue_temp.push(row);
			}
		}
	}

	return queue_temp;
}

#endif /* STATE_H_ */