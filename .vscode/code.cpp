#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
using namespace std;

#define MAX_SIZE 10  // Maximum allowed maze size

// ANSI escape codes for colors
#define RESET "\033[0m"
#define WALL_COLOR "\033[41m"  // Red background for walls
#define PATH_COLOR "\033[42m"  // Green background for path
#define RAT_COLOR "\033[43m"   // Yellow background for rat
#define TRIED_PATH_COLOR "\033[44m"  // Blue background for backtracked paths

// Direction arrays for possible movements (DOWN, RIGHT, UP, LEFT)
const int dx[] = {1, 0, -1, 0};
const int dy[] = {0, 1, 0, -1};

void sleep_ms(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

void printMaze(vector<vector<int>>& maze, vector<vector<int>>& sol, int N, int ratX = -1, int ratY = -1) {
    system("cls");  // For Windows
    
    // Print top border
    cout << "+";
    for (int j = 0; j < N; ++j) {
        cout << "---+";
    }
    cout << endl;

    // Print maze contents
    for (int i = 0; i < N; ++i) {
        cout << "|";
        for (int j = 0; j < N; ++j) {
            if (ratX == i && ratY == j) {
                cout << RAT_COLOR << "🐀 " << RESET;  // Changed to mouse emoji
            } else if (sol[i][j] == 1) {
                cout << PATH_COLOR << " * " << RESET;
            } else if (sol[i][j] == 2) {  // Backtracked path
                cout << TRIED_PATH_COLOR << " . " << RESET;
            } else if (maze[i][j] == 0) {
                cout << WALL_COLOR << "###" << RESET;
            } else {
                cout << "   ";
            }
            cout << "|";
        }
        cout << endl;
        
        // Print horizontal borders
        cout << "+";
        for (int j = 0; j < N; ++j) {
            cout << "---+";
        }
        cout << endl;
    }
    cout << endl;
}

bool isSafe(vector<vector<int>>& maze, int x, int y, int N) {
    return (x >= 0 && x < N && y >= 0 && y < N && maze[x][y] == 1);
}

bool solveMazeUtil(vector<vector<int>>& maze, int x, int y, vector<vector<int>>& sol, int N) {
    // Base case: reached the destination
    if (x == N-1 && y == N-1) {
        sol[x][y] = 1;
        printMaze(maze, sol, N, x, y);
        return true;
    }

    // Check if current position is valid
    if (isSafe(maze, x, y, N)) {
        // Mark current cell as part of the path
        sol[x][y] = 1;
        printMaze(maze, sol, N, x, y);
        sleep_ms(100);

        // Try all possible directions
        for (int i = 0; i < 4; i++) {
            int nextX = x + dx[i];
            int nextY = y + dy[i];
            
            if (solveMazeUtil(maze, nextX, nextY, sol, N))
                return true;
        }

        // If no direction works, backtrack
        sol[x][y] = 2;  // Mark as tried path
        printMaze(maze, sol, N, x, y);
        sleep_ms(100);
        return false;
    }
    return false;
}

bool solveMaze(vector<vector<int>>& maze, int N) {
    vector<vector<int>> sol(N, vector<int>(N, 0));
    
    if (!solveMazeUtil(maze, 0, 0, sol, N)) {
        cout << "No solution exists!" << endl;
        return false;
    }
    return true;
}

int main() {
    int N;
    cout << "Enter maze size (1-" << MAX_SIZE << "): ";
    cin >> N;
    
    // Validate input size
    if (N < 1 || N > MAX_SIZE) {
        cout << "Invalid maze size! Size must be between 1 and " << MAX_SIZE << endl;
        return 1;
    }

    // Create maze of size N x N
    vector<vector<int>> maze(N, vector<int>(N));
    
    cout << "\nEnter the maze layout (" << N << "x" << N << "):\n";
    cout << "Enter 0 for wall, 1 for path\n";
    cout << "Enter row by row:\n\n";
    
    // Input maze layout
    for (int i = 0; i < N; i++) {
        cout << "Row " << i+1 << ": ";
        for (int j = 0; j < N; j++) {
            cin >> maze[i][j];
            // Validate input
            if (maze[i][j] != 0 && maze[i][j] != 1) {
                cout << "Invalid input! Please enter only 0 or 1.\n";
                j--; // Retry this position
            }
        }
    }

    cout << "\nMaze layout entered. Finding path...\n";
    sleep_ms(1000);
    
    solveMaze(maze, N);
    
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    return 0;
}