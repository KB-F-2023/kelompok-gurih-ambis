#include <iostream>
#include <queue>
#include <algorithm>

int count = 0;

using Board = std::vector<int>;
using Solution = std::vector<std::string>;

typedef struct {
    Board board;
    int moves;
    std::vector<std::string> steps;
} State;

using StatePair = std::pair<int, State>;

void printSolution(const Solution& solution) {
    for(auto step : solution) {
        std::cout << step << " ";
    }
    std::cout << std::endl;
}

void printBoard(const Board& board) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            std::cout << board[i*3 + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool checkSolved(const Board& board, const Board& target) {
    for(int i = 0; i < board.size(); i++) {
        if(board[i] != target[i]) return false;
    }
    return true;
} 

void swap(Board& board, int a, int b) {
    int temp = board[a];
    board[a] = board[b];
    board[b] = temp;
}

int outOfPlace(const Board& board, const Board& target) {
    int result = 0;
    for(int i = 0; i < 9; i++)
        if(board[i] != 0 && board[i] != target[i]) result++;
    return result;
}

int manhattanDist(const Board& board, const Board& target) {
    int result = 0;
    int boardTile[9];
    int targetTile[9];
    for(int i = 0; i < 9; i++) {
        boardTile[board[i]] = i;
        targetTile[target[i]] = i;
    }
    for(int i = 1; i < 9; i++) {
        result += abs(targetTile[i]/3 - boardTile[i]/3);
        result += abs(targetTile[i]%3 - boardTile[i]%3);
    }
    return result;
}

Board getNeighbor(const Board& board, const int& idx, const int& dir) {
    Board newBoard = board;
    switch(dir) {
    case 0:
        swap(newBoard, idx, idx-3);
        return newBoard;
    case 1:
        swap(newBoard, idx, idx+1);
        return newBoard;
    case 2:
        swap(newBoard, idx, idx+3);
        return newBoard;
    case 3:
        swap(newBoard, idx, idx-1);
        return newBoard;
    }
    return newBoard;
}

Solution astar(const Board& board, const Board& target, int (*heuristicFunc)(const Board& board, const Board& target)) {
    auto comp = [](const StatePair& a, const StatePair& b){return a.first > b.first;};
    std::priority_queue<StatePair, std::vector<StatePair>, decltype(comp)> pq(comp);

    int moves = 0, score = heuristicFunc(board, target);
    Board curBoard, newBoard;
    Solution curSteps;
    curSteps.push_back("Start ->");
    pq.push({score,{board, moves, curSteps}});

    while(!pq.empty()) {
        curBoard = pq.top().second.board;
        moves = pq.top().second.moves;
        curSteps = pq.top().second.steps;
        pq.pop();
        
        moves++;
        int idx = std::distance(curBoard.begin(), std::find(curBoard.begin(), curBoard.end(), 0));
        
        if((idx-3) >= 0 && curSteps.back() != "Down") {
            count++;
            newBoard = getNeighbor(curBoard, idx, 0);
            score = heuristicFunc(newBoard, target);
            curSteps.push_back("Up");
            if(!score) return curSteps;
            pq.push({score + moves,{newBoard, moves, curSteps}});
            curSteps.pop_back();
        }
        if((idx+1) % 3 != 0 && curSteps.back() != "Left") {
            count++;
            newBoard = getNeighbor(curBoard, idx, 1);
            score = heuristicFunc(newBoard, target);
            curSteps.push_back("Right");
            if(!score) return curSteps;
            pq.push({score + moves,{newBoard, moves, curSteps}});
            curSteps.pop_back();
        }
        if((idx+3) < 9 && curSteps.back() != "Up") {
            count++;
            newBoard = getNeighbor(curBoard, idx, 2);
            score = heuristicFunc(newBoard, target);
            curSteps.push_back("Down");
            if(!score) return curSteps;
            pq.push({score + moves,{newBoard, moves, curSteps}});
            curSteps.pop_back();
        }
        if(idx % 3 != 0 && curSteps.back() != "Right") {
            count++;
            newBoard = getNeighbor(curBoard, idx, 3);
            score = heuristicFunc(newBoard, target);
            curSteps.push_back("Left");
            if(!score) return curSteps;
            pq.push({score + moves,{newBoard, moves, curSteps}});
            curSteps.pop_back();
        }
    }
    return curSteps;
}

int main() {
    // Board board = {2, 8, 3, 1, 6, 4, 7, 0, 5};
    Board board = {2, 8, 3, 1, 6, 4, 7, 0, 5};
    Board target = {1, 2, 3, 8, 0, 4, 7, 6, 5};
    count = 0;
    printf("A* with number of not in place tiles\n");
    printSolution(astar(board, target, outOfPlace));
    printf("Nodes expanded : %d\n\n", count);
    count = 0;
    printf("A* with Manhattan Distance\n");
    printSolution(astar(board, target, manhattanDist));
    printf("Nodes expanded : %d\n", count);
    return 0;
}