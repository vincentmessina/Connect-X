/*
The below function analyzes a game of connect X respented
 as an array of moves.
 
In Connect X, the width and and height can each be any
 integers greater than 2 (not just 7 x 6 as in Connect 4)
 the length (the number of pieces in a row required to win)
 can also be any integer greater than 2 and at most the maximum
 of the width and the height (not just 4 as in Connect 4).
 */
#include "trace-2d.h"
#include <stdio.h>
#include <assert.h>


// Constants used in returning values for game analysis
const int INVALID_GAME = -1;
const int TIE = 1;
const int MAX_BOARD_SIZE = 1024;


// max(num1, num2) returns the max of num1
//    num 2
int max(int num1, int num2) {
  if(num1 > num2)
    return num1;
  else return num2;
}

// in_bounds(row, col, width, height) returns
//   1 if the given item is bounds otherwise 0
int in_bounds(int row, int col, int width, int height) {
  if (row < 0) return 0;
  if (col < 0) return 0;
  if (row >= width) return 0;
  if (col >= height) return 0;
  return 1;
}

// valid_move(move_column, height, width) returns
//   1 if the given item is bounds otherwise 0
int valid_move(int move_column, int height, int width,
                int board[1024][1024]) {
  if (move_column < 0) {
    return 0;
  }
  else if (move_column > width - 1){
    return 0;
  }
  else if (board[0][move_column] != 0){
    return 0;
  }
  return 1;
}

// make_move(width, height, board, player,
//            move_column) assigns a move
//            to player position
int make_move(
  int width,
  int height,
  int board[1024][1024],
  int player,
  int move_column
) {
  for (int row = height - 1; row >=0; row--) {
    if (board[row][move_column] == 0) {
      board[row][move_column] = player;
      return row;
    }
  }
  return 0;
}

// count_pieces_row(row, width, player)
//   returns the number of consectiuve pieces
//   in a row
int count_pieces_row(int row[], int width, int player) {
  int curr_consecutive = 0;
  int max_consecutive = 0;
  
  for (int col = 0; col < width; col++){
    if (row[col] != player) {
       curr_consecutive = 0;
    }
    else {
      curr_consecutive++;
      if (curr_consecutive > max_consecutive) {
        max_consecutive = curr_consecutive;
      }
    }
  }
  return max_consecutive;
}

// count_pieces_row(row, width, player)
//   returns the number of consectiuve pieces
//   in a column
int count_pieces_col(
  int width,
  int height,
  int player,
  int board[1024][1024],
  int col
) {
  int curr_consecutive = 0;
  int max_consecutive = 0;
  
  for (int row = 0; row < height; row++){
    if (board[row][col] != player) {
      curr_consecutive = 0;
    }
    else {
      curr_consecutive++;
      if (curr_consecutive > max_consecutive) {
        max_consecutive = curr_consecutive;
      }
    }
  }
  return max_consecutive;
}

// count_pieces_row(row, width, player)
//   returns the number of consectiuve pieces
//   in a left diagonal
int count_pieces_left_diagonal(
  int width,
  int height,
  int player,
  int board[1024][1024],
  int move_row,
  int move_col
) {
  int current_row = move_row - 1;
  int current_col = move_col - 1;
  int left_counter = 0;

  while (in_bounds(current_row, current_col, width, height)) {
    if (board[current_row][current_col] != player) {
      break;
    } else {
      left_counter++;
      current_col--;
      current_row--;
    }
  }

  current_row = move_row + 1;
  current_col = move_col + 1;
  int right_counter = 0;

  while (in_bounds(current_row, current_col, width, height)) {
    if (board[current_row][current_col] != player) {
      break;
    } else {
      right_counter++;
      current_col++;
      current_row++;
    }
  }

  return (left_counter + right_counter + 1);
}

// count_pieces_row(row, width, player)
//   returns the number of consectiuve pieces
//   in a right diagonal
int count_pieces_right_diagonal(
  int width,
  int height,
  int player,
  int board[1024][1024],
  int move_row,
  int move_col
) {
  int current_row = move_row + 1;
  int current_col = move_col - 1;
  int left_counter = 0;

  while (in_bounds(current_row, current_col, width, height)) {
    if (board[current_row][current_col] != player) {
      break;
    } else {
      left_counter++;
      current_col--;
      current_row++;
    }
  }

  current_row = move_row - 1;
  current_col = move_col + 1;
  int right_counter = 0;

  while (in_bounds(current_row, current_col, width, height)) {
    if (board[current_row][current_col] != player) {
      break;
    } else {
      right_counter++;
      current_col++;
      current_row--;
    }
  }

  return (left_counter + right_counter + 1);
}

// connect_analysis(moves, width, height, length) analyzes a game of connect X
//  and returns the results which take the following values:
//      INVALID_GAME to indicate an illegal move
//      TIE to indicate that the board is full, and no player has won
//      a positive integer x to indicate that player #1 won on move #x
//      a negative integer -x to indicate that player #2 won on move #x
int connect_analysis(const int moves[], int width, int height, int length) {
    
  // Assertions
  assert(width >= 2);
  assert(height >= 2);
  assert(length >= 2);
  assert(width * height <= MAX_BOARD_SIZE);
  assert(length < max(width, height));

  // Create Board
  // 0: empty space, 1: player1, 2: player2
  int board[1024][1024];
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      board[row][col] = 0;
    }
  }

  // Loop through all moves
  for(int i = 0; i < width * height; i++) {
    // print_board(width, height, board);

    int current_player = 1;
    if (i % 2 == 1) current_player = 2;

    int move_col = moves[i];
    if (valid_move(move_col, height, width, board) == 0) {
      return INVALID_GAME;
    }

    int move_row = make_move(width, height, board, current_player, move_col);

    int row_pieces = count_pieces_row(board[move_row], width, current_player);
    int col_pieces = count_pieces_col
      (width, height, current_player, board, move_col);
    int left_diagonal_pieces = count_pieces_left_diagonal
      (width, height, current_player, board, move_row, move_col);
    int right_diagonal_pieces = count_pieces_right_diagonal
      (width, height, current_player, board, move_row, move_col);
    
    if (
      row_pieces >= length ||
      col_pieces >= length ||
      left_diagonal_pieces >= length ||
      right_diagonal_pieces >= length
    ) {
      if (current_player == 1) {
        return i + 1;
      } else {
        return (i + 1) * -1;
      }
    }
  }
  return TIE;
}
