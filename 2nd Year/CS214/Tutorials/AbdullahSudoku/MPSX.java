import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.Random;
import java.util.Arrays;

/**
 * this class implements the Multiparental Crossover as described in A Prefiltered Cuckoo Search
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
    Integer[] p1 = new Integer[9];
    Integer[] p2 = new Integer[9];
    Integer[] p3 = new Integer[9];
    Integer[] mask = new Integer[9];
    for (int i = 0; i < 9; i++) {
      mask[i] = Integer.parseInt(lines.get(0).substring(2 * i, 2 * i + 1));
      p1[i] = Integer.parseInt(lines.get(1).substring(2 * i, 2 * i + 1));
      p2[i] = Integer.parseInt(lines.get(2).substring(2 * i, 2 * i + 1));
      p3[i] = Integer.parseInt(lines.get(3).substring(2 * i, 2 * i + 1));
    }
    scan.close();

    Integer[] child = mPMX(p1, p2, p3, mask);
    String line = "";
    for (int i = 0; i < 9; i++) {
      line = line + child[i] + " ";
    }
    System.out.println(line);
  }

  /**
   * This is the Multiparental Crossover algorithm. It uses a mask, which consists of numbers 1,2,3
   * in random order. This how the algorithm essentially works: We loop through the mask If the mask
   * is equal to one, then we assign the value at index of the Parent one to the same index of the
   * child. We find the position of the that same value in parents two and three We then swap them
   * with the value (at the same index) of parents two and three The process is repeated for mask
   * values two and three
   * 
   * @param parent1 this is the first parent
   * @param parent2 this is the second parent
   * @param parent3 this is the third parent
   * @param mask this is a random string consisting of 1s,2s and 3s.
   * @return the child "born" from the function
   */
  public static Integer[] mPMX(Integer[] parent1, Integer[] parent2, Integer[] parent3,
      Integer[] mask) {
    Integer[] child = new Integer[9];
    for (int i = 0; i < mask.length; i++) {
      if (mask[i] == 1) {
        child[i] = parent1[i];
        int pos1 = findIndex(parent2, parent1[i]);
        swap(i, pos1, parent2);
        int pos2 = findIndex(parent3, parent1[i]);
        swap(i, pos2, parent3);
      } else if (mask[i] == 2) {
        child[i] = parent2[i];
        int pos1 = findIndex(parent1, parent2[i]);
        swap(i, pos1, parent1);
        int pos2 = findIndex(parent3, parent2[i]);
        swap(i, pos2, parent3);
      } else {
        child[i] = parent3[i];
        int pos1 = findIndex(parent1, parent3[i]);
        swap(i, pos1, parent1);
        int pos2 = findIndex(parent2, parent3[i]);
        swap(i, pos2, parent2);
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
  public static void swap(int x, int y, Integer[] parent) {
    int aux = parent[x];
    parent[x] = parent[y];
    parent[y] = aux;
  }

  /**
   * this function finds the index of an element in an array
   * 
   * @param a this the array that we wish to search in
   * @param key this is the item whose position we are looking for
   * @return the position of the key if successful, else 0 (for debugging purposes)
   */
  public static int findIndex(Integer[] a, int key) {
    int pos = 0;
    for (int i = 0; i < a.length; i++) {

      if (a[i] == key) {
        pos = i;
        break;
      } else {
        pos = 0;
      }
    }
    return pos;
  }

  public static Integer[] generateMask() {
    Integer[] mask = new Integer[9];
    float w1 = 0, w2 = 0, w3 = 0;
    if (w1 + w2 + w3 != 1) {
      for (int i = 0; i < 9; i++) {
        Random rand = new Random();
        mask[i] = rand.nextInt(3) + 1;
      }
      w1 = (count(mask, 1)) / 9;
      w2 = (count(mask, 2)) / 9;
      w3 = (count(mask, 3)) / 9;
    }
    return mask;
  }

  public static int count(Integer[] array, int key) {
    int counter = 0;
    for (int i = 0; i < array.length; i++) {
      if (array[i] == key) {
        counter++;
      }
    }
    return counter;
  }

}
