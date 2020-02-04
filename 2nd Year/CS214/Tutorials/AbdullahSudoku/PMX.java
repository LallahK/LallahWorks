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
   public static Integer[]  pmx(int c, int d,
      Integer[] aParent1, Integer[] aParent2)
       throws ArrayIndexOutOfBoundsException {

         if ((aParent1.length != 9) || (aParent2.length != 9)) {
            throw new ArrayIndexOutOfBoundsException();
        }

        Integer[] aChild = new Integer[9];
        Integer[] aNotCopied = new Integer[9];
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
              int v = aParent1[Arrays.asList(aParent2).
              indexOf(val)];

              int lookupIndex = 
                Arrays.asList(aParent2).indexOf(v);

              if (aChild[lookupIndex] == null) {
                aChild[lookupIndex] = iVal;
                flag = false;
                break;
                } else {
                val = v;
                }
                }
                }

         for (int i = 0; i < 9; i++) {
           if (aChild[i] == null) {
             aChild[i] = aParent2[i];
           }


           }

           return aChild;


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
        Integer[] p1 = new Integer[9];
        Integer[] p2 = new Integer[9];
        for (int i = 0; i < 9; i++) {
            p1[i] = Integer.parseInt(lines.get(1).
            substring(2 * i, 2 * i + 1));
            p2[i] = Integer.parseInt(lines.get(2).
            substring(2 * i, 2 * i + 1));
        }
        scan.close();

        Integer[] pmx = pmx(a, b, p1, p2);
        String line = "";
        for (int i = 0; i < 9; i++) {
           line = line + pmx[i] + " ";
        }
         System.out.println(line);
   }


}
