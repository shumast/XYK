#include "xyk.h"

void XYK::makeRandomCloseMove() {
    std::vector<std::pair<int, int>> untriedMoves = allowedMoves(board_);
    auto p = untriedMoves[gen() % untriedMoves.size()];
    makeMove(p.first, p.second);
}

int XYK::makeMinimax(int x, int y, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = 10;
        return 10;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -10;
        return -10;
    }
    int ans = -INF;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        board_[i][j] = sophisticated_queue[turn + 1];
        int result = makeMinimax(i, j, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
        if (ans == -INF) {
            ans = result;
        }
        else if (sophisticated_queue[turn] == currentPlayer_) {
            ans = std::min(ans, result);
        }
        else {
            ans = std::max(ans, result);
        }
        board_[i][j] = -1;
        winner_ = -1;
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxMove() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    for (auto [i, j] : allmoves) {
        board_[i][j] = currentPlayer_;
        int tmp = makeMinimax(i, j, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
        if (val < tmp) {
            val = tmp;
            ans = { i, j };
        }
        else if (tmp == val && gen() % R_VARIOTY == 0) {
            val = tmp;
            ans = { i, j };
        }
        board_[i][j] = -1;
        winner_ = -1;
    }
    makeMove(ans.first, ans.second);
}

int XYK::makeMinimaxWithAlphaBeta(int x, int y, int alpha, int beta, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = 10;
        return 10;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -10;
        return -10;
    }
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    int ans = -INF;
    for (auto [i, j] : allmoves) {
        board_[i][j] = sophisticated_queue[turn + 1];
        int result = makeMinimaxWithAlphaBeta(i, j, alpha, beta, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
        if (sophisticated_queue[turn] == currentPlayer_) {
            beta = std::min(result, beta);
        }
        else {
            alpha = std::max(result, alpha);
        }
        if (ans == -INF) {
            ans = result;
        }
        else if (sophisticated_queue[turn] == currentPlayer_) {
            ans = std::min(ans, result);
        }
        else {
            ans = std::max(ans, result);
        }
        board_[i][j] = -1;
        winner_ = -1;
        if (alpha > beta) {
            break;
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxWithAlphaBetaMove() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    for (auto [i, j] : allmoves) {
        board_[i][j] = currentPlayer_;
        int tmp = makeMinimaxWithAlphaBeta(i, j, -INF, +INF, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
        if (tmp > val) {
            val = tmp;
            ans = { i, j };
        }
        else if (tmp == val && gen() % R_VARIOTY == 0) {
            val = tmp;
            ans = { i, j };
        }
        board_[i][j] = -1;
        winner_ = -1;
    }
    makeMove(ans.first, ans.second);
}

int XYK::makeMinimaxWithDepth(int x, int y, int depth, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = 1000 - depth;
        return 1000 - depth;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -1000 + depth;
        return -1000 + depth;
    }
    int ans = -INF;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            if (depth + 1 <= MAX_DEPTH_ALPHA_BETA) {
                board_[i][j] = sophisticated_queue[turn + 1];
                int result = makeMinimaxWithDepth(i, j, depth + 1, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
                if (ans == -INF) {
                    ans = result;
                }
                else if (sophisticated_queue[turn] == currentPlayer_) {
                    ans = std::min(ans, result);
                }
                else {
                    ans = std::max(ans, result);
                }
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxWithDepthMove() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimaxWithDepth(i, j, 0, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            else if (tmp == val && gen() % R_VARIOTY == 0) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    makeMove(ans.first, ans.second);
}

int XYK::makeMinimaxWithDepthAndTime(int x, int y, int depth, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = 1000 - depth;
        return 1000 - depth;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -1000 + depth;
        return -1000 + depth;
    }
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    int ans = -INF;
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            if (depth + 1 <= MAX_DEPTH_ALPHA_BETA) {
                board_[i][j] = sophisticated_queue[turn + 1];
                int result = makeMinimaxWithDepthAndTime(i, j, depth + 1, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
                if (ans == -INF) {
                    ans = result;
                }
                else if (sophisticated_queue[turn] == currentPlayer_) {
                    ans = std::min(ans, result);
                }
                else {
                    ans = std::max(ans, result);
                }
                board_[i][j] = -1;
                winner_ = -1;
            }
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
                break;
            }
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxWithDepthAndTimeMove() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    start_time = std::chrono::steady_clock::now();
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimaxWithDepthAndTime(i, j, 0, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            else if (tmp == val && gen() % R_VARIOTY == 0) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
                break;
            }
        }
    }
    makeMove(ans.first, ans.second);
}

void XYK::makeGreedySearch(int player) {
    int ans = -INF, x = 0, y = 0;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        int val = evaluatePosition(i, j, player);
        if (val > ans) {
            ans = val;
            x = i;
            y = j;
        }
        else if (val == ans && gen() % R_VARIOTY == 0) {
            ans = val;
            x = i;
            y = j;
        }
    }
    makeMove(x, y);
}

void XYK::makeGreedySearchMove() {
    makeGreedySearch(currentPlayer_);
}

void XYK::makeGreedySearch2(int player) {
    int ans = -INF, x = 0, y = 0;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        board_[i][j] = player;
        int val = -evaluatePositionOnBoard(1 - player);
        if (val > ans) {
            ans = val;
            x = i;
            y = j;
        }
        else if (val == ans && gen() % R_VARIOTY == 0) {
            ans = val;
            x = i;
            y = j;
        }
        board_[i][j] = -1;
    }
    makeMove(x, y);
}

void XYK::makeGreedySearch2Move() {
    makeGreedySearch2(currentPlayer_);
}

int XYK::makeMinimaxWithDepth2(int x, int y, int depth, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = INF - depth;
        return INF - depth;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -INF + depth;
        return -INF + depth;
    }
    if (depth == MAX_DEPTH_MINIMAX_DEPTH) {
        int val = -evaluatePositionOnBoard(1 - sophisticated_queue[turn]);
        costPosition[hash] = val;
        return val;
    }
    int ans = -INF;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = sophisticated_queue[turn + 1];
            int result = makeMinimaxWithDepth2(i, j, depth + 1, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
            if (ans == -INF) {
                ans = result;
            }
            else if (sophisticated_queue[turn] == currentPlayer_) {
                ans = std::min(ans, result);
            }
            else {
                ans = std::max(ans, result);
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
            break;
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxWithDepth2Move() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    start_time = std::chrono::steady_clock::now();
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimaxWithDepth2(i, j, 0, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            else if (tmp == val && gen() % R_VARIOTY == 0) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    makeMove(ans.first, ans.second);
}

int XYK::makeMinimaxWithDepth3(int x, int y, int depth, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = INF - depth;
        return INF - depth;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -INF + depth;
        return -INF + depth;
    }
    if (depth == MAX_DEPTH_MINIMAX_DEPTH) {
        board_[x][y] = -1;
        int val = evaluatePosition(x, y, sophisticated_queue[turn]);
        board_[x][y] = sophisticated_queue[turn];
        costPosition[hash] = val;
        return val;
    }
    int ans = -INF;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = sophisticated_queue[turn + 1];
            int result = makeMinimaxWithDepth3(i, j, depth + 1, turn + 1, hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
            if (ans == -INF) {
                ans = result;
            }
            else if (sophisticated_queue[turn] == currentPlayer_) {
                ans = std::min(ans, result);
            }
            else {
                ans = std::max(ans, result);
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
            break;
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeMinimaxWithDepth3Move() {
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    std::pair<int, int> ans;
    int val = -INF;
    start_time = std::chrono::steady_clock::now();
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimaxWithDepth3(i, j, 0, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            else if (tmp == val && gen() % R_VARIOTY == 0) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    makeMove(ans.first, ans.second);
}

int XYK::makeRadialSearch(int x, int y, int depth, int turn, unsigned long long int hash) {
    if (costPosition.find(hash) != costPosition.end()) {
        return costPosition[hash];
    }
    checkWinner(x, y);
    if (!canWin()) {
        costPosition[hash] = 0;
        return 0;
    }
    if (winner_ == currentPlayer_) {
        costPosition[hash] = INF - depth;
        return INF - depth;
    }
    else if (winner_ != -1) {
        costPosition[hash] = -INF + depth;
        return -INF + depth;
    }
    if (depth == MAX_DEPTH_RADIAL_SEARCH) {
        int val = -evaluatePositionOnBoard(1 - sophisticated_queue[turn]);
        costPosition[hash] = val;
        return val;
    }
    std::vector<std::pair<std::pair<int, int>, int>> close_moves;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = sophisticated_queue[turn + 1];
            close_moves.push_back({ { i, j }, -evaluatePositionOnBoard(1 - sophisticated_queue[turn]) });
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    if (sophisticated_queue[turn] != currentPlayer_) {
        std::sort(close_moves.begin(), close_moves.end(), [](std::pair<std::pair<int, int>, int> l, std::pair<std::pair<int, int>, int> r) { return l.second < r.second; });
    }
    else {
        std::sort(close_moves.begin(), close_moves.end(), [](std::pair<std::pair<int, int>, int> l, std::pair<std::pair<int, int>, int> r) { return l.second > r.second; });
    }
    std::vector<std::pair<int, int>> top_close(std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL));
    for (int i = 0; i < std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL); i++) {
        top_close[i] = close_moves[i].first;
    }
    int ans = -INF;
    for (auto [i, j] : top_close) {
        if (board_[i][j] == -1) {
            board_[i][j] = sophisticated_queue[turn + 1];
            int result = makeRadialSearch(i, j, depth + 1, sophisticated_queue[turn + 1], hash ^ ZobristTable[i][j][sophisticated_queue[turn + 1]]);
            if (ans == -INF) {
                ans = result;
            }
            else if (sophisticated_queue[turn] == currentPlayer_) {
                ans = std::min(ans, result);
            }
            else {
                ans = std::max(ans, result);
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    if (ans == -INF) {
        ans = 0;
    }
    costPosition[hash] = ans;
    return ans;
}

void XYK::makeRadialSearchMove() {
    std::vector<std::pair<std::pair<int, int>, int>> close_moves;
    std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
    for (auto [i, j] : allmoves) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            close_moves.push_back({ { i, j }, -evaluatePositionOnBoard(1 - sophisticated_queue[currentTurn]) });
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    std::sort(close_moves.begin(), close_moves.end(), [](std::pair<std::pair<int, int>, int> l, std::pair<std::pair<int, int>, int> r) { return l.second > r.second; });
    std::vector<std::pair<int, int>> top_close(std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL));
    for (int i = 0; i < std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL); i++) {
        top_close[i] = close_moves[i].first;
    }
    std::pair<int, int> ans;
    int val = -INF;
    for (auto [i, j] : top_close) {
        if (board_[i][j] == -1) {
            board_[i][j] = currentPlayer_;
            int tmp = makeRadialSearch(i, j, 0, currentTurn, currentHash ^ ZobristTable[i][j][currentPlayer_]);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            else if (tmp == val && gen() % R_VARIOTY == 0) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
    }
    makeMove(ans.first, ans.second);
}

struct Node {
    std::vector<std::vector<int>> board;
    int turn; // current turn in this node
    std::pair<int, int> move; // move that we did to get into this node
    Node* parent;
    std::vector<Node*> children;
    double wins = 0; // win = 1, draw = 0.5, lose = 0
    int visits = 0;
    bool terminate = false;
    std::vector<std::pair<int, int>> untriedMoves;

    Node(std::vector<std::vector<int>> brd, int t, const std::pair<int, int>& m, Node* par) : board(brd), turn(t), move(m), parent(par) {
        untriedMoves = allowedMoves(brd);
    }

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }

    Node* selectChild(double c) {
        Node* bestChild = nullptr;
        double bestValue = -1;
        for (auto child : children) {
            double uct = (double)child->wins / child->visits + c * sqrtl(log(visits + 1) / child->visits);
            if (uct > bestValue) {
                bestValue = uct;
                bestChild = child;
            }
        }
        return bestChild;
    }

    Node* addChild(const std::pair<int, int>& m, int nextTurn) {
        std::vector<std::vector<int>> newBoard = board;
        newBoard[m.first][m.second] = sophisticated_queue[turn];
        Node* child = new Node(newBoard, nextTurn, m, this);
        if (giveWinner(m.first, m.second, newBoard) != -1) {
            child->terminate = true;
        }
        auto it = remove_if(untriedMoves.begin(), untriedMoves.end(), [&m](const std::pair<int, int>& move) {
            return move.first == m.first && move.second == m.second;
            });
        untriedMoves.erase(it, untriedMoves.end());
        children.push_back(child);
        return child;
    }

    void update(double result, int vis) {
        visits += vis;
        wins += result;
    }
};

int simulate(std::vector<std::vector<int>> board, int turn, std::pair<int, int> last) {
    int win = giveWinner(last.first, last.second, board);
    if (win != -1) {
        return win;
    }
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < M_GRID_SIZE; i++) {
        for (int j = 0; j < N_GRID_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                moves.push_back({ i, j });
            }
        }
    }
    std::shuffle(moves.begin(), moves.end(), gen);
    int i = 0;
    while (i < moves.size()) {
        board[moves[i].first][moves[i].second] = sophisticated_queue[turn];
        int win = giveWinner(moves[i].first, moves[i].second, board);
        if (win != -1) {
            return win;
        }
        i++;
        turn++;
    }
    return -1;
}

std::atomic<int> total_0(0);
std::atomic<int> total_1(0);
std::atomic<int> total_draw(0);

void paralel_simulate(int iter, std::vector<std::vector<int>> board, int turn, std::pair<int, int> last) {
    int ltotal_0 = 0;
    int ltotal_1 = 0;
    int ltotal_draw = 0;
    for (int _ = 0; _ < iter; _++) {
        int result = simulate(board, turn, last);
        if (result == -1) {
            ltotal_draw++;
        }
        else if (result == 0) {
            ltotal_0++;
        }
        else {
            ltotal_1++;
        }
    }
    total_0 += ltotal_0;
    total_1 += ltotal_1;
    total_draw += ltotal_draw;
}

std::pair<int, int> MCTS(double c, std::vector<std::vector<int>> rootBoard, int currentturn) {
    std::vector<std::pair<int, int>> legal = allowedMoves(rootBoard);

    if (legal.empty()) {
        return { -1, -1 };
    }

    for (const auto& move : legal) {
        rootBoard[move.first][move.second] = sophisticated_queue[currentTurn];
        int win = giveWinner(move.first, move.second, rootBoard);
        rootBoard[move.first][move.second] = -1;
        if (win != -1) {
            return move;
        }
    }
    if (FIRST_PLAYER == 3 && QUEUE == 2) {
        if (sophisticated_queue[currentTurn] == sophisticated_queue[currentTurn + 1]) {
            for (const auto& move1 : legal) {
                rootBoard[move1.first][move1.second] = sophisticated_queue[currentTurn];
                for (const auto& move2 : legal) {
                    if (move1.first == move2.first && move1.second == move2.second) {
                        continue;
                    }
                    rootBoard[move2.first][move2.second] = sophisticated_queue[currentTurn];
                    int win = giveWinner(move2.first, move2.second, rootBoard);
                    rootBoard[move2.first][move2.second] = -1;
                    if (win != -1) {
                        return move1;
                    }
                }
                rootBoard[move1.first][move1.second] = -1;
            }
        }
    }
    else if (SECOND_PLAYER == 3 && QUEUE == 1) {
        if (sophisticated_queue[currentTurn] == sophisticated_queue[currentTurn + 1]) {
            for (const auto& move1 : legal) {
                rootBoard[move1.first][move1.second] = sophisticated_queue[currentTurn];
                for (const auto& move2 : legal) {
                    if (move1.first == move2.first && move1.second == move2.second) {
                        continue;
                    }
                    rootBoard[move2.first][move2.second] = sophisticated_queue[currentTurn];
                    int win = giveWinner(move2.first, move2.second, rootBoard);
                    rootBoard[move2.first][move2.second] = -1;
                    if (win != -1) {
                        return move1;
                    }
                }
                rootBoard[move1.first][move1.second] = -1;
            }
        }
    }

    start_time = std::chrono::steady_clock::now();
    Node* root = new Node(rootBoard, currentturn, std::make_pair(-1, -1), nullptr);
    for (int i = 0;; i++) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit_for_MCST) {
            break;
        }
        Node* node = root;
        int turn = currentturn;
        std::vector<std::vector<int>> board = rootBoard;
        std::pair<int, int> last = { -1,-1 };
        int result = -1;
        // Selection
        while (node->untriedMoves.empty() && !node->children.empty()) {
            node = node->selectChild(c);
            board[node->move.first][node->move.second] = sophisticated_queue[turn];
            last = node->move;
            turn = node->turn;
        }

        // Expansion
        if (!node->untriedMoves.empty() && !node->terminate) {
            std::pair<int, int> m = node->untriedMoves[gen() % node->untriedMoves.size()];
            board[m.first][m.second] = sophisticated_queue[node->turn];
            last = m;
            node = node->addChild(m, node->turn + 1);
            turn = node->turn;
        }

        // Paralel simulation
        //int total = 800, num_threads = std::thread::hardware_concurrency();
        //int it = total / num_threads;
        //total_0 = 0;
        //total_1 = 0;
        //total_draw = 0;
        //std::vector<std::thread> threads;
        //threads.reserve(total);
        //for (int i = 0; i < num_threads; i++) {
        //    threads.emplace_back(paralel_simulate, it, board, turn, last);
        //}
        //for (auto& i : threads) {
        //    i.join();
        //}
        //// Backpropagation
        //while (node != nullptr) {
        //    node->update(0.5 * total_draw, total);
        //    if (sophisticated_queue[node->turn] == 0) {
        //        node->update(total_1, 0);
        //    }
        //    else {
        //        node->update(total_0, 0);
        //    }
        //    node = node->parent;
        //}

        // Paralel simulation one thread
        int total = 16;
        int ltotal_0 = 0;
        int ltotal_1 = 0;
        int ltotal_draw = 0;
        for (int _ = 0; _ < total; _++) {
            int result = simulate(board, turn, last);
            if (result == -1) {
                ltotal_draw++;
            }
            else if (result == 0) {
                ltotal_0++;
            }
            else {
                ltotal_1++;
            }
        }
        // Backpropagation
        while (node != nullptr) {
            node->update(0.5 * ltotal_draw, total);
            if (sophisticated_queue[node->turn] == 0) {
                node->update(ltotal_1, 0);
            }
            else {
                node->update(ltotal_0, 0);
            }
            node = node->parent;
        }

        //// Simulation
        //if (result == -1) {
        //    result = simulate(board, turn, last);
        //}
        //// Backpropagation
        //while (node != nullptr) {
        //    if (sophisticated_queue[node->turn] == result) {
        //        node->update(0, 1);
        //    }
        //    else if (result == -1) {
        //        node->update(0.5, 1);
        //    }
        //    else {
        //        node->update(1, 1);
        //    }
        //    node = node->parent;
        //}
    }

    Node* bestChild = nullptr;
    double bestWinRate = 0;
    for (auto child : root->children) {
        double winRate = (double)child->wins / child->visits;
        if (winRate >= bestWinRate) {
            bestWinRate = winRate;
            bestChild = child;
        }
    }
    std::pair<int, int> bestMove = bestChild->move;
    delete root;
    return bestMove;
}

std::mutex mutex;
std::map<std::pair<int, int>, int> bestMoves;

void threadWork(std::vector<std::vector<int>> rootBoard, int position) {
    auto ans = MCTS((double)gen() / RAND_MAX / RAND_MAX / 4 + 1, rootBoard, position);
    std::lock_guard<std::mutex> lock(mutex);
    bestMoves[ans]++;
}

void XYK::makeMCTSmove() {
    int position = currentTurn;
    if (QUEUE == 1) {
        position = 0;
    }
    else if (QUEUE == 2) {
        position = 1;
    }
    else if (QUEUE == 3) {
        position = 0;
    }
    else if (QUEUE == 4) {
        position = currentTurn;
    }
    /*auto ans = MCTS(board_, position);
    makeMove(ans.first, ans.second);*/

    // Paralel MCTS
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    threads.reserve(num_threads);
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(threadWork, getBoard(), position);
    }
    for (auto& i : threads) {
        i.join();
    }
    std::pair<int, int> best;
    int res = 0;
    for (auto [i, j] : bestMoves) {
        if (j > res) {
            res = j;
            best = i;
        }
    }
    bestMoves.clear();
    makeMove(best.first, best.second);
}
