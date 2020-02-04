import java.awt.Color;

public class GUI {
  public static void main(String[] args) {
  }
  
  public static void gui(int[][] sudokuBoard) {
    StdDraw.setPenColor(new Color(150, 150, 150));
    StdDraw.filledRectangle(0.5, 0.5, 0.5, 0.5);
    StdDraw.setPenColor(new Color(240, 240, 240));
    for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
        StdDraw.filledRectangle((1.0 / 10) * (i + 1) + 0.003, 
        (9 - k) * (1.0 / 10) - 0.02, (1.0 / 30), (1.0 / 30));
      }
    }
    
    Solver.solver(sudokuBoard);
    for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
        StdDraw.filledRectangle((1.0 / 10) * (i + 1) + 0.003, 
        (9 - k) * (1.0 / 10) - 0.02, , (1.0 / 30));
      }
    }
  }  
}
