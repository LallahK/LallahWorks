import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;

public class MPSX {  
  private static int[] mask = new int[9];
  private static int[][] parent = new int[3][9];  
  private static int[] child = new int[9];
 
/**
 * This function is the main function that is called from Sudoku class
 * 
 * @param args This contains the name of the file to be scanned
 * @Exception e this is thrown when the file is not found
 */ 
  public static void main(String[] args) {
    int[] m = new int[9];
    int[] a1 = new int[9];
    int[] a2 = new int[9];
    int[] a3 = new int[9];  

	Scanner scan;
    try {
      scan = new Scanner(new File(args[0]));
    } catch (Exception e) {
      System.out.println("No file found");
      return;
    }

	ArrayList<String> lines = new ArrayList<String>();
    while (scan.hasNextLine()) {
      lines.add(scan.nextLine());
    }
    
    for (int i = 0; i < 9; i++) {    
      m[i] = Integer.parseInt(lines.get(0).substring(2 * i, 2 * i + 1));
      a1[i] = Integer.parseInt(lines.get(1).substring(2 * i, 2 * i + 1));
      a2[i] = Integer.parseInt(lines.get(2).substring(2 * i, 2 * i + 1));
      a3[i] = Integer.parseInt(lines.get(3).substring(2 * i, 2 * i + 1));
    }
    int[] child = mpsx(m, a1, a2, a3);

    String line = "";
    for (int i = 0; i < 9; i++)
      line += child[i] + " ";
    System.out.println(line);
  }

/**
 * This is the function that actually generates the child from the 
 * multiple parents
 * @param m The mask for mpsx
 * @param a1 the first parent
 * @param a2 the second parent
 * @param a3 the third parent
 * @return Returns the child of the mpsx function
 */
  private static int[] mpsx(int[] m, int[] a1, int[] a2, int[] a3) {
    for (int i = 0; i < 9; i++) {
      mask[i] = m[i];
      parent[0][i] = a1[i];
      parent[1][i] = a2[i];
      parent[2][i] = a3[i];
    }
    
    int[] child = new int[9];

    for (int i = 0; i < 9; i++) {
      child[i] = parent[mask[i] - 1][i];
      swap(parent[0], i, findIndex(parent[0], child[i]));
      swap(parent[1], i, findIndex(parent[1], child[i]));
      swap(parent[2], i, findIndex(parent[2], child[i]));      
    }        

    return child;
  }

/**
 * Ths function simply swaps the values in an array
 * 
 * @param arr This is the array in which values will be switched
 * @param i1 this is the index of the first value
 * @param i2 this is the index of the second value
 */
  private static void swap(int[] arr, int i1, int i2) {
    int temp = arr[i1];
    arr[i1] = arr[i2];
    arr[i2] = temp;
  }

/**
 * This function finds the index of a value in an array
 *
 * @param arr the array in which the value is being searched
 * @param val the value that is being searched for
 * @return the integer that is being returned
 */
  private static int findIndex(int[] arr, int val) {
    int index = 0;
	for (int i = 0; i < 9; i++) {
      if (arr[i] == val) {
        index = i;
        break;
      }
    }
    return index;
  }
}  
