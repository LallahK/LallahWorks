import java.util.*;

public class S1P1_2015 {
  private static int length, words, possWords;
  private static String[] language;

  private static void Boo(int index, String val) {
    if (index == length) {
	  for (int i = 0; i < language.length; i++) {
        if (language[i].equals(val)) {
	      iterCount++;
        }
      }
      return;
	} else {
	  for (int i = 0; i < combo[index].length(); i++) {
		Boo(index + 1,val + combo[index].substring(i, i + 1));
	  }
	}
	return;	
  }

  public static void main(String[] args) {
    Scanner input = new Scanner(System.in);
	ArrayList<String> lines = new ArrayList<String>();	
	Out output;

	while (input.hasNextLine()) {
      lines.add(input.nextLine());	
	}
	length = Integer.parseInt(lines.get(0).substring(0,1));
	words = Integer.parseInt(lines.get(0).substring(2,3));
	possWords = Integer.parseInt(lines.get(0).substring(4,5));	  
      
	language = new String[words];
	for (int i = 1; i <= words; i++)
	  language[i - 1] = lines.get(i);
        
        
	for (int i = words + 1; i < lines.size(); i++) {
	  copyStrings(length, lines.get(i));
	  int size = 1;
	  for (int k = 0; k < length; k++) {
		size = size*combo[k].length();
	  }
      iterCount = 0;
	  Boo(0,"");
        
      System.out.println(iterCount);
	}				
  }
	
  private static int iterCount = 0;
  private static String[] combo;
  private static void copyStrings(int length, String string) {
	combo = new String[length];
	int count = 0;
	while (string.length() > 0) {
	  if (string.charAt(0) == '(') {
	    int k = 0;
		for (k = 0; k < string.length() && !(string.charAt(k) == ')'); k++) {}
		  combo[count] = string.substring(1,k);
		  string = string.substring(k + 1, string.length());
	    } else {
		  combo[count] = string.substring(0, 1);
		  string = string.substring(1, string.length());
		}
	  count++;
	}
  }
}
