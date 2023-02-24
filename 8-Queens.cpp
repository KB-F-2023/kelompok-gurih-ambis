#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using Board = std::vector<bool>;

int count1 = 0;
int count2 = 0;

void printBoard(const Board& board) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            std::cout << board[i*8 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool checkSafe(const Board& board, int idx) {
    if(idx >= 64 || idx < 0) return false;
    int row = idx/8;
    int coll = idx%8;
    for(int i = row; i >= 0; i--) 
        if(board[i*8 + coll]) 
            return false; // Up
    for(int i = row, j = coll; i >= 0 && j>= 0; i--, j--) {
        if(board[i*8 + j]) 
            return false; // Left-Up
    }
        
    for(int i = row, j = coll; i >= 0 && j < 8; i--, j++) {
        if(board[i*8 + j]) 
            return false; // Right-Up
    }
    return true;
}

bool dfs_helper(Board& board, int row) {
    if(row == 8) {
        count1 += 1;
        printBoard(board);
        return true;
    }
    for(int i = 0; i < 8; i++) {
        if(checkSafe(board, row*8 + i)) {
            board[row*8 + i] = true;
            dfs_helper(board, row + 1);
            board[row*8 + i] = false;
        }
        board[row*8 + i] = false;
    }
    return false;
}

bool dfs(const Board& board) {
    Board current = board;
    dfs_helper(current, 0);
    return false;
}

Board nextBoard(const Board& current, int idx, int coll) {
    Board next = current;
    next[(idx/8)*8 + coll] = true;
    return next;
}

bool bfs(const Board& board) {
    Board current;
    std::queue<Board> queue;
    for(int i = 0; i < 8; i++) {
        queue.push(nextBoard(board, 0, i));
    }
    int idx = 0;
    while(!queue.empty()) {
        current = queue.front();
        queue.pop();
        for(int i = 0; i < 64; i++) if(current[i]) idx = i;
        if(idx/8 == 7) {
            count2 += 1;
            printBoard(current);
        }

        for(int i = 0; i < 8; i++) {
            if(checkSafe(current, (idx/8)*8 + 8 + i)) {
                queue.push(nextBoard(current, idx + 8, i));
            }
        }
    }
    return false;

}



int main() {
    Board board(64, false);
    std::cout << "DFS Solutions : " << std::endl;
    dfs(board);
    std::cout << "BFS Solutions : " << std::endl;
    bfs(board);
    printf("%d vs %d", count1, count2);
    return 0;
}
