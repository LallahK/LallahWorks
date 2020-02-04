import java.util.*;
import java.io.*;

public class Hashing {
	private static String[] hashDictionary = new String[100000];
	
	public static void main(String[] args) {
		generateDictionary();

		String pass = args[0];
		if (pass.length() < 12) {
			System.out.println("Bad Password");
			return;
		}
		
		ArrayList<String> wordsInPass = new ArrayList<String>();
		for (int i = 0; i < pass.length(); i++) {
			if (!((pass.charAt(i) >= 65 && pass.charAt(i) <= 90) || 
				(pass.charAt(i) >= 97 && pass.charAt(i) <= 122))) {
				wordsInPass.add(pass.substring(0, i));
				pass = pass.substring(i + 1, pass.length());
			}
		}
		if (pass.length() > 0)
			wordsInPass.add(pass);

		for (int i = 0; i < wordsInPass.size(); i++)
			if (!(hashDictionary[hashCode(wordsInPass.get(i))] == null)) {
				System.out.println("Bad Password");
				return;
			}	
		
	}

	private static void generateDictionary() {
		Scanner scan;
		try {
			scan = new Scanner(new File("words_alpha.txt"));
		} catch (Exception e) {
			return;
		}
		
		while(scan.hasNextLine()) {
			String str = scan.nextLine();
			hashDictionary[hashCode(str)] = str;
		}
	}
	
	private static int hashCode(String str) {
		int hash = 0;
        for (int i = 0; i < str.length(); i++) {
        	hash = (int)(hash*1.1) + str.charAt(i);
        }
		return (int)hash;
	}
}
