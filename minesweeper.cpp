#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */
bool is_complete(const char mines[9][9], const char revealed[9][9]) {
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (mines[row][col] == '.' && (revealed[row][col] == '?' || revealed[row][col] == '*'))
        return false;
    }
  }
  return true;
}

int count_mines(const char position[2], const char mines[9][9]) {
  return count_char(position, mines, '*');
}

int count_char(const char position[2], const char mines[9][9], char ch) {
  int row, col;
  int count = 0;
  convert_position_to_row_col(position, row, col);
  for (int i = row - 1; i <= row + 1; ++i) {
    for (int j = col -1; j <= col + 1; ++j) {
      if (valid_coordinates(i, j) && mines[i][j] == ch)
        ++count;
    }
  }
  return count;
}

MoveResult make_move(const char position[3], const char mines[9][9], char revealed[9][9]) {
  int row, col;
  convert_position_to_row_col(position, row, col);

  if (!valid_coordinates(row, col) || (position[2] && position[2] != '*' && position[2] != '\0'))
    return INVALID_MOVE;

  if (revealed[row][col] != '?')
    return REDUNDANT_MOVE;

  if (mines[row][col] == '*' && position[2] != '*')
    return BLOWN_UP;

  if (position[2] == '*') {
    revealed[row][col] = '*';
    return VALID_MOVE;
  }

  if (is_complete(mines, revealed))
    return SOLVED_BOARD;

  int mine_count = count_mines(position, mines);
  if (mine_count == 0) {
    revealed[row][col] = ' ';
    for (int i = row - 1; i <= row + 1; ++i) {
      for (int j = col - 1; j <= col + 1; ++j) {
        if (!valid_coordinates(i, j)) continue;
        
        char position[3] = {i + 'A', j + '1', '\0'};
        make_move(position, mines, revealed);
      }
    }
  } else {
    revealed[row][col] = mine_count + '0';
  }

  return VALID_MOVE;
}

bool find_safe_move(const char revealed[9][9], char move[MAX_LENGTH]) {
  for (int row = 0; row < 9; ++row) {
    for (int col = 0; col < 9; ++col) {
      if (revealed[row][col] >= '1' && revealed[row][col] <= '8') {
        char position[3] = {row + 'A', col + '1', ' '};
        char end_char;
        if (count_mines(position, revealed) == (int) (revealed[row][col] - '0')) {
          end_char = '\0';
        } else if ((count_char(position, revealed, '?') + count_mines(position, revealed)) == (int) (revealed[row][col] - '0')) {
          end_char = '*';
        } else {
          continue;
        }
        
        for (int i = row - 1; i <= row + 1; ++i) {
          for (int j = col - 1; j <= col + 1; ++j) {
            if (!valid_coordinates(i, j) || revealed[i][j] != '?') continue;
            
            char safe_pos[3] = {i + 'A', j + '1', end_char};
            strcpy(move, safe_pos);
            return true;
          }
        }
      }
    }
  }
  return false;
}

void convert_position_to_row_col(const char position[2], int &row, int &col) {
  row = position[0] - 'A';
  col = position[1] - '1';
}

bool valid_coordinates(const int row, const int col) {
  return row >= 0 && row < 9 && col >= 0 && col < 9;
}
