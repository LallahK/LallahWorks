
/**
 * This class is used to calculate the cost of Eggs
 *
 * @author Roscoe Kerby
 */
public class Utils {

/**
 * This main function tests the functionality of the Utils function
 *
 * @param args This is the arguments to be sent to the utils class
 */
  public static void main(String[] args) {
    int[][] arr = {{5, 3, 4, 6, 7, 8, 9, 1, 2 }, 
                   {6, 7, 2, 1, 9, 5, 3, 4, 8 }, 
                   {1, 9, 8, 3, 4, 2, 5, 6, 7 },
                   {8, 5, 9, 7, 6, 1, 4, 2, 3 }, 
                   {4, 2, 6, 8, 5, 3, 7, 9, 1 }, 
                   {7, 1, 3, 9, 2, 4, 8, 5, 6 },
                   {9, 6, 1, 5, 3, 7, 2, 8, 4 }, 
                   {2, 8, 7, 4, 1, 9, 6, 3, 5 }, 
                   {6, 6, 5, 2, 8, 6, 1, 7, 9 }};
    System.out.println(cost(arr));
  }
  
/**
 * This actually calculates the cost of some board 
 *
 * @param board The board who's cost will be calculated
 * @return An integer that is being returned
 */
  public static int cost(int[][] board) {
    int cost = 0;
    for (int i = 0; i < 9; i++) {
      boolean[] rowCheck = new boolean[9];
      boolean[] colCheck = new boolean[9];
      boolean[] gridCheck = new boolean[9];
      
      for (int k = 0; k < 9; k++) {
        //Checks the cost for the rows
        if (!rowCheck[board[i][k] - 1]) {
          rowCheck[board[i][k] - 1] = true;
        } else {
          cost++;
        }
        
        //Checks the cost for the columns
        if (!colCheck[board[k][i] - 1]) {
          colCheck[board[k][i] - 1] = true;
        } else { 
          cost++;
        }
        
        //Checks the cost of the subgrids
        if (!gridCheck[board[(i / 3) * 3 + k / 3][(i % 3) * 3 + k % 3] - 1]) {
          gridCheck[board[(i / 3) * 3 + k / 3][(i % 3) * 3 + k % 3] - 1] = true;
        } else {
          cost++;
        }
      }
    }
    return cost;
  }
}
