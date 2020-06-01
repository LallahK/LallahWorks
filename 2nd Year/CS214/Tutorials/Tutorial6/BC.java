import java.io.*;
import java.util.Scanner;

public class BC {
    public static void main (String[] args) {
        Scanner file;
        int vertices, edges;
        String filename = args[0];

        try {
            file = new Scanner(new File(filename));
        } catch (Exception e) {
            System.err.print("File not found");
            return;
        }
        
        vertices = file.nextInt();
        edges = file.nextInt();
        int[][] graph = new int[vertices][vertices];
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                graph[i][j] = -1;
            }
        }

        while (file.hasNextInt()) {
            int start = file.nextInt();
            int end = file.nextInt();
            int i = 0;
            while (graph[start][i] != -1) {
                i++;
            }
            graph[start][i] = end;
        }

        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                System.out.print(graph[i][j] + " ");
            }
            System.out.println();
        }
    }

}
