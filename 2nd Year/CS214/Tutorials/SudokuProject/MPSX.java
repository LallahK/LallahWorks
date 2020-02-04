import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Arrays;

/**
 * this class implements the Multiparental Crossover
 * as described in A Prefiltered Cuckoo Search
 * Algorithm with Geometric Operators for Solving Sudoku Problems.
 * 
 * @author ahmfar
 *
 */
public class MPSX {
  /**
   * The function of this class is to implement the MPSX algorithm
   * 
   * @param args main method
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

    ArrayList<String> lines = new ArrayList<String>();
    while (scan.hasNextLine()) {
      lines.add(scan.nextLine());
    }
    int[] p1 = new int[9];
    int[] p2 = new int[9];
    int[] p3 = new int[9];
    int[] mask = new int[9];
    for (int i = 0; i < 9; i++) {
      mask[i] = Integer.parseInt(lines.get(0).substring(2 * i, 2 * i + 1));
      p1[i] = Integer.parseInt(lines.get(1).substring(2 * i, 2 * i + 1));
      p2[i] = Integer.parseInt(lines.get(2).substring(2 * i, 2 * i + 1));
      p3[i] = Integer.parseInt(lines.get(3).substring(2 * i, 2 * i + 1));
    }
    scan.close();

    int[] child = mPMX(p1, p2, p3, mask);
    String line = "";
    for (int i = 0; i < 9; i++) {
      line = line + child[i] + " ";
    }
    System.out.println(line);
  }

  /**
   * This is the Multiparental Crossover algorithm. 
   * It uses a mask, which consists of numbers 1,2,3
   * in random order. This how the algorithm essentially works: 
   * We loop through the mask If the mask
   * is equal to one, then we assign the value at 
   * index of the Parent one to the same index of the
   * child. We find the position of the that same 
   * value in parents two and three We then swap them
   * with the value (at the same index) of parents 
   * two and three The process is repeated for mask
   * values two and three
   * 
   * @param parent1 this is the first parent
   * @param parent2 this is the second parent
   * @param parent3 this is the third parent
   * @param mask this is a random string consisting of 
   * 1s,2s and 3s.
   * @return the child "born" from the function
   */
  public static int[] mPMX(int[] parent1, int[] parent2, 
      int[] parent3, int[] mask) {
    int[] child = new int[9];
    for (int i = 0; i < mask.length; i++) {
      switch(mask[i]) {
        case 1 : child[i] = parent1[i]; 
                 swap(i, findIndex(parent2, child[i]), parent2);
                 swap(i, findIndex(parent3, child[i]), parent3);        
          break;
        case 2 : child[i] = parent2[i];
                 swap(i, findIndex(parent1, child[i]), parent1);
                 swap(i, findIndex(parent3, child[i]), parent3);
          break;
        case 3 : child[i] = parent3[i];
                 swap(i, findIndex(parent1, child[i]), parent1);
                 swap(i, findIndex(parent2, child[i]), parent2); 
          break;
      }
    }
    return child;

  }

  /**
   * this function swaps two elements in an array
   * 
   * @param x this is the variable that will receive Y's position
   * @param y this is the variable that will receive X's position
   * @param parent this is the array where the swaps will take place
   */
  public static void swap(int x, int y, int[] parent) {
    int aux = parent[x];
    parent[x] = parent[y];
    parent[y] = aux;
  }

  /**
   * this function finds the index of an element in an array
   * 
   * @param a this the array that we wish to search in
   * @param key this is the item whose position we are looking for
   * @return the position of the key if successful,
   *  else 0 (for debugging purposes)
   */
  public static int findIndex(int[] a, int key) {
    int pos = 0;
    for (int i = 0; i < a.length; i++) {
      if (a[i] == key) {
        return pos;
      }
    }
    return pos;
  }

/**
 * This function takes in the 3 Eggs and generates the mutated egg
 *
 * @param chosenEgg The egg to be mutated
 * @param bestEgg the best egg
 * @param lastBestEgg The last best egg
 * @return Returns the mutated egg
 */
  public static Egg MPSXMutate(Egg bestEgg, Egg lastBestEgg, Egg chosenEgg) {
    int[][] newEgg = new int[9][9]; 
    
    int[] mask = new int[9];
    for (int k = 0; k < 9; k++) {
      double r = Math.random();
      if (r < (5.0 / 9)) {
        mask[k] = 1; 
      } else if (r < (8.0 / 9)) {
        mask[k] = 2;
      } else {
        mask[k] = 3;
      }
    }
    for (int i = 0; i < 9; i++) {
      newEgg[i] = mPMX(bestEgg.getRow(i), 
                       lastBestEgg.getRow(i), chosenEgg.getRow(i), mask);
    }
    return (new Egg(newEgg));
  }
}
