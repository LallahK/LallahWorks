class Egg implements Comparable<Egg> {
  private int[][] board = new int[9][9];
  private int cost;
  
  public Egg(int[][] newBoard) {
    this.board = newBoard;
    this.cost = Utils.cost(this.board);
  }
  
  public int[][] getEgg() {
    return board;
  }
  
  public int[][] getBoard() {
    return this.board;
  }
  
  public int compareTo(Egg other) {
    if (this.getCost() < other.getCost()) {
      return - 1;
    } else if (this.getCost() == other.getCost()) {
      return 0;
    } else {
      return 1;
    }
  }
  
  public int[] getRow(int i) {
    return board[i];
  }
  
  public int getCost() {
    return cost;
  }
  
  public void newCost() {
    this.cost = Utils.cost(this.board);
  }
  
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
