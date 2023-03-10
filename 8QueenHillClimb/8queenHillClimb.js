function randomState() {
    let board = Array(8).fill(0);
    for(let i = 0; i < 8; i++) {
        board[i] = Math.floor(Math.random() * 8)
    } 
    return board;
}

function printBoard(board) {
    let buffer = '';
    for(let i = 0; i < 8; i++) {
        for(let j = 0; j < 8; j++) {
            if(board[j] == i) buffer += '1 '
            else buffer += '0 '
        }
        console.log(buffer);
        buffer = '';
    }
}

function getConflict(board) {
    let leftDiag = Array(15).fill(0)
    let rightDiag = Array(15).fill(0)
    let row = Array(8).fill(0)
    let result = 0;
    for(let j = 0; j < 8; j++) {
        row[board[j]]++
        leftDiag[board[j] + j]++
        rightDiag[7-board[j] + j]++
    }

    for(let i = 0; i < 15; i++) {
        if(i < 8) result += (row[i] * (row[i] - 1)) / 2
        result += (leftDiag[i] * (leftDiag[i] - 1)) / 2
        result += (rightDiag[i] * (rightDiag[i] - 1)) / 2
    }
    return result
}

function swap(board , a, b) {
    let temp = board[a[0]][a[1]];
    board[a[0]][a[1]] = board[b[0]][b[1]];
    board[b[0]][b[1]] = temp
}

function hillClimb(board) {
    console.log("Start State :")
    printBoard(board)
    console.log('')
    let low = getConflict(board)
    let nextStates = []
    let flag = false
    while(true) {
        flag = false
        if(low == 0) {
            console.log("Found solution")
            return board
        }
        for(let j = 0; j < 8; j++) {
            var copy = board.slice()
            for(let i = 0; i < 8; i++) {
                copy[j] = i
                var conflicts = getConflict(copy)
                if(conflicts <= low && copy[j] != board[j]) {
                    flag = true
                    if(conflicts != low) nextStates = []
                    low = conflicts
                    nextStates.push([j, i])
                }
            }
        }
        if(!flag) {
            console.log("No better states")
            return board
        }
        var i = Math.floor(Math.random() * nextStates.length)
        board[nextStates[i][0]] = nextStates[i][1]
    }
}

board = randomState()
board = hillClimb(board)
printBoard(board)
console.log(getConflict(board))