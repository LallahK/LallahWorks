import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;

/**
 * Given a specefic prefiltering constraint array, builds a random board
 * 
 * @author Abdullah Karbanee 21865728
 */
public class RandomEggGenerator {
 
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
     
    int[][] randy = randomBoard(Prefiltering.prefilter(sudoku)); 
    
    for (int i = 0; i < 9; i++) {
      String line = "";
      for (int k = 0; k < 9; k++) {
        line = line + randy[i][k];
      }
      System.out.println(line);
    }
  }

/**
 * This function will be used to generate random rows, abiding by the 
 * prefiltering restrictions, for the egg
 *
 * @param optionBoard The board containing which numbers can be inputted where
 * @return Returns the random egg board
 */
  public static int[][] randomBoard(boolean[][][] optionBoard) {
    int[][] ranBoard = new int[9][9];
    
    for (int i = 0; i < 9; i++) {
      boolean[][] optionRow = new boolean[9][9];
      for (int k = 0; k < 9; k++) {
        for (int j = 0; j < 9; j++) {
          optionRow[k][j] = optionBoard[i][j][k];
        }
      }
      ranBoard[i] = genRandomRow(optionRow);
    }
    
    return ranBoard;
  }

/**
 * This function takes a 2D array of options for a row and returns a 
 * randomly generated row generated from the option board
 *
 * @param optionBoard the options for a row represented ass a 2D array
 * @return Returns a randomly generated row
 */
 private static int[] genRandomRow(boolean[][] optionBoard) {
    int[] ranRow = new int[9];
    ArrayList<ArrayList<Integer>> options = new ArrayList<ArrayList<Integer>>();
    
    for (int i = 0; i < 9; i++) {
      options.add(new ArrayList<Integer>());
      for (int k = 0; k < 9; k++) {
        if (optionBoard[i][k]) {
          options.get(i).add(k);
        }
      }
    }
    
    for (int i = 0; i < 9; i++) {
      boolean change = false;
      for (int k = 0; k < 9; k++) {
        int indexCount = 0;
        for (int j = 0; j < 9; j++) {
          if (options.get(j).contains(k)) {
            indexCount++;
          }
        }
        if (indexCount == 1) {
          int j;
          for (j = 0; j < 9; j++) {
            if (options.get(j).contains(k)) {
              break;
            }
          }
          ranRow[options.get(j).get(options.get(j).indexOf(k))] = (j + 1);
          options.get(j).clear();
          change = true;
          break;
        }
      }
      
      if (!change) {
        int min = 10;
        int minIndex = 0;
        for (int k = 0; k < 9; k++) {
          if (options.get(k).size() < min && !(options.get(k).size() == 0)) {
            min = options.get(k).size();
            minIndex = k; 
          }
        }
        int x = (int) (Math.random() * options.get(minIndex).size());
        x = options.get(minIndex).get(x);
        ranRow[x] = (minIndex + 1);
        for (int k = 0; k < 9; k++) {
          if (options.get(k).contains(x)) {
            options.get(k).remove(options.get(k).indexOf(x));
          }
        }
        options.get(minIndex).clear();
      }
    }
    return ranRow;
  }
}
