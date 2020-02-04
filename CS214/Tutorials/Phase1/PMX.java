import java.io.File;
import java.util.ArrayList;
import java.util.Scanner;

public class PMX 
{
	public static void main(String[] args)
	{
		int[] p1 = new int[9];
		int[] p2 = new int[9];
		int a;
		int b;
			
		Scanner scan;
		try {
			scan = new Scanner(new File(args[0]));
		} catch (Exception e) {
			System.out.println("No file found");
			return;
		}
			
		String line = scan.nextLine();
		a = Integer.parseInt(line.substring(0, 1));
		b = Integer.parseInt(line.substring(2, 3));
		line = scan.nextLine();
		for (int i = 0; i < 9; i++)
			p1[i] = Integer.parseInt(line.substring(2 * i, 2 * i + 1));
		line = scan.nextLine();
		for (int i = 0; i < 9; i++)
			p2[i] = Integer.parseInt(line.substring(2 * i, 2 * i + 1));		
		scan.close();
		
		
		pmx baby = new pmx(p1, p2, a, b);
		baby.breed();
		
		String babe = "";
		for (int i = 0; i < 9; i++)
			babe += baby.getChild()[i];
		System.out.println(babe);
	}
	
	static class pmx
	{
		private int[] parent1 = new int[9];
		private int[] parent2 = new int[9];
		private int[] child = new int[] { 0, 0, 0, 0, 0, 0, 0, 0, 0};
		ArrayList<int[]> lostChildren = new ArrayList<int[]>();
		
		public pmx(int[] parent1, int[] parent2, int i1, int i2)
		{
			this.parent1 = parent1;
			this.parent2 = parent2;
			
			for (int i = i1 - 1; i < i2; i++)
				child[i] = this.parent1[i];
			
			for (int i = i1 - 1; i < i2; i++)
			{
				if (findIndex(child, this.parent2[i]) == -1)
						lostChildren.add(new int[] {this.parent2[i],i});
			}
		}
		
		public void breed()
		{
			for (int i = 0; i < lostChildren.size(); i++)
				child[placeAt(lostChildren.get(i)[1])] = lostChildren.get(i)[0];
			for (int i = 0; i < 9; i++)
				if (child[i] == 0)
					child[i] = parent2[i];
		}
		
		public int placeAt(int i)
		{
			int index = i;
			int val = parent1[index];
			boolean found = false;;
			
			while (!found)
			{
				index = findIndex(parent2, val);
				if (child[index] == 0)
				{
					found = true;
					break;
				} else
					val = parent1[index];
			}
			return index;
		}
		
		private int findIndex(int[] array, int value)
		{ 
			for (int i = 0; i < array.length; i++)
				if (array[i] == value)
					return i;
			return -1;
		}
		
		public int[] getChild()
		{
			return child;
		}
	}
}
