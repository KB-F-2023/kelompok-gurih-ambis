type Board = number[]
type Population = BoardEntity[]

interface BoardEntity {
    board: Board,
    fit: number
}

const randomInt = (start: number, end: number): number => {
    return Math.floor(Math.random() * (end-start+1)) + start
}

function createBoard(b: Board = []): BoardEntity {
    if(b.length == 0) b = randomState()
    let newFit = getConflict(b)
    return {
        board: b,
        fit: newFit
    }
}

function randomState(): Board {
    let board = Array(8).fill(0);
    for(let i = 0; i < 8; i++) {
        board[i] = Math.floor(Math.random() * 8)
    } 
    return board;
}

function printBoard(board: Board) {
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

function getConflict(board: Board): number {
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

function crossOver(a: Board, b: Board, mutation: number = 50): BoardEntity[] {
    let split = randomInt(0, 7)
    let newA = [...a]
    let newB = [...b]
    for(let i = 0; i < split; i++) {
        newA[i] = b[i]
        newB[i] = a[i]
    }

    // Mutation (50% Chance)
    if(randomInt(0, 100) > mutation) newA[randomInt(0,7)] = randomInt(0,7)
    if(randomInt(0, 100) > mutation) newB[randomInt(0,7)] = randomInt(0,7)
    return [createBoard(newA), createBoard(newB)]
}

function geneticAlgorithm(pop: number = 10, maxGen: number = 1000000): [Board, number] {
    let startPop: Population = []
    let newPop: Population = []
    let generations = 0
    // Get initial population
    for(let i = 0; i < pop; i++) {
        startPop.push(createBoard())
    }
    newPop = startPop.slice()
    for(let i = 0; i < maxGen; i++) {
        generations++
        // Sort by fitness
        startPop.sort((a, b) => (a.fit - b.fit))
        if(startPop[0].fit == 0) return [startPop[0].board, i]
        // Emptying new population
        newPop = []
        // Crossover
        for(let j = 0; j < startPop.length-1 && newPop.length + 2 <= pop; j++) {
            newPop.push(...crossOver(startPop[j].board, startPop[j+1].board))
        }
        // Replacing old population
        startPop = newPop.slice()
    }
    return [startPop[0].board, generations]
}

let [board, generations] = geneticAlgorithm()
printBoard(board)
console.log('Fitness : ' + getConflict(board))
console.log('Number of Generations : ' + generations)
