/**
   A* Search
   <15 Puzzle>
   
   http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_13_C
   
   Shunji Lin
   29/12/2015
**/

#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <cstdio>
#include <cstdint>
#include <exception>
#include <cstdlib>


// constants
const uint64_t end_state = 0x123456789abcdef0; // end state
const uint64_t tile_mask = 0xf; // mask to filter out lower 4 bits
const int max_moves = 100;
const uint64_t hash_entries = 1212199;

enum NSEW{ xNorth = 0, xSouth = 1, xEast = 2, xWest = 3, xNone = 4 }; // previous move from?


/**********************************************
 *  END_STATE:
 *
 *  | 1  | 2  | 3  | 4  |
 *  | 5  | 6  | 7  | 8  |
 *  | 9  | 10 | 11 | 12 |
 *  | 13 | 14 | 15 | 0  |
 *
 *  corresponding to the hexadecimal encoding:
 *  0x123456789abcdef0
 *
 *  INDEXING:
 *
 *  | 15 | 14 | 13 | 12 | 3 ^
 *  | 11 | 10 | 9  | 8  | 2 |
 *  | 7  | 6  | 5  | 4  | 1 |
 *  | 3  | 2  | 1  | 0  | 0 y
 *    3    2    1    0
 *    <--------------x
 *
 **********************************************/

/**************************************************
 *                     BOARD                      *
 **************************************************/

// Board Class
class Board {
    uint64_t state; // current state
    NSEW prev_move; // previous move, to prevent reexpansion of parent of parent
    int g_value; // number of moves taken
    int h_value; // heuristic value
    
    // pointer to children boards, indexed by moves: NSEW
    Board();
public:
    Board(const int state_array[16]); // constructor from array
    // constructor taking state and parent in uint format, also takes prev_move, g_value
    Board(uint64_t state, NSEW prev_move, int g_value);
    ~Board(void); // destructor
    
    uint64_t get_state(void) const; // return state
    int get_g_value(void) const; // return g_value
    int get_h_value(void) const; // return h_value
    int get_f_value(void) const; // return f_value
    

    int manhattan(const uint64_t state) const; // get h_value (manhattan distance) of state
    void set_h_value(void); // set h_value according to manhattan distance
    void swap_tiles(const int index1, const int index2); // swap index1 and index2 of board
    void enum_children(Board * children[4]); // enumerate children of board  
};

// constructor
// takes array representation of state and converts into hexadecimal representation of state
Board::Board(const int state_array[16]) : state(0),prev_move(xNone), g_value(0)
{
    // set state
    for (int i = 0; i < 16; ++i) {
        state += state_array[i];
        if (i < 15) state <<= 4;
    }
    h_value = manhattan(state);
}

// constructor
// takes prev_move (to block reexpansion of parent of parent), defaults to xNone
// also takes g_value of child state ( should be 1 + g_value of parent state )
Board::Board(uint64_t state, NSEW prev_move, int g_value) : state(state), prev_move(prev_move), g_value(g_value)
{
}

// destructor
Board::~Board(void)
{
}

// return state
uint64_t Board::get_state(void) const
{
    return state;
}


// return g_value (number of moves taken)
int Board::get_g_value() const
{
    return g_value;
}

// return h_value
int Board::get_h_value() const
{
    return h_value;
}

// return f_value
int Board::get_f_value() const
{
    return g_value + h_value;
}

// set h_value according to manhattan distance
void Board::set_h_value()
{
    h_value = manhattan(state);
}

// evaluates returns h_value (manhattan distance heuristic)
int Board::manhattan(const uint64_t state) const
{
    if (state == end_state) return 0;
    
    int manhattan_dist = 0;
    
    for (int i = 0; i < 16; ++i) {
        int tile = (state >> (4 * i)) & tile_mask;
        if (tile == 0) continue; // ignore blank tile for admissible heuristic
        int end_tile_index = 16 - tile; // end_state tile index
        if (i == end_tile_index) continue; // if end state index matches current index
        int x1, y1, x2, y2; // x1, y1 for tile index, x2, y2 for end_state index of tile
        
        x1 = i % 4;
        y1 = i / 4;
        x2 = end_tile_index % 4;
        y2 = end_tile_index / 4;
        
        manhattan_dist += abs(y1 - y2) + abs(x1 - x2);
        
    }
    
    return manhattan_dist;
}

// swap 2 tiles in state given indexes
void Board::swap_tiles(const int index1, const int index2)
{
    if (index1 == index2) return; // do nothing
    int high_shift, low_shift, shift_diff; // number of bits to shift
    
    high_shift = low_shift = index1 * 4;
    index1 > index2 ? low_shift = index2 * 4 : high_shift = index2 * 4;
    
    shift_diff = high_shift - low_shift; // bit distance between tiles
    
    uint64_t extract_high = state & (tile_mask << high_shift); //extract bits of tile with higher index
    uint64_t extract_low = state & (tile_mask << low_shift); // extract bits of tile with lower index
    
    state ^= (extract_high + extract_low); // replace tiles with 0000
    state ^= ((extract_high >> shift_diff) + (extract_low << shift_diff)); // fill in swapped tiles
}


// enumerate children of board and store pointers in children array
void Board::enum_children(Board * children[4])
{
    int blank_index = -1; // index of blank
    uint64_t shifted_state = state;
    for (int i = 0; i < 16; ++i)
    {
        if ((shifted_state & tile_mask) == 0) {
            blank_index = i;
            break;
        }
        shifted_state >>= 4;
        
    }
    int child_g_value = g_value + 1; // g_value after 1 move
    
    // for north move
    if (prev_move != xNorth && blank_index < 12) {
        Board * child = new Board(state, xSouth, child_g_value);
        child->swap_tiles(blank_index + 4, blank_index);
        child->set_h_value();
        children[xNorth] = child;
    }
    // for south move
    if (prev_move != xSouth && blank_index > 3) {
        Board * child = new Board(state, xNorth, child_g_value);
        child->swap_tiles(blank_index - 4, blank_index);
        child->set_h_value();
        children[xSouth] = child;
    }
    // for east move
    if (prev_move != xEast && (blank_index % 4) != 0) {
        Board * child = new Board(state, xWest, child_g_value);
        child->swap_tiles(blank_index, blank_index - 1);
        child->set_h_value();
        children[xEast] = child;
    }
    // for west move
    if (prev_move != xWest && (blank_index % 4) != 3) {
        Board * child = new Board(state, xEast, child_g_value);
        child->swap_tiles(blank_index + 1, blank_index);
        child->set_h_value();
        children[xWest] = child;
    }
    
    return;
}

/**************************************************
 *              PRIORITY QUEUE                    *
 **************************************************/

// Priority Queue Class
class PriorityQ {
    // linked-list node
    struct node {
        Board * board;
        node * next;
    };
    
    int n_elements; // number of queue elements
    int min_index; // index of current minimum f-score, max_moves for empty queue
    node * queue[max_moves];
public:
    PriorityQ(); // constructor
    ~PriorityQ(); // destructor
    
    int get_n_elements(void) const;
    void insert(Board * boardptr); // insert board into priority queue
    void remove(const uint64_t state, const int old_f); // remove board with state from f_value index, assume no change to min_index
    Board * extract_min(void); // extract board with min f_value
};

// constructor, empty priority queue
PriorityQ::PriorityQ(): n_elements(0), min_index(max_moves)
{
    for (int i = 0; i < max_moves; ++i ) queue[i] = nullptr;
}

// destructor
PriorityQ::~PriorityQ()
{
    for (int i = 0; i < max_moves; ++i ) {
        node * current = queue[i];
        while (current != nullptr) {
            node * temp = current;
            current = current->next;
            delete temp->board;
            delete temp;
        }
    }
}

// return number of elements
int PriorityQ::get_n_elements() const
{
    return n_elements;
}

// insert given board into priority queue
void PriorityQ::insert(Board * boardptr)
{
    node * board_node = new node;
    board_node->board = boardptr; // store board in node
    
    int f_value = boardptr->get_f_value(); // queue is indexed by f_value
    
    min_index = f_value < min_index ? f_value : min_index ; // update min_index if needed
    
    // insert into head of linked list
    node * temp = queue[f_value];
    queue[f_value] = board_node;
    board_node->next = temp;
    
    n_elements++; // increment number of elements in priority queue
}


// remove given state from priority queue given f_value.
// assume that min_index need not be updated.
void PriorityQ::remove(const uint64_t state, const int old_f)
{
    node * temp = queue[old_f];
    
    if (temp == nullptr) return; // state not in given index
    
    node * temp_next = temp->next;
    
    if (temp->board->get_state() == state) { // if state is first item in linked list
        queue[old_f] = temp_next;
        delete temp;
        --n_elements;
        return;
    }
    
    while (temp_next != nullptr) {
        if (temp_next->board->get_state() == state) {
            temp->next = temp_next->next; // skip over node to be removed
            delete temp_next;
            --n_elements;
            return;
        }
        // if not found, move forward temp, temp_next
        temp = temp_next;
        temp_next = temp_next->next;
    }
}

// extract board with min f_value, from head of bucket
Board * PriorityQ::extract_min(void)
{
    if (!n_elements) {
        printf("no elements in priority queue\n");
        return nullptr;
    }

    while ( min_index != max_moves && !queue[min_index])
    { // while min_index is pointing at empty bucket
        ++min_index; // increment min_index
    }
    // extract node
    node * temp = queue[min_index];
    queue[min_index] = temp->next;
    
    // extract board
    Board * boardptr = temp->board;
    
    // delete node
    delete temp;
    
    n_elements--; // decrement number of elements
    min_index = n_elements == 0 ? max_moves : min_index ; // update min_index if needed
    
    return boardptr;
}

/**************************************************
 *                Closed List                     *
 **************************************************/
enum Response{ xInsert = 0, xUpdate = 1, xReject = 2 };
// Closed List Class
class Closed {
    struct hash_node {
        uint64_t state = 0;
        bool processed = false; // processed upon extracting from priority queue (children fully expanded)
        int f_value = max_moves; // if f_score set, state is discovered, else max_moves
    };
    hash_node closed[hash_entries]; // hash table, size is hash_entries
public:
    Closed(); // constructor
    ~Closed(); // destructor
    uint64_t hash_value(const uint64_t state); // returns hash_value given state
    Response expand_check(const Board * boardptr, int & old_f); // check closed list before adding into priority queue
    void process(const Board * boardptr);
    
};

// constructor: set close to hold hash_entries * hash_node
Closed::Closed()
{
}

// cleanup memory
Closed::~Closed()
{
}

// returns hash_value given state
uint64_t Closed::hash_value(uint64_t state)
{
    return state % hash_entries;
}

/* search closed set for state:
 * if not found, set to discovered, update state, and update f_score. return xInsert.
 * if found and processed, do nothing. return xReject.
 * if found and not processed, compare f_scores:
 *  - if f_score lower than existing, update f_score, and return old f_score
 *  - if f_score higher or equivalent to existing, do nothing. return INT_MAX.
 */
Response Closed::expand_check(const Board * boardptr, int & old_f)
{
    uint64_t state = boardptr->get_state();
    uint64_t hash_index = hash_value(state);
    
    
    while (closed[hash_index].f_value != max_moves && closed[hash_index].state != state) {
        // while hash collision
        // increment index, or wrap around if end of hash table
        hash_index = hash_index == hash_entries - 1 ? 0 : hash_index + 1;
    }
    
    if (closed[hash_index].f_value == max_moves) { // if not discovered
        //insert
        closed[hash_index].state = state; // update state
        closed[hash_index].f_value = boardptr->get_g_value() + boardptr->get_f_value(); // update f_value
        return xInsert;
    } else if (closed[hash_index].state == state) { // if discovered
        if (closed[hash_index].processed) return xReject; // reject if already processed
        
        int board_f_value = boardptr->get_f_value();
        if (closed[hash_index].f_value > board_f_value) { // if lower f_value
            closed[hash_index].f_value = board_f_value; // update f_value
            old_f = closed[hash_index].f_value; // store old f_value (for deletion in priority Q)
            return xUpdate;
        } else { // f_value higher or equivalent
            return xReject;
        }
    }
    return xReject;
}

// mark board as processed
void Closed::process(const Board * boardptr)
{
    uint64_t state = boardptr->get_state();
    uint64_t hash_index = hash_value(state);
    
    while (closed[hash_index].state != state && closed[hash_index].f_value != max_moves) {
        // while hash collision
        // increment index, or wrap around if end of hash table
        hash_index = hash_index == hash_entries - 1 ? 0 : hash_index + 1;
    }
    // if undiscovered, update all values
    if (closed[hash_index].f_value == max_moves) {
        closed[hash_index].state = state;
        closed[hash_index].f_value = boardptr->get_f_value();
    }
    closed[hash_index].processed = true;
    
}

/**************************************************
 *                A* Functions                    *
 **************************************************/

//perform one step of A*, return board if goal state found
Board * a_star_step(PriorityQ & priorityQ, Closed & closed)
{
    Board * boardptr = priorityQ.extract_min(); // extract lowest f_value from priority queue
    
    if (boardptr->get_state() == end_state) return boardptr;
    
    Board * children[4] = { nullptr, nullptr, nullptr, nullptr }; // initialize children array
    
    boardptr->enum_children(children); // enumerate children
    
    closed.process(boardptr);
    
    delete boardptr;
    
    for (int i = 0; i < 4; ++i) {
        int old_f;
        Board * child = children[i];
        if (child == nullptr) continue; // if not valid child pointer
        
        Response check = closed.expand_check(child, old_f);

        if (check == xInsert) {
            priorityQ.insert(child); // insert child into priorityQ
        } else if (check == xUpdate) {
            priorityQ.insert(child); // insert child into priorityQ
            priorityQ.remove(child->get_state(), old_f); // remove state of higher f_value
        } else {
            delete child; // cleanup memory
        }
    }
    return nullptr; // return nullptr if not goal state
}

// Perform A* until goal state is reached
void a_star(PriorityQ & priorityQ, Closed & closed) {
    if (!priorityQ.get_n_elements()) printf("failed, priority queue empty\n");
    Board * goal_board = nullptr;
    while (!goal_board) {
        // continue to do a_star_step if goal state not found
        goal_board = a_star_step(priorityQ, closed);
    }
    printf("%d\n", goal_board->get_g_value()); // number of steps to reach goal
    delete goal_board;
    
}



int main(int argc,char ** argv)
{
  //read input
    int state_array[16];
    for (int i = 0; i < 16; ++i) {
        scanf("%d", &state_array[i]);
    }

    //initialize board, priority queue and closed list
    Board * boardptr = new Board(state_array);
    PriorityQ * priorityQ = new PriorityQ();
    Closed * closed = new Closed();
    closed->process(boardptr); 
    priorityQ->insert(boardptr);
    
    // search
    a_star(*priorityQ, *closed);

    // clean up
    delete priorityQ;
    delete closed; 
}
