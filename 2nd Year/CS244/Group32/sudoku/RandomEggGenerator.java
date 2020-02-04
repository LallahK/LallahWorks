import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;

/**
 * Given a specefic prefiltering constraint array, builds a random board
 * 
 * @author Abdullah Karbanee 21865728
 */
public class RandomEggGenerator {
  int[][][] ranRows;
/**
 * This main function tests if the two random egg generator functions work
 *
 * @param args The name of the file to be searched
 */
  public static void main(String[] args) {
    Scanner scan;
    int[][] sudoku = new int[9][9];
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
    genRandomBoard(Prefiltering.prefilter(sudoku));
    Egg randy = getRandomBoard();
    randy.print();
    double d = System.currentTimeMillis();
    for (int i = 0; i < 10000; i++) {
      randy = getRandomBoard();
    }
    System.out.println((System.currentTimeMillis() - d) / 10000000);
  }

/**
 * This function will be used to generate random rows, abiding by the 
 * prefiltering restrictions, for the egg
 *
 * @return Returns the random egg board
 */ 
  public static Egg getRandomBoard() {
    int[][] board = new int[9][9];
    for (int i = 0; i < 9; i++) {
      int x = (int) (Math.random() * randomRows.get(i).size());
      int[] ranRow = randomRows.get(i).get(x);
      for (int j = 0; j < 9; j++) {
        board[i][j] = ranRow[j];
      }
    }
    return (new Egg(board));
  }

  private static ArrayList<ArrayList<int[]>> randomRows = 
                           new ArrayList<ArrayList<int[]>>();
/**
 * Generates all the possible rows for the prefiltered board
 *
 * @param optionBoard The 3D boolean array of prefiltered options
 */
  public static void genRandomBoard(boolean[][][] optionBoard) {
    for (int j = 0; j < 9; j++) {
      boolean[][] rowOptions = new boolean[9][9];
      randomRows.add(new ArrayList<int[]>());
      for (int i = 0; i < 9; i++) {
        for (int k = 0; k < 9; k++) {
          rowOptions[k][i] = optionBoard[j][i][k];
        }
      }
      genRandomRows(rowOptions);
    }
  }

/**
 * Begins the recursive call to generate the rows
 *
 * @param rowOptions The 2D boolean array of options for a row
 */
  private static void genRandomRows(boolean[][] rowOptions) {
    recursiveRows(rowOptions, new int[]{0, 0, 0, 0, 0, 0, 0, 0, 0}, 0); 
  }

/**
 * Recursively generates the rows, using a trie-esque algorithm
 *
 * @param rowOptions The 2D boolean array of options for a row
 * @param row The row that has been generated so far
 * @param count The amount of numbers inputted into the row
 */
  private static void recursiveRows(boolean[][] rowOptions, 
    int[] row, int count) {
    boolean change = false;
    if (count == 9) {
      randomRows.get(randomRows.size() - 1).add(row);
      return;
    } 
  
    for (int i = 0; i < 9; i++) {
      int ops = 0;
      for (int k = 0; k < 9; k++) {
        if (rowOptions[k][i]) {
          ops++;
        }
      }
      if (ops == 1) {
        boolean[][] newOps = new boolean[9][9];
        for (int a = 0; a < 9; a++) {
          for (int b = 0; b < 9; b++) {
            newOps[a][b] = rowOptions[a][b];
          }
        }
      
        int[] newRow = new int[9];
        int c = 0;
        for (int k = 0; k < 9; k++) {
          newRow[k] = row[k];
          newOps[k][i] = false;
          if (rowOptions[k][i]) {
            c = (k + 1);
          }
        }
        for (int k = 0; k < 9; k++) {
          newOps[k][i] = false;
          newOps[c - 1][k] = false;
        }
        newRow[i] = c;
        recursiveRows(newOps, newRow, count + 1);
        change = true;
        return;
      }
    }
    if (!change) {
      ArrayList<Integer> mins = new ArrayList<Integer>();
      int min = 10;
      for (int k = 0; k < 9; k++) {
        int rOps = 0;
        for (int i = 0; i < 9; i++) {
          if (rowOptions[k][i]) {
            rOps++;
          }
        } 
        if (rOps <= min && rOps > 0) {
          if (rOps == min) {
            mins.add(k);
          } else {
            min = rOps;
            mins.clear();
            mins.add(k);
          }
        }
      }
      
      for (int i = 0; i < mins.size(); i++) {
        int minN = mins.get(i);
        
        for (int m = 0; m < 9; m++) {
          boolean[][] newOps = new boolean[9][9];
          for (int a = 0; a < 9; a++) {
            for (int b = 0; b < 9; b++) {
              newOps[a][b] = rowOptions[a][b];
            }
          }
          for (int k = 0; k < 9; k++) {
            newOps[minN][k] = false;
          }
          int[] newRow = new int[9];
          for (int n = 0; n < 9; n++) {
            newRow[n] = row[n];
          }
          if (rowOptions[minN][m]) {
            newRow[m] = minN + 1;
            for (int n = 0; n < 9; n++) {
              newOps[n][m] = false;
            }
            recursiveRows(newOps, newRow, count + 1);
          }
        }
      }
    }
  }
}
