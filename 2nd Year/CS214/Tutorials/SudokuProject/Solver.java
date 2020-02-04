import java.util.Scanner;
import java.io.File;

public class Solver {
  private static int lastBestEgg = 0;
  private static Egg bestEgg;
  private static int nestSize = 10;
  private static Egg[] nest = new Egg[nestSize];
  private static boolean[][][] prefiltered;

/** 
 * The main function used to test the solver
 *
 * @param args takes in the name of the file
 */   
  public static void main(String[] args) {
    int[][] sudoku = new int[9][9];
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
   
    double s = System.currentTimeMillis();
    //for (int i = 0; i < 200; i++) { 
      solve(sudoku);
    //}
    //s = System.currentTimeMillis() - s;
    //System.out.println(((s) / 1000) / 200);
    bestEgg.print();
 }

/**
 * The Prefiltered discrete cuckoo search algorithm
 *
 * @param initialBoard the intiial board with the blank blocks
 * @return Returns The solved solution in the form of an egg
 */  
  public static Egg solve(int[][] initialBoard) {    
    prefiltered = Prefiltering.prefilter(initialBoard);
    RandomEggGenerator.genRandomBoard(prefiltered);
    for (int i = 0; i < nestSize; i++) {
      nest[i] = RandomEggGenerator.getRandomBoard();
    }
    bestEgg = nest[0];
    findBestEgg();


    double start = System.currentTimeMillis();
    int iterations = 0;
    while (!(bestEgg.getCost() == 0) && (System.currentTimeMillis() - start) < 5000) {
      int ran = 0;
      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      Egg child = geometricOperators(nest[ran], initialBoard, 0.5, 0.7);
      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      if (child.compareTo(nest[ran]) < 0) {
        nest[ran] = child;
      }
  
      findBestEgg();
      abandonWorstEggs(0.3);
      iterations++;
      if (iterations > 8000000) {
        System.out.println("MAX ITER EXCEEDED");
        System.exit(0);
      }
      iterations++;
    }
    System.out.println(iterations);
    System.out.println((System.currentTimeMillis() - start) / (1000));
    return bestEgg;
  }
  
/**
 * Finds the best egg that lies in the nest
 *
 */
  private static void findBestEgg() {
    int best = 0;
    for (int i = 0; i < nestSize; i++) {
      if (nest[best].compareTo(nest[i]) > 0) {
        best = i;
      }
    }
    lastBestEgg = best;
    if (bestEgg.getCost() >= nest[lastBestEgg].getCost()) {
      bestEgg = nest[lastBestEgg];
    }
  }
 
/** 
 * Abandons all eggs except for the best egg
 * 
 * @param pAbandon The probability of abandoning an egg
 */ 
  private static void abandonWorstEggs(double pAbandon) {
    for (int i = 0; i < nestSize; i++) {
      if (!(i == lastBestEgg) && Math.random() < pAbandon) {
        nest[i] = RandomEggGenerator.getRandomBoard();
      }
    }
  }

  /*private static void abandonWorstEggs2(double pAbandon) {
    int[][] tempBestBoard = bestEgg.getBoard();
    
    for (int i = 0; i < nestSize; i++) {
      if (!(i == lastBestEgg) && Math.random() < pAbandon) {
        int[][] bestBoard = new int[9][9];
        for (int j = 0; j < 9; j++) {
          for (int k = 0; k < 9; k++) {
            bestBoard[j][k] = tempBestBoard[j][k];
          }
        }
        Egg bEgg = new Egg(bestBoard);
        nest[i] = FGM.FeasibleGeometricMutation(prefiltered, bEgg);
      }
    }
  }*/
  
/**
 * This function mutates a given egg (The check on whether the egg is the best
 * egg is done before calling this function)
 * 
 * @param chosenEgg The egg to be mutated
 * @param pPmxMulti The probability of either calling PMX or MPMX
 * @param pMutate The probability of mutating the given egg
 * @return Returns the mutated egg
 */
  private static Egg geometricOperators(Egg chosenEgg, int[][] initSol, 
                                        double pPmxMulti, double pMutate) {
    Egg mutatedEgg;
    if (Math.random() < pPmxMulti) {
      mutatedEgg = PMX.PMXMutate(chosenEgg, nest[lastBestEgg]);
    } else {
      mutatedEgg = MPSX.MPSXMutate(bestEgg, nest[lastBestEgg], chosenEgg);
    } 
    if (Math.random() < pMutate) {
      mutatedEgg = FGM.FeasibleGeometricMutation(initSol, mutatedEgg); 
    }
    return chosenEgg;
  }
  
}
