import java.util.ArrayList;
import java.util.Scanner;
import java.io.File;

/**
 * This class holds the function that mutates the given board according
 * to a given probability
 *
 */
public class FGM {

/**
 * This main function tests the functionality of the FGM
 *
 * @param args The arguments to be read
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

    boolean[][][] prefiltered = Prefiltering.prefilter(sudoku);
    RandomEggGenerator.genRandomBoard(prefiltered);
    Egg randyEgg = RandomEggGenerator.getRandomBoard();
    randyEgg.print();
   
    System.out.println("---------");
     
    randyEgg = feasibleGeometricMutation(prefiltered, randyEgg);
    randyEgg.print();
  }
/** 
 * Mutates the egg with a given probability
 *
 * @param optionBoard The domain defining which values can go where
 * @param egg The egg to be mutated
 * @return Returns the mutated egg
 */
  public static Egg feasibleGeometricMutation(boolean[][][] optionBoard,
                                      Egg egg) {
    int[][] mutation = egg.getBoard();
    
    for (int m = 0; m < 9; m++) {
      if (Math.random() < (0.6)) {
        ArrayList<Integer> cols = new ArrayList<>();
        for (int i = 0; i < 9; i++) {
          int count = 0;
          for (int k = 0; k < 9; k++) {
            if (optionBoard[m][i][k]) {
              count++;
            }
          }
          if (count > 1) {
            cols.add(i);
          }
        }
        if (cols.size() > 0) {
          int x = (int) (Math.random() * cols.size());
          int col1 = cols.get(x);
          cols.remove(x);
          x = (int) (Math.random() * cols.size());
          int col2  = cols.get(x);
      
          int[] eggRow = mutation[m];
          x = eggRow[col1];
          eggRow[col1] = eggRow[col2];
          eggRow[col2] = x;
          mutation[m] = eggRow;
        }
      }
    }
    
    egg = new Egg(mutation);
    return (egg);
  }

}
