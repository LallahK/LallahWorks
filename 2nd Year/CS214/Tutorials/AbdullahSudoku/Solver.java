import java.io.File;
import java.util.Random;
import java.util.Scanner;
import java.util.Arrays;

/**
 * 
 */

/**
 * @author ahmfar
 *
 */
public class Solver {
  private static boolean[][][] noOptions = new boolean[9][9][9];
  private static int[][] sudoku = new int[9][9];

  /**
   * @param args
   */
  public static void main(String[] args) {
    // TODO Auto-generated method stub

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
  }


  public static Integer[][] PrefilteredDCS(int NestSize, float P_multi, float P_mutate) {
    noOptions = Prefiltering.prefilter(sudoku);
    Integer[][] eggBest = new Integer[9][9];
    Integer[][] egglastBest = new Integer[9][9];
    Integer[][][] Nests = GenerateInitialPopulation(NestSize, noOptions);
    int cost = Utils.cost(sudoku);
    while (cost > 0) {
      Random rand = new Random();
      int i = rand.nextInt(NestSize);
      Integer[][] EggA = Nests[i];
      EggA = GeometricOperators.GeometricOperator(eggBest, EggA, egglastBest, Nests, noOptions, P_multi, P_mutate);
    }



    return null;

  }

  public static Integer[][][] GenerateInitialPopulation(int nestSize, boolean[][][] noOption) {
    Integer[][][] Nest = new Integer[nestSize][9][9];
    for (int i = 0; i <= nestSize; i++) {
      Nest[i] = RandomEggGenerator.randomBoard(noOption);
    }
    return Nest;
  }
}
