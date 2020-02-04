import java.io.File;
import java.util.Scanner;

/**
 * The Prefiltering algorithm that takes in the sudoku board as a parameter and
 * returns a 3D boolean with false in positions where a number CANNOT go
 *
 * @author Abdullah Karbanee 21865728
 */
public class Prefiltering {
  private static boolean[][][] noOptions = new boolean[9][9][9];
  private static int[][] sudoku = new int[9][9];

/**
 * The main function of this class that is called if mode 2 is 
 * selected in the Sudoku class
 *
 * @param args The name of the file
 * @Exception Error is displayed if the file is not found and 
 * the function exits
 * 
 */
  public static void main(String[] args) {

    Scanner scan;
    try {
      scan = new Scanner(new File(args[0]));
    } catch (Exception e) {
      System.out.println(args[0] + " No file found");
      return;
    }
    for (int i = 0; i < 9; i++) {
      String line = scan.nextLine();
      for (int k = 0; k < 9; k++) {
        if (line.charAt(k) == '.') {
          sudoku[i][k] = 0;
        } else {
          sudoku[i][k] = Integer.parseInt(line.substring(k, k + 1));
        }
      }
    }
    scan.close();

    noOptions = prefilter(sudoku);

    //This part of the code prints the outcome to output
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        String line = "";
        for (int k = 0; k < 9; k++) {
          if (noOptions[i][j][k]) {
            line = line + (k + 1) + " ";
          }
        }
        System.out.println(line);
      }
    }
  }

/**
 * This functon sets all the values of the 3D Boolean array to true
 * and calls the actual sub functions of prefilter
 *
 * @param sudokuBoard The actual board that will be inspected
 * @return Returns the 3D Boolean board
 */
  public static boolean[][][] prefilter(int[][] sudokuBoard) {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        for (int k = 0; k < 9; k++) {
          noOptions[i][j][k] = true;
        }
      }
    }
    
    for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
        explosionCheck(i, k, sudokuBoard[i][k]);
      }
    }
    
    //Possible additional code for the prefiltering process 
    /*for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        secondPreCheck(i, j);
      }
    }*/
    

    return noOptions;
  }

/**
 * Adds the additional prefiltering in order to better solve the boards
 *
 * @param sudokuBoard The board to be prefiltered
 * @return Returns the 3D array of options for the board
 */
  public static boolean[][][] prefilterV2(int[][] sudokuBoard) {
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        for (int k = 0; k < 9; k++) {
          noOptions[i][j][k] = true;
        }
      }
    }

    for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
        explosionCheck(i, k, sudokuBoard[i][k]);
      }
    }

     
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        secondPreCheck(i, j);
      }
    }

    return noOptions;
  }


/**
 * This function is the initial function that will call the recursive function
 * that checks for if each block has only one solution value
 * 
 * @param row Row that will be inspected
 * @param col Column that will be inspected
 * @param val of the sudoku board at that position
 */

  private static void explosionCheck(int row, int col, int val) {
    rowColCheck(row, col, val);
    gridCheck(row, col, val);
  }

/**
 * This function inspects a 1D boolean array, returning true if only one
 * value can be inputed into that position
 * 
 * @param arr is the boolean 1D array which will be inspected
 * @return returns the value of the only solution or 0 if there is no solution
 */
  private static int oneSol(boolean[] arr) {
    int oneSol = 0;
    int sol = 0;
    for (int i = 0; i < 9 && oneSol <= 1; i++) {
      if (arr[i]) {
        oneSol++;
        sol = i + 1; 
      }
    }
    if (oneSol == 1) {
      return sol;
    } else {
      return 0;
    }
  }

/**
 * Checks a col and row of the board to do the constraint check
 *
 * @param row Row that will be inspected
 * @param col Column that will be inspected
 * @param val of the sudoku board at that position
 */
  private static void rowColCheck(int row, int col, int val) {
    if (!(val == 0)) {
      for (int j = 0; j < 9; j++) {
         noOptions[row][col][j] = false;
      }
      noOptions[row][col][val - 1] = true;
      
      for (int j = 0; j < 9; j++) {
        //Does the check for rows
        if (noOptions[row][j][val - 1] && !(j == col)) {
          noOptions[row][j][val - 1] = false;
          int sol = oneSol(noOptions[row][j]);
          if (!(sol == 0)) {
            explosionCheck(row, j, sol);
          } 
        }

        //Does the check for columns
        if (noOptions[j][col][val - 1] && !(j == row)) {
          noOptions[j][col][val - 1] = false;
          int sol = oneSol(noOptions[j][col]);
          if (!(sol == 0)) {
            explosionCheck(j, col, sol);
          } 
        }
      }
    }
  }

/**
 * Checks a subgrid in order to do the constraint check
 *
 * @param row Row that will be inspected
 * @param col Column that will be inspected
 * @param val of the sudoku board at that position
 */
  private static void gridCheck(int row, int col, int val) {
    if (!(val == 0)) {
      for (int m = 0; m < 3; m++) {
        for (int n = 0; n < 3; n++) {
          noOptions[row][col][3 * m + n] = false;
        }
      }
      noOptions[row][col][val - 1] = true;
      
      for (int m = 0; m < 3; m++) {
        for (int n = 0; n < 3; n++) {
          if (noOptions[3 * (row / 3) + m][3 * (col / 3) + n][val - 1]
            && !(3 * (row / 3) + m == row && 3 * (col / 3) + n == col)) {
            noOptions[3 * (row / 3) + m][3 * (col / 3) + n][val - 1] = false;
            int sol = oneSol(noOptions[3 * (row / 3) + m][3 * (col / 3) + n]);
            if (!(sol == 0)) {
              explosionCheck(3 * (row / 3) + m, 3 * (col / 3) + n, sol);
            } 
          }
        }
      }
    }
  }

/**
 * This check checks to see if a number is alone in its row/col/subgrid
 *
 * @param row Row that will be inspected
 * @param col Column that will be inspected
 * @param val of the sudoku board at that position
 * @return Returns whether or not theere is only one solution
 */
  private static boolean onlySol(int row, int col, int val) {
    int count = 0;
   
    //checks rows
    for (int i = 0; i < 9; i++) {
      if (noOptions[row][i][val - 1]) {
        count++;
      }
    }
    if (count == 1) {
      return true;
    }

    //checks cols
    count = 0;
    for (int i = 0; i < 9; i++) {
      if (noOptions[i][col][val - 1]) {
        count++;
      }
    }
    if (count == 1) {
      return true;
    }
    
    //checks subgrids
    count = 0;
    for (int i = 0; i < 9; i++) {
      if (noOptions[3 * (row / 3) + (i % 3)]
        [3 * (col / 3) + (i / 3)][val - 1]) {
        count++; 
      }
    }

    return (count == 1); 
  }

/**
 * As a consquence of the second check, checks the other blocks also affected
 * by the change and changes them accordingly
 *
 * @param row The row of the recently changed block
 * @param col The col of the recently changed block
 * @param val The value of the block that was recently changed
 */ 
  private static void secondPreFind(int row, int col, int val) {
    for (int i = 0; i < 9; i++) {
      if (noOptions[row][i][val - 1]) {
        secondPreCheck(row, i);
      }
      if (noOptions[i][col][val - 1]) {
        secondPreCheck(i, col);
      }
      if (noOptions[3 * (row / 3) + (i % 3)]
         [3 * (col / 3) + (i / 3)][val - 1]) {
        secondPreCheck(3 * (row / 3) + (i % 3), 3 * (col / 3) + (i / 3));
      }
    }
  }
/**
 * Checks the rows, columns and subgrids to see if a certain number is the only
 * viable solution to the given sector
 *
 * @param row The row of the block to be inspected
 * @param col The col of the block to be inspected
 */
  private static void secondPreCheck(int row, int col) {
    if (!(oneSol(noOptions[row][col]) == 1)) {
      for (int i = 0; i < 9; i++) {
        if (noOptions[row][col][i]) {
          if (onlySol(row, col, i + 1)) {
            for (int j = 0; j < 9; j++) {
              if (noOptions[row][col][j] && !(i == j)) {
                noOptions[row][col][j] = false;
                secondPreFind(row, col, j + 1);
              }
            }
            explosionCheck(row, col, i + 1);
            break;
          }
        }
        
      }
    }
  }
}
