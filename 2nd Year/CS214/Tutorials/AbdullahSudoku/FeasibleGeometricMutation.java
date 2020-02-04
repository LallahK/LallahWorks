import java.io.File;
import java.lang.Math;
import java.util.*;

public class FeasibleGeometricMutation {
  private static int[][] sudoku = new int[9][9];

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

  public static Integer[][] FGM(Integer initSol[][], Integer sol[][]) {
    int rows = getRandomNumberInRange(0, initSol.length);
    Integer[] randomRow = initSol[rows];
    int pos1 = 0;
    int pos2 = 0;

    for (int row : randomRow) {
      while (pos1 == pos2) {
        pos1 = findEmptyCells(row, randomRow);
        pos2 = findEmptyCells(row, randomRow);
      }
      int aux = sol[row][pos2];
      sol[row][pos1] = sol[row][pos2];
      sol[row][pos2] = aux;
      System.out.println(pos1);
    }
    return sol;

  }

  private static int getRandomNumberInRange(int min, int max) {

    if (min >= max) {
      throw new IllegalArgumentException("max must be greater than min");
    }

    Random r = new Random();
    return r.nextInt((max - min) + 1) + min;
  }

  public static int findEmptyCells(int randomRow, Integer[] row) {
    int pos = 0;
    for (int i = 0; i < row.length; i++) {
      if (Integer.toString(row[i]) == null) {
        pos = i;
        break;
      }
    }
    return pos;
  }
}
