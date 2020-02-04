import java.awt.Color;
import java.util.Scanner;
import java.io.File;

public class GUI {
  private static int[][] sudoku;
  private static Color c1 = new Color(10, 10, 10);
  private static Color c2 = new Color(240, 240, 240);
  private static Color c3 = new Color(150, 150, 150);  

  public static void main(String[] args) {
  }
  
  public static void gui(int[][] sudokuBoard) {
    sudoku = new int[9][9];
    sudoku = sudokuBoard;
    StdDraw.setPenColor(c3);
    StdDraw.filledRectangle(0.5, 0.5, 0.5, 0.5);
    StdDraw.setPenColor(c2);
    for (int i = 0; i < 9; i++) {
      for (int k = 0; k < 9; k++) {
        StdDraw.filledRectangle((1.0 / 10) * (i + 1) + 0.003, 
        (9 - k) * (1.0 / 10) - 0.02, (1.0 / 30), (1.0 / 30));
      }
    }
    StdDraw.setPenColor(c1);
    for (int i = 0; i < 2; i++) {
      StdDraw.filledRectangle(0.5,
        ((9 - (2.5 + i*3)) * (1.0 / 10) - 0.02), (1.0 / 10) * (4.5), 0.002);
      StdDraw.filledRectangle(((1.0 / 10) * (3.5 + i*3) + 0.003), 0.48,
        0.002, (1.0 / 10) * (4.5));
    }
   
    StdDraw.filledRectangle(0.5, 0.96, 0.12, 0.02); 
    StdDraw.setPenColor(c2);
    StdDraw.text(0.5, 0.955, "Solving...");
    double t = System.currentTimeMillis();
   
    Egg solvedEgg;
    solvedEgg = Solver.solve(sudokuBoard);
    
    t = System.currentTimeMillis() - t;
    StdDraw.setPenColor(c1);
    StdDraw.filledRectangle(0.5, 0.96, 0.12, 0.02);
    StdDraw.setPenColor(c2);
    StdDraw.text(0.5, 0.955, "Time: " + (t / 1000));    
  
    randomNumPrint();
 
    StdDraw.setPenColor(c1);
    int[][] board = solvedEgg.getBoard();
    for (int i = 0; i < 36; i = i + 4) {
      for (int k = 1; k < 37; k = k + 4) {
        StdDraw.setPenColor(c2);
        StdDraw.filledRectangle((1.0 / 10) * (i % 9 + 1) + 0.003,
        (9 - k % 9) * (1.0 / 10) - 0.02, (1.0 / 30), (1.0 / 30));
        StdDraw.setPenColor(c1);
        StdDraw.text((1.0 / 10) * (i % 9 + 1) + 0.003, 
        (9 - k % 9) * (1.0 / 10) - 0.02, Integer.toString(board[k % 9][i % 9]));
        StdDraw.pause(3);
      }
    }
    
    nextChoiceWait();
  }

  private static void randomNumPrint() {
    for (int i = 0; i < 500; i++) {
      int rRow = (int) (Math.random() * 9);
      int rCol = (int) (Math.random() * 9);
      int rNum = (int) (Math.random() * 9) + 1;
      StdDraw.setPenColor(c2);
      StdDraw.filledRectangle((1.0 / 10) * (rRow + 1) + 0.003,
        (9 - rCol) * (1.0 / 10) - 0.02, (1.0 / 30), (1.0 / 30));
      StdDraw.setPenColor(c1);
      StdDraw.text((1.0 / 10) * (rRow + 1) + 0.003,
        (9 - rCol) * (1.0 / 10) - 0.02, Integer.toString(rNum));
      StdDraw.pause(3);
    }
  }
  
  private static void nextChoiceWait() {
    StdDraw.setPenColor(c1);
    StdDraw.filledRectangle(0.12, 0.96, 0.06, 0.02);
    StdDraw.filledRectangle(0.26, 0.96, 0.06, 0.02);    

    StdDraw.setPenColor(c2);
    StdDraw.filledRectangle(0.82, 0.96, 0.14, 0.02);
    StdDraw.text(0.12, 0.955, "EXIT");
    StdDraw.text(0.26, 0.955, "REDO");
    StdDraw.setPenColor(c1);
    StdDraw.text(0.82, 0.955, "Enter new file");
    
    boolean chosen  = false;
    while (!chosen) {
      while (!StdDraw.isMousePressed()) {}
      double x = StdDraw.mouseX();
      double y = StdDraw.mouseY();
      while (StdDraw.isMousePressed()) {}
      
      if (x > 0.06 && x < 0.18 && y < 0.98 && y > 0.94) {
        System.exit(0);
      }
      if (x > 0.2 && x < 0.32 && y < 0.98 && y > 0.94) {
        gui(sudoku);
      }
      if (x < 0.96 && x > 0.68 && y < 0.98 && y > 0.94) {
        StdDraw.setPenColor(c2);
        StdDraw.filledRectangle(0.82, 0.96, 0.14, 0.02);
        
        String file = "";
        while (true) {
          while (!StdDraw.hasNextKeyTyped()) {}
          char c = StdDraw.nextKeyTyped();
          if (c == 10) {
            int[][] newSud = new int[9][9];
            Scanner scan;
            try {
              scan = new Scanner(new File("TestCases/" + file));
              chosen = true;
            } catch (Exception e) {
              chosen = false;
              return;
            }
            for (int i = 0; i < 9 && chosen; i++) {
              String line = scan.nextLine();
              for (int k = 0; k < 9; k++) {
                if (line.charAt(k) == '.') {
                  newSud[i][k] = 0;
                } else {
                  newSud[i][k] = Integer.parseInt(line.substring(k, k + 1));
                }
              }
            }
            scan.close();
            gui(newSud);
          } else if (c == 8 && file.length() > 0) {
            file = file.substring(0, file.length() - 1);
          } else {
            file = file + Character.toString(c);
          }
          StdDraw.setPenColor(c2);
          StdDraw.filledRectangle(0.82, 0.96, 0.14, 0.02);
          StdDraw.setPenColor(c1);
          StdDraw.text(0.82, 0.955, file);
        }
        
      }
    }
  }
}
