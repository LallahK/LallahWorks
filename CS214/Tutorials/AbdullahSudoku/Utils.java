import java.util.ArrayList;
import java.util.Arrays;
/**
 *Utils class is here for us to use to allow modular programming. 
 */
public class Utils {

/**
 * This is the main method for the Utils class. 
 * @param args is the default argument for main
 *
 */
    public static void main(String[] args) {

        int[][] sudoku = new int[9][9];
        int[][] arr = {{5, 3, 4, 6, 7, 8, 9, 1, 2 }, 
                {6, 7, 2, 1, 9, 5, 3, 4, 8 }, 
                {1, 9, 8, 3, 4, 2, 5, 6, 7 },

                {8, 5, 9, 7, 6, 1, 4, 2, 3 }, {4, 2, 6, 8, 5, 3, 7, 9, 1 }, 
                {7, 1, 3, 9, 2, 4, 8, 5, 6 },

                {9, 6, 1, 5, 3, 7, 2, 8, 4 }, {2, 8, 7, 4, 1, 9, 6, 3, 5 }, 
                {3, 4, 5, 2, 8, 6, 1, 7, 9 } };

        for (int r = 0; r < 9; r++) {
            for (int c = 0; c < 9; c++) {
                sudoku[r][c] = (int) (Math.random() * 9 + 1);

            }
            System.out.println(Arrays.toString(sudoku[r]));
        }

        // System.out.println(cost(sudoku));
        System.out.println(cost(arr));

    }
    
    /**
     * This is the cost method which computers the cost of the board 
     * (the amount of stuff wrong with the board). 
     * @param board as a 2d array
     * @return integer cost
     * 
     */

    public static int cost(int[][] board) {

        int cost = 0;

        ArrayList<Integer> isNumThere;

        for (int r = 0; r < 9; r++) {
            isNumThere = new ArrayList();
            for (int c = 0; c < 9; c++) {
                if (isNumThere.contains(board[r][c])) {

                    cost++;
               } else {
                    isNumThere.add(board[r][c]);

                }

            }

        }

        for (int c = 0; c < 9; c++) {
            isNumThere = new ArrayList();
            for (int r = 0; r < 9; r++) {
                if (isNumThere.contains(board[r][c])) {

                    cost++;
               } else {
                    isNumThere.add(board[r][c]);

                }

            }

        }

        for (int boxrow = 0; boxrow < 3; boxrow++) {
            for (int boxcol = 0; boxcol < 3; boxcol++) {

                for (int boxleftright = 0; boxleftright < 3; boxleftright++) {
                    isNumThere = new ArrayList();
                    for (int boxupdown = 0; boxupdown < 3; boxupdown++) {
                        if (isNumThere.contains(board[boxleftright 
                             + 3 * boxrow][boxupdown + 3 * boxcol])) {

                            cost++;
                       } else {
                            isNumThere.add(board[boxleftright 
                             + 3 * boxrow][boxupdown + 3 * boxcol]);

                        }

                    }

                }

            }

            for (int boxcol = 0; boxcol < 3; boxcol++) {

                for (int boxleftright = 0; boxleftright < 3; boxleftright++) {
                    isNumThere = new ArrayList();
                    for (int boxupdown = 0; boxupdown < 3; boxupdown++) {
                        if (isNumThere.contains(board[boxleftright 
                            + 3 * boxrow][boxupdown + 3 * boxcol])) {

                            cost++;
                       } else {
                            isNumThere.add(board[boxleftright 
                            + 3 * boxrow][boxupdown + 3 * boxcol]);

                        }

                    }

                }

            }

        }
        return cost;
    }
}
