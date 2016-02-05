/**
   8 Queens Problem
   
   http://judge.u-aizu.ac.jp/onlinejudge/description.jsp?id=ALDS1_13_A
   
   8 queens puzzle using Minimum Remaining Value(MRV) for row selection
   and min-conflicts heuristic for col selection
   
   Shunji Lin
   05/02/2016
**/

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>

static const int N = 8;

// class prototype
class EightQ;

// for ordering row according to MRV
struct mrv_node {
    int row; // row index
    int n_safe; // number of safe spots in row
    bool operator<(const mrv_node & rhs) const { return n_safe < rhs.n_safe; }
};

// for ordering col according to min_conflict heuristic
struct min_con_node {
    int col; // col index
    EightQ * child;
    int n_safe; // number of safe spots
    bool operator<(const min_con_node & rhs) const { return n_safe < rhs.n_safe; }
};

class EightQ {
    int assigned = 0;
    int row_piece[N]; // positions of pieces in each row
    int safe_row[N]; // number of safe spots on each row
    bool board[N][N] = {{false}}; // positions unsafe
    EightQ * children[N] = {nullptr} ; // children states
public:
    EightQ();
    ~EightQ();
    EightQ(const EightQ &obj); // copy constructor
    void insert_piece(int row, int col);
    int safe_pos(); // return number of safe positions
    void populate_children(int row); // given row, populate with children
    bool solvable(); // return true if solvable
    int min_conflict_index(); // return solvable, min_conflict child index
    void solve();
    void print_pieces();
    void print_safe_pos();
    
};


// constructor
EightQ::EightQ()
{
    for (int i = 0; i < N; ++i) {
        row_piece[i] = -1; // -1 for unassigned
    }
    for (int i = 0; i < N; ++i) {
        safe_row[i]= N; // 8 safe positions per row initially
    }
    
}

EightQ::~EightQ()
{
}

// copy constructor
EightQ::EightQ(const EightQ &obj) {
    assigned = obj.assigned;
    std::copy(&(obj.row_piece[0]), &(obj.row_piece[N]), &(row_piece[0]));
    std::copy(&(obj.safe_row[0]), &(obj.safe_row[N]), &(safe_row[0]));
    std::copy(&(obj.board[0][0]), &(obj.board[N][N]), &(board[0][0]));
    for (int i = 0; i < N; ++i) {
        children[i] = nullptr;
    }
    
}

// insert piece and update board details
void EightQ::insert_piece(int row, int col)
{
    if (board[row][col]) {
        printf("Cannot assign piece to unsafe position.\n");
        return;
    }
    row_piece[row] = col;
    ++assigned;
    
    // populate row
    for (int i = 0; i < N; ++i) {
        if (!board[row][i]) {
            board[row][i] = true; // row is unsafe
            --safe_row[row];
        }
    }
    
    // populate col
    for (int i =0; i < N; ++i) {
        if (!board[i][col]) {
            board[i][col] = true; // col is unsafe
            --safe_row[row];
        }
    }
    
    // populate diagonals
    for (int i = 0; i < N; ++i) {
        int row_diff, l_diag, r_diag;
        row_diff = std::abs(row - i);
        l_diag = col - row_diff;
        r_diag = col + row_diff;
        
        // left diagonal
        if (l_diag >= 0 && !board[i][l_diag]) {
            board[i][l_diag] = true;
            --safe_row[row];
        }
        // right diagonal
        if (r_diag < N && !board[i][r_diag]) {
            board[i][r_diag] = true;
            --safe_row[row];
        }
    }
}

// return number of safe positions on board
int EightQ::safe_pos() {
    int sum = 0;
    for (int i = 0; i < N; ++i) {
        sum += safe_row[i];
    }
    return sum;
}


// populate the children array with child board configurations
void EightQ::populate_children(int row)
{
    for (int i = 0; i < N; ++i) {
        if (!board[row][i]) { // if safe
            EightQ * child = new EightQ(*this);
            child->insert_piece(row, i);
            this->children[i] = child;
        }
    }
}

// return true if solvable
bool EightQ::solvable()
{
    for (int i = 0; i < N; ++i) {
        // if unassigned and no safe spots (unless all assigned), not solvable
        if (row_piece[i] == -1 && !safe_row[i]) {
            //if (assigned != 8)
            return false;
        }
    }
    return true;
}

// solve for board
void EightQ::solve()
{
    static bool solved = false;
    if (solved) return; // if solved, backtrack
    
    if (assigned == 8) { // solved
        solved = true;
        this->print_pieces();
    }
    
    // array to order row in MRV order
    std::vector<mrv_node> mrv_vector;
    
    for (int i = 0; i < N; ++i) {
        if (safe_row[i]) {
            mrv_node temp;
            temp.row = i;
            temp.n_safe = safe_row[i];
            mrv_vector.push_back(temp);
        }
    }
    
    std::sort(mrv_vector.begin(), mrv_vector.end()); // increasing order of assignable positions
    
    for (std::vector<mrv_node>::iterator it=mrv_vector.begin(); it!=mrv_vector.end(); ++it) {
        populate_children(it->row); // populate given row's children
        // array to order assignable cols in min_conflict order
        std::vector<min_con_node> min_con_vector;
        for (int i = 0; i < N; ++i) {
            if (children[i]) { // if not nullptr
                min_con_node temp;
                temp.col = i;
                temp.child = children[i];
                temp.n_safe = temp.child->safe_pos();
                min_con_vector.push_back(temp);
            }
        }
        std::sort(min_con_vector.begin(), min_con_vector.end()); // decreasing order of safe positions
        for (std::vector<min_con_node>::reverse_iterator it2=min_con_vector.rbegin(); it2!=min_con_vector.rend(); ++it2) {
            it2->child->solve();
        }
        
        // free memory
        for (int i = 0; i < N; ++i) {
            if (children[i]) {
                delete(children[i]);
                children[i] = nullptr;
            }
        }
        
    }
 
}


// print pieces assigned
void EightQ::print_pieces()
{
    for (int i = 0; i < N; ++i) {
        
        // if unassigned, print "."s
        if (row_piece[i] == -1) {
            for (int j = 0; j < N; ++j) {
                printf(".");
            }
            printf("\n");
        } else {
            // if assigned, print "."s with Q
            for (int j = 0; j < row_piece[i]; ++j) {
                printf(".");
            }
            printf("Q");
            for (int j = row_piece[i] + 1; j < N; ++j) {
                printf(".");
            }
            printf("\n");
        }
    }
}

// print safe positions
void EightQ::print_safe_pos() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j]) {
                printf("x");
            } else {
                printf("o");
            }
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    
    EightQ * board = new EightQ();
    
    int initial_n; // initial number of pieces
    scanf("%d", &initial_n);
    for (int i = 0; i < initial_n; ++i) {
        int row, col;
        scanf("%d %d", &row, &col);
        board->insert_piece(row, col);
    }
    
    board->solve();

    delete(board);
}

