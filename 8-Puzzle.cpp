#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using Board = std::vector<int>;
using Result = std::vector<std::vector<std::string>>;

void printResult(const Result& result) {
    for(auto steps : result) {
        std::cout << "Solution : "<< steps.size() << " Steps" << std::endl;
        for(auto step : steps) {
            std::cout << step << " ";
        }
        std::cout << std::endl;
    }
}

bool checkSolved(const Board& board, const Board& target) {
    for(int i = 0; i < board.size(); i++) {
        if(board[i] != target[i]) return false;
    }
    return true;
} 

void dfs_helper(Board& board, const Board& target, int idx, int pIdx, Board& visited, std::vector<std::string> steps, Result& result) {
    if(idx < 0 || idx >= 9) {
        // steps.pop_back();
        return;
    }
    if(visited[idx] >= 3) {
        // steps.pop_back();
        return;
    }

    visited[idx] += 1;
    int temp = board[idx];
    board[idx] = board[pIdx];
    board[pIdx] = temp;
    
    if(checkSolved(board, target)) {
        result.push_back(steps); 

        visited[idx] -= 1;
        temp = board[idx];
        board[idx] = board[pIdx];
        board[pIdx] = temp;       
        return;
    } 
    
    if(idx - 3 != pIdx) {
        steps.push_back("Up");
        dfs_helper(board, target, idx - 3, idx, visited, steps, result);
        steps.pop_back();
    } // Up
    if((idx + 1 != pIdx) && (idx % 3 < 2)) {
        steps.push_back("Right");
        dfs_helper(board, target, idx + 1, idx, visited, steps, result);
        steps.pop_back();
    } // Right
    if(idx + 3 != pIdx) {
        steps.push_back("Down");
        dfs_helper(board, target, idx + 3, idx, visited, steps, result);
        steps.pop_back();
    } // Down
    if((idx - 1 != pIdx) && (idx % 3 > 0)) {
        steps.push_back("Left");
        dfs_helper(board, target, idx - 1, idx, visited, steps, result);
        steps.pop_back();
    } // Left

    visited[idx] -= 1;
    temp = board[idx];
    board[idx] = board[pIdx];
    board[pIdx] = temp;
    // steps.pop_back();
    return;
}

Result dfs(const Board& board, const Board& target) {
    Board visited = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    Board current = board;
    std::vector<std::string>steps;
    steps.push_back("Start -> ");
    Result result;
    int idx = std::distance(current.begin(), std::find(current.begin(), current.end(), 0));
    dfs_helper(current, target, idx, idx, visited, steps, result);
    return result;
}

Board getNeighbor(const Board& node, int dir) {
    int idx;
    int temp;
    Board neighbor = node;
    switch(dir) {
    case 0:
        idx = std::find(node.begin(), node.end(), 0) - node.begin();
        temp = neighbor[idx];
        neighbor[idx] = neighbor[idx-3];
        neighbor[idx-3] = temp;
        return neighbor;
    case 1:
        idx = std::find(node.begin(), node.end(), 0) - node.begin();
        temp = neighbor[idx];
        neighbor[idx] = neighbor[idx+1];
        neighbor[idx+1] = temp;
        return neighbor;
    case 2:
        idx = std::find(node.begin(), node.end(), 0) - node.begin();
        temp = neighbor[idx];
        neighbor[idx] = neighbor[idx+3];
        neighbor[idx+3] = temp;
        return neighbor;
    case 3:
        idx = std::find(node.begin(), node.end(), 0) - node.begin();
        temp = neighbor[idx];
        neighbor[idx] = neighbor[idx-1];
        neighbor[idx-1] = temp;
        return neighbor;
    }
    return neighbor;
}

Result bfs(Board& board, Board& target) {
    int idx;
    std::queue<Board>queue;
    std::queue<std::vector<std::string>>steps;
    queue.push(board);

    Board current;
    Result result;
    std::vector<std::string>cur_steps;
    cur_steps.push_back("Start -> ");
    steps.push(cur_steps);
    while(!queue.empty() && result.size() <= 10) {
        current = queue.front();
        queue.pop();
        cur_steps = steps.front();
        steps.pop();
        
        if(checkSolved(current, target)) {
            result.push_back(cur_steps);
        }
        idx = std::find(current.begin(), current.end(), 0) - current.begin();
        
        if((idx - 3) > 0  && cur_steps.back() != "Down") {
            cur_steps.push_back("Up");
            steps.push(cur_steps);
            cur_steps.pop_back();
            queue.push(getNeighbor(current, 0));
        } // Up
        if(idx % 3 < 2 && cur_steps.back() != "Left") {
            cur_steps.push_back("Right");
            steps.push(cur_steps);
            cur_steps.pop_back();
            queue.push(getNeighbor(current, 1)); 
        } // Right
        if((idx + 3) < 8 && cur_steps.back() != "Up") {
            cur_steps.push_back("Down");
            steps.push(cur_steps);
            cur_steps.pop_back();
            queue.push(getNeighbor(current, 2));
        } // Down
        if(idx % 3 > 0 && cur_steps.back() != "Right") {
            cur_steps.push_back("Left");
            steps.push(cur_steps);
            cur_steps.pop_back();
            queue.push(getNeighbor(current, 3));
        } // Left
    }
    return result;
}

int main() {
    Board board = {2, 8, 3, 1, 6, 4, 7, 0, 5};
    Board target = {1, 2, 3, 8, 0, 4, 7, 6, 5};

    auto result = dfs(board, target);
    std::cout << "DFS Results : " << std::endl;
    printResult(result);

    result = bfs(board, target);
    std::cout << "BFS Results : " << std::endl;
    printResult(result);

    return 0;
}
