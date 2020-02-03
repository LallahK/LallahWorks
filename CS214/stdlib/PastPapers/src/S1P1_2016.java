import java.util.*;

public class S1P1_2016 {
	private static double[][][] nodes;
	private static int nodeCount, edges;	

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		ArrayList<String> lines = new ArrayList<>(0);
		lines.add(input.nextLine());
		nodeCount = Integer.parseInt(lines.get(0).substring(0,1));
		edges = Integer.parseInt(lines.get(0).substring(2,3));
		
		nodes = new double[nodeCount][nodeCount][2];
		for (int i = 0; i < nodeCount; i++)
			for (int j = 0; j < nodeCount; j++)
				for (int k = 0; k < 2; k++)
					nodes[i][j][k] = 0;
		
		for (int i = 1; i < lines.size(); i++) {
			int st = Integer.parseInt(lines.get(i).substring(0,1));
			int en = Integer.parseInt(lines.get(i).substring(2,3));
			double dis = Integer.parseInt(lines.get(i).substring(4,5));
			nodes[st][en][0] = dis;
		}
		
		
	}
	
	private static double treeLength;
	private static String path = "0"; 
	private static void findMaxTree() {
		for (int i = 0; i < nodeCount; i++) {
			int newIndex = findMax(nodes[Integer.parseInt(path.substring(path.length() - 1, path.length()))]);
			path += newIndex;		
		}
	}
	
	private static int findMax(double[][] arr) {
		int iMax = 0;
		for (int i = 1; i < arr.length; i++)
			if (arr[i][0] > arr[iMax][0] && arr[i][1] == 0) {
				iMax = i;
			}
		treeLength += arr[iMax][0];
		arr[iMax][1] = 1.0;
		return iMax;
	}
	
	/*private static void shellSort() {
		int jump = 1;
		while (3*jump + 1 < distances.length)
			jump = 3*jump + 1;
		while (jump >= 1) {
			for (int i = 1;i  < distances.length; i = i + jump)
				for (int k = i; k > 0 && distances[k] > distances[k - jump]; k = k - jump) {
					double Temp = distances[k - jump];
					distances[k - jump] = distances[k];
					distances[k] = Temp;

					int in1 = index[k - jump][0];
					int in2 = index[k - jump][1];
					index[k - jump][0] = index[k][0];
					index[k - jump][1] = index[k][1];
					index[k][0] = in1;
					index[k][1] = in2;
				}
			jump = jump/3;
		} 		
	}*/
}
