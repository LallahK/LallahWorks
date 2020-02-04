import java.io.File;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Scanner;
/** PMX this is the PMX class which performs the PMX algorithm.
 * @author Roscoe
 * 
 */
public class PMX {
  
/** This is the pmx method which returns the child array.
 * @param c the lower random bound
 * @param d the upper random bound
 * @param aParent1 is the first parent
 * @param aParent2 is the second parent
 * @throws ArrayIndexOutOfBoundsException throws an error for bad input!
 * @return the child array
 * 
 */
  public static int[] pmx(int c, int d, int[] aParent1, int[] aParent2) {
    int[] aChild = new int[9];
    int[] aNotCopied = new int[9];
    int counter = 0;
    int a = c - 1;
    int b = d - 1;
    for (int i = a; i <= b; i++) {
      aChild[i] = aParent1[i];
    }

    for (int i = a; i <= b; i++) {
      for (int j = a; j  <= b; j++) {
        if (aParent1[j] == aParent2[i]) {
          break;
        }

        if (j == b) {
          aNotCopied[counter] = aParent2[i];
          counter++;
        }
      }
    }
   
    for (int i = 0; i < counter; i++) {
      int val = aNotCopied[i];
      int iVal = aNotCopied[i];
      boolean flag = true;

      while (flag) {
        int v = aParent1[index(aParent2, val)];
        int lookupIndex = index(aParent2, v);

        if (aChild[lookupIndex] == 0) {
          aChild[lookupIndex] = iVal;
          flag = false;
        } else {
          val = v;
        }
      }
    }
   
    for (int i = 0; i < 9; i++) {
      if (aChild[i] == 0) {
        aChild[i] = aParent2[i];
      }
    }
  
    return aChild;

  }

/** 
 * Finds the index of some number
 *
 * @param arr The array being inspected
 * @param val Value to be found
 * @return Returns the index of the value
 */
  private static int index(int[] arr, int val) {
    int index = 0;
    for (int i = 0; i < 9; i++) {
      if (arr[i] == val) {
        return index;
      }
    }
    return index;
  }

    /** This is the main method that returns void.
     * @param args is the general argument  for public static void
     * 
     */
     public static void main(String[] args) {
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
        int a = Integer.parseInt(lines.get(0).substring(0, 1));
        int b = Integer.parseInt(lines.get(0).substring(2, 3));
        int[] p1 = new int[9];
        int[] p2 = new int[9];
        for (int i = 0; i < 9; i++) {
            p1[i] = Integer.parseInt(lines.get(1).
            substring(2 * i, 2 * i + 1));
            p2[i] = Integer.parseInt(lines.get(2).
            substring(2 * i, 2 * i + 1));
        }
        scan.close();

        int[] pmx = pmx(a, b, p1, p2);
        String line = "";
        for (int i = 0; i < 9; i++) {
           line = line + pmx[i] + " ";
        }
         System.out.println(line);
   }

/**
 *
 *
 *
 */
  public static Egg PMXMutate(Egg chosenEgg, Egg bestEgg) {
    int[][] newEgg = new int[9][9];
    //double x = Math.random();
    //int a = (int) ((1 / 6) * (Math.pow(x, 3)) - 
                   //(1 / 4) * (Math.pow(x, 2)) + 7.08 * x); 
    int a = (int) (Math.random() * 8) + 1;
    int b = (int) (Math.random() * (7 - a)) + a + 1;
    for (int i = 0; i < 9; i++) {
      newEgg[i] = pmx(a, b, chosenEgg.getRow(i), bestEgg.getRow(i));
    }
    return (new Egg(newEgg));
  }

}
