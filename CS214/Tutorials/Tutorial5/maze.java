import java.util.ArrayList;

public class maze {
	public static void main(String args[]) {
		int n = Integer.parseInt(args[0]);
		boolean[][] maze = new boolean[n][n];
		
		int[][][] theMaze = genMaze(maze);
		
		for (int i = 1; i < theMaze.length*2; i++) {
			String line = "";
			if (i % 2 == 0) {
				for (int k = 0; k < theMaze.length; k++) {
					if (theMaze[i/2][k][0] == i/2 - 1 || theMaze[i/2][k][0] == i/2 + 1)
						line = line + "| ";
					else
						line = line + "  ";
				}
			} else {
				line = line + "o";
				for (int k = 0; k < theMaze.length; k++) {
					if (theMaze[i/2][k][1] == k - 1 || theMaze[i/2][k][1] == k + 1)
						line = line + "-";
					else
						line = line + " ";
					line = line + "o";
				}
			}
			System.out.println(line);				
		}
	}
	
	private static int[][][] genMaze(boolean[][] maze) {
		//int sR = (int)(Math.random()*maze.length);
		//int sC = (int)(Math.random()*maze.length);
		int sR = 0;
		int sC = 0;
		
		ArrayList<int[]> q = new ArrayList<int[]>();
		maze[sR][sC] = true;
		q.add(new int[] {sR, sC});
		int[][][] mazeDir = new int[maze.length][maze.length][2];
		
		while (!q.isEmpty()) {
			int r = (int)(Math.random()*q.size());
			int[] v = q.get(r);
			q.remove(r);
			//System.out.println(v[0] + " " + v[1]);
			for (int i = -1; i < 2; i = i + 2) {
				if (isAdj(v[0] + i, v[1], maze.length))
					if (!maze[v[0] + i][v[1]]) {
						mazeDir[v[0] + i][v[1]][0] = v[0];
						mazeDir[v[0] + i][v[1]][1] = v[1];
						maze[v[0] + i][v[1]] = true;
						q.add(new int[] {v[0] + i,v[1]});
					}
				if (isAdj(v[0], v[1] + i, maze.length))
					if (!maze[v[0]][v[1] + i]) {
						mazeDir[v[0]][v[1] + i][0] = v[0];
						mazeDir[v[0]][v[1] + i][1] = v[1];
						maze[v[0]][v[1] + i] = true;
						q.add(new int[] {v[0], v[1] + i});
					}
			}
		}
		for (int i = 0; i < mazeDir.length; i++) {
			String line = "";
			for (int k = 0; k < mazeDir.length; k++)
				line = line + mazeDir[i][k][0] + "|" + mazeDir[i][k][1] + " ";
			System.out.println(line);
		}
		return mazeDir;
				
	}
	private static boolean isAdj(int row, int col, int bound) {
		if (row < 0 || row >= bound || col < 0 || col >= bound)
			return false;
		else
			return true;	
	}
}

