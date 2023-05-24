#include <iostream>
#include <vector>

//for visuals
#include <Windows.h>

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


void gotoxy(short a, short b)
{
    COORD coordinates;
    coordinates.X = a;
    coordinates.Y = b;

SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE), coordinates);

}

//add maps, sets, etc

// Define the ChessBoard class
class ChessBoard {
private:
    // Define the chess pieces as characters
    char board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };
    // Define a vector to store the captured pieces
    vector<char> captured_pieces;
public:
    // Define a function to print the current state of the board
    void print_board() {
        gotoxy(0,0);
        cout << "  0 1 2 3 4 5 6 7\n";
        gotoxy(0,1);
        for (int i = 0; i < 8; i++) {
            cout << i << " ";
            for (int j = 0; j < 8; j++) {
                if ((i+j) % 2 ==0){
                    // paint white

                    SetConsoleTextAttribute(hConsole, 0xf0);//baltas background
                }

                cout << board[i][j] << " ";
                SetConsoleTextAttribute(hConsole, 0xf);//reset color
            }
            gotoxy(0,i+2);
        }
    }
    // Define a function to move a piece on the board
    bool move_piece(int start_row, int start_col, int end_row, int end_col) {
        // Check if the move is valid
        if (is_valid_move(start_row, start_col, end_row, end_col)) {
            // If the end position is occupied by an opposing piece, capture it
            if (board[end_row][end_col] != ' ') {
                captured_pieces.push_back(board[end_row][end_col]);
            }
            // Move the piece to the end position
            board[end_row][end_col] = board[start_row][start_col];
            board[start_row][start_col] = ' ';
            return true;
        }
        else return false;
    }
    // Define a function to check if a move is valid
    bool is_valid_move(int start_row, int start_col, int end_row, int end_col) {
        // Check if the start and end positions are within the bounds of the board
        if (start_row < 0 || start_row > 7 || start_col < 0 || start_col > 7 || end_row < 0 || end_row > 7 || end_col < 0 || end_col > 7) {
            return false;
        }
        // Check if the start position is empty
        if (board[start_row][start_col] == ' ') {
            return false;
        }
        // Check if the end position is occupied by a piece of the same color
        if (board[end_row][end_col] != ' ' && board[end_row][end_col] < 'a' && board[start_row][start_col] < 'a' || board[end_row][end_col] != ' ' && board[end_row][end_col] >= 'a' && board[start_row][start_col] >= 'a') {
            return false;
        }
        // Check if the piece can move to the end position
        switch (board[start_row][start_col]) {
            case 'p': // Pawn
                // Check if the pawn is moving forward
                if (start_col == end_col) {
                    // Check if the pawn is moving one or two squares
                    if (start_row == 1 && end_row == 3 && board[2][start_col] == ' ' && board[3][start_col] == ' ') {
                        return true;
                    }
                    if (start_row == 6 && end_row == 4 && board[5][start_col] == ' ' && board[4][start_col] == ' ') {
                        return true;
                    }
                    if (start_row + 1 == end_row && board[end_row][end_col] == ' ') {
                        return true;
                    }
                }
                // Check if the pawn is capturing
                if (start_col - 1 == end_col || start_col + 1 == end_col) {
                    if (start_row + 1 == end_row && board[end_row][end_col] != ' ') {
                        return true;
                    }
                }
                break;
            case 'P': // Pawn
                // Check if the pawn is moving forward
                if (start_col == end_col) {
                    // Check if the pawn is moving one or two squares
                    if (start_row == 6 && end_row == 4 && board[5][start_col] == ' ' && board[4][start_col] == ' ') {
                        return true;
                    }
                    if (start_row == 1 && end_row == 3 && board[2][start_col] == ' ' && board[3][start_col] == ' ') {
                        return true;
                    }
                    if (start_row - 1 == end_row && board[end_row][end_col] == ' ') {
                        return true;
                    }
                }
                // Check if the pawn is capturing
                if (start_col - 1 == end_col || start_col + 1 == end_col) {
                    if (start_row - 1 == end_row && board[end_row][end_col] != ' ') {
                        return true;
                    }
                }
                break;
            case 'r': // Rook
            case 'R':
                // Check if the rook is moving horizontally or vertically
                if (start_row == end_row || start_col == end_col) {
                    // Check if the path is clear
                    if (is_path_clear(start_row, start_col, end_row, end_col)) {
                        return true;
                    }
                }
                break;
            case 'n': // Knight
            case 'N':
                // Check if the knight is moving in an L shape
                if ((abs(start_row - end_row) == 2 && abs(start_col - end_col) == 1) || (abs(start_row - end_row) == 1 && abs(start_col - end_col) == 2)) {
                    return true;
                }
                break;
            case 'b': // Bishop
            case 'B':
                // Check if the bishop is moving diagonally
                if (abs(start_row - end_row) == abs(start_col - end_col)) {
                    // Check if the path is clear
                    if (is_path_clear(start_row, start_col, end_row, end_col)) {
                        return true;
                    }
                }
                break;
            case 'q': // Queen
            case 'Q':
                // Check if the queen is moving horizontally, vertically, or diagonally
                if (start_row == end_row || start_col == end_col || abs(start_row - end_row) == abs(start_col - end_col)) {
                    // Check if the path is clear
                    if (is_path_clear(start_row, start_col, end_row, end_col)) {
                        return true;
                    }
                }
                break;
            case 'k': // King
            case 'K':
                // Check if the king is moving one square in any direction
                if (abs(start_row - end_row) <= 1 && abs(start_col - end_col) <= 1) {
                    return true;
                }
                break;
        }
        return false;
    }
    // Define a function to check if the path between two squares is clear
    bool is_path_clear(int start_row, int start_col, int end_row, int end_col) {
        int row_dir = (end_row - start_row) / max(abs(end_row - start_row), 1);
        int col_dir = (end_col - start_col) / max(abs(end_col - start_col), 1);
        for (int i = start_row + row_dir, j = start_col + col_dir; i != end_row || j != end_col; i += row_dir, j += col_dir) {
            if (board[i][j] != ' ') {
                return false;
            }
        }
        return true;
    }

    vector<char> get_captured_pieces(){
        return captured_pieces;
    }
};

int main() {
    ChessBoard board;

    int Turn_count = 1;

    gotoxy(0,10);
    cout << "Hint: Choose your position (X and Y of your piece; X and Y where you want to move):\n";
    while (true){
        board.print_board();

        gotoxy(0,10);
        // clear lines
        cout << '\n' << string(50, ' ') << '\n'<< string(50, ' ') << '\n'<< string(50, ' ') << '\n'<< string(50, ' ') << '\n' << string(50, ' ') << '\n';

        gotoxy(0,11);
        cout << "Turn " << Turn_count << '\n';
        cout << "Captured pieces: ";

        vector <char> Captured = board.get_captured_pieces();

        for (int i = 0; i < Captured.size(); i++){
            cout << Captured.at(i) << " ";
        }
        cout << "\n";

        // Check win
        if (Captured.size() > 0){
            if (Captured.at(Captured.size()-1) == 'K') {
                    cout << "Black player wins!";
                    break;
            }
            if (Captured.at(Captured.size()-1) == 'k') {
                    cout << "White player wins!";
                    break;
            }
        }


        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        if (board.move_piece(x1, y1, x2, y2)) Turn_count++;
        else cout << "INVALID MOVE";
        //         your piece   ->x,  y|  x,  y<- where u want to move

    }
    return 0;
}
