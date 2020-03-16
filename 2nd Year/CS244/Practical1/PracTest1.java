import java.util.*;

public class PracTest1 {
	private static int[] hashValues;
	
	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		ArrayList<String> lines = new ArrayList<String>();
		
		while (input.hasNextLine())
			lines.add(input.nextLine());

		hashValues = new int[lines.size()];
		int n = 0;

		BTree hashTree= new BTree();
		for (int i = 0; i < lines.size(); i++) {
			int newHash = hashSU(lines.get(i));
			if (!(hashTree.get(newHash) == null)) {
				newHash = hashSU(reverse(lines.get(i)));
				if(!(hashTree.get(newHash) == null))
					newHash = -1;
				else
					hashTree.put(newHash, newHash);
			} else 
				hashTree.put(newHash, newHash);
			hashValues[n] = newHash;
			n++;
		}

		for (int i = 0; i < n; i++)
			System.out.println(hashValues[i]);
	}
	
	private static int hashSU(String hash) {
		int first = Integer.parseInt(hash.substring(0,3));
		int sec = Integer.parseInt(hash.substring(3,6));
		int third = Integer.parseInt(hash.substring(6,8));
		return first + sec + third;
	}

	private static String reverse(String string) {
		String newString = "";
		for (int i = string.length() - 1; i >= 0; i--)
			newString = newString + string.substring(i, i + 1);
		return newString;
	}
}
