import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.Scanner;

/**
 * This is the main class of the Sudoku program
 * 
 * @author Farhaan Ahmed, Abdullah Karbanee, Roscoe Kerby
 */

public class Sudoku {
  private static int[][] sudokuBoard = new int[9][9];

/**
 * The main function to be called, where the switch statement indicates
 * which function will be calles
 *
 * @param args The mode that will be executed
 */
  public static void main(String[] args) {
    int mode = Integer.parseInt(args[0]);
    String fileName = args[1];
    switch (mode) {
    case 0 : PMX.main(new String[] {fileName});
      break;
    case 1 :
      break;
    case 2 : Prefiltering.main(new String[] {fileName});
      break;
    default :
    }
  }

/**
 * Reads the 9x9 sudoku board from a file and saved it to the local array
 * Will be used in Phase 2
 * 
 * @param fileName The name of the file
 * @Exception exception when the file to be read is not found
 */
  private static void readFile(String fileName) {
    Scanner file;
    try {
      file = new Scanner(new File(fileName));
    } catch (Exception e) {
      System.err.print("File not found");
      return;
    }

    int rows = 0;
    while (file.hasNext()) {
      String line = "";
      for (int i = 0; i < 9; i++) {
        if (line.charAt(i) == '.') {
          sudokuBoard[rows][i] = 0;
        } else {
          sudokuBoard[rows][i] = Integer.parseInt(line.substring(i, i + 1));
        }
      }
    }
    file.close();
  }

/**
 * Writes the game board to a file.
 * 
 * @param outputFileName The name of the output file
 * @FileNotFoundException Throws exception if no file is found or if the 
 *                        format is incorrect
 * @UnsupportedEncodingException Throws exception if no file is found or
 *                        if the format is incorrect
 */

  private static void writeFile(String outputFileName) {
    PrintWriter writeGame = null;
    try {
      writeGame = new PrintWriter(outputFileName, "UTF-8");
    } catch (FileNotFoundException e) {
      e.printStackTrace();
      return;
    } catch (UnsupportedEncodingException e) {
      e.printStackTrace();
      return;
    }

    for (int i = 0; i < 9; i++) {
      String line = "";
      for (int k = 0; k < 9; k++) {
        if (sudokuBoard[i][k] == 0) {
          line += ".";
        } else {
          line += sudokuBoard[i][k];
        }
        writeGame.write(line);
      }
      writeGame.close();
    }
  }
}

