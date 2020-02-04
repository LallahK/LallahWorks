import java.util.Scanner;
import java.io.File;

/**
 * This is where a new board is sent, after which the cuckoo search algorithm
 * is used to try and solve the board
 *
 * @author Farhaan Ahmed, Abdullah Karbanee, Roscoe Kerby
 */
public class Solver {
  private static int lastBestEgg = 0;
  private static Egg bestEgg;
  
  private static int nestSize = 22;
  private static double pAbandon = 0.3;
  private static double pPMXMPSX = 0.65;
  private static double pFGM = 0.9;
  
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
   
    /*double s = System.currentTimeMillis();
    for (int i = 0; i < 3; i++) { 
      solve(sudoku);
    }
    s = System.currentTimeMillis() - s;
    System.out.println(((s) / 1000) / 3);*/
    paramTest(sudoku);
 }
  
/**
 * Function to test the parameter; Allows the user to systematically
 * change the value of the parameter to try and optimise them
 *
 * @param sudoku The board that will tested
 */
  private static void paramTest(int[][] sudoku) {
    for (int s = 4; s <= 20; s++) {
      nestSize = s;
      double t = System.currentTimeMillis();
      for (int i = 0; i < 150; i++) {
        solve(sudoku);
      }
      t = System.currentTimeMillis() - t;
      System.out.println("nest size: " + nestSize 
               + " : " + (((t) / 1000) / 150));
    }
  }

/**
 * The Prefiltered discrete cuckoo search algorithm; as implemented
 * in the article using an extra layer of prefiltering
 *
 * @param initialBoard the intiial board with the blank blocks
 * @return Returns The solved solution in the form of an egg
 */ 
  public static Egg solveV2(int[][] initialBoard) {
    prefiltered = Prefiltering.prefilterV2(initialBoard);
    RandomEggGenerator.genRandomBoard(prefiltered);
    for (int i = 0; i < nestSize; i++) {
      nest[i] = RandomEggGenerator.getRandomBoard();
    }
    bestEgg = nest[0];
    findBestEgg();

    int bestCount = 0;
    int iterations = 0;
    double start = System.currentTimeMillis();
    while (!(bestEgg.getCost() == 0)
       && (System.currentTimeMillis() - start) < 14925) {
      int ran = 0;

      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      Egg child = geometricOperators(nest[ran], pPMXMPSX, pFGM);
      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      if (child.compareTo(nest[ran]) < 0) {
        nest[ran] = child;
      }

      int oldBest = bestEgg.getCost();
      findBestEgg();
      if (oldBest == bestEgg.getCost()) {
        bestCount++;
      } else {
        bestCount = 0;
      }
      if (bestCount > Math.pow(3, -1 * oldBest + 2) * 400000) {
        bestCount = 0;
        abandonWorstEggs(0.8);
      }

      abandonWorstEggs(pAbandon);
      iterations++;
    }
    return bestEgg;
  }

/**
 * The Prefiltered discrete cuckoo search algorithm; as implemented
 * in the article 
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

    int bestCount = 0;
    int iterations = 0;
    double start = System.currentTimeMillis();
    while (!(bestEgg.getCost() == 0) 
       && (System.currentTimeMillis() - start) < 14925) {
      int ran = 0;
      
      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      Egg child = geometricOperators(nest[ran], pPMXMPSX, pFGM);
      while (ran != lastBestEgg) {
        ran = (int) (Math.random() * nestSize);
      }
      if (child.compareTo(nest[ran]) < 0) {
        nest[ran] = child;
      }
      
      int oldBest = bestEgg.getCost();
      findBestEgg();
      if (oldBest == bestEgg.getCost()) {
        bestCount++;
      } else {
        bestCount = 0;
      }
      if (bestCount > Math.pow(3, -1 * oldBest + 2) * 400000) {
        bestCount = 0;
        abandonWorstEggs(0.8);
      }

      abandonWorstEggs(pAbandon);
      iterations++;
    }
    return bestEgg;
  }
  
/**
 * Finds the best egg that lies in the nest as well as saving as an index
 * the best solution to the previous generation
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
 * Abandons all eggs except for the best egg according to a certain
 * probability
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

/**
 * A possible alternative to making the replacement for abandoned eggs
 * random, rather using the best egg to generate them
 *
 * @param pAbandon The probability of abandoning a random Egg
 */
  private static void abandonWorstEggs2(double pAbandon) {
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
        //bEgg.print();
        //System.out.println("---------");
        bEgg = FGM.feasibleGeometricMutation(prefiltered, bEgg);
        //bEgg.print();
        //System.out.println("=========");
        nest[i] = bEgg;
      }
    }
  }
  
/**
 * This function mutates a given egg (The check on whether the egg is the best
 * egg is done before calling this function)
 * 
 * @param chosenEgg The egg to be mutated
 * @param pPmxMulti The probability of either calling PMX or MPMX
 * @param pMutate The probability of mutating the given egg
 * @return Returns the mutated egg
 */
  private static Egg geometricOperators(Egg chosenEgg, 
                                        double pPmxMulti, double pMutate) {
    Egg mutatedEgg;
    if (Math.random() < pPmxMulti) {
      mutatedEgg = PMX.pMXMutate(chosenEgg, nest[lastBestEgg]);
    } else {
      mutatedEgg = MPSX.mPSXMutate(bestEgg, nest[lastBestEgg], chosenEgg);
    }
    if (Math.random() < pMutate) {
      mutatedEgg = FGM.feasibleGeometricMutation(prefiltered, mutatedEgg); 
    }
    return mutatedEgg;
  }
  
}

