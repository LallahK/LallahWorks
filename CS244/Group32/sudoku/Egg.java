/**
 * This Egg class represents a single board and holds
 * additional operations to be done on the board
 *
 * 
 */
class Egg implements Comparable<Egg> {
  private int[][] board = new int[9][9];
  private int cost;
/**
 * This is the constructor for the Egg class. It 
 * creates the egg and calculates the cost for future 
 * referencing
 * 
 * @param newBoard The board that is inside the egg
 */
  Egg(int[][] newBoard) {
    this.board = newBoard;
    this.cost = Utils.cost(this.board);
  }

/**
 * Returns the board associated with the Egg 
 *
 * @return The 2D int array of the board
 */  
  public int[][] getBoard() {
    return this.board;
  }
  
/**
 * Compares the given board to another board, returning a value of -1
 * if this board has a lower cost than the other board, 0 if they have
 * the same cost and 1 if the cost of this board is higher than the 
 * cost of the other board
 *
 * @param other The other board to which this board is compared
 * @return Returns the relevant integer based on the comparison
 */
  public int compareTo(Egg other) {
    if (this.getCost() < other.getCost()) {
      return -1;
    } else if (this.getCost() == other.getCost()) {
      return 0;
    } else {
      return 1;
    }
  }
  
/**
 * Returns a copy of a row of the Egg
 *
 * @param i The index of the row
 * @return Returns the 1D int array of the row
 */
  public int[] getRow(int i) {
    int[] row = new int[9];
    for (int k = 0; k < 9; k++) {
      row[k] = board[i][k];
    }
    return row;
  }
  
/**
 * Returns the cost of this egg
 *
 * @return Returns the cost as an integer
 */
  public int getCost() {
    return cost;
  }
  
/**
 * Allows the cost of the class to be reset
 *
 */
  public void newCost() {
    this.cost = Utils.cost(this.board);
  }
  
/**
 * Prints the board onto standard output
 *
 */
  public void print() {
    for (int i = 0; i < 9; i++) {
      String line = "";
      for (int k = 0; k < 9; k++) { 
        line += board[i][k];
      }
      System.out.println(line);
    }
  }
}
