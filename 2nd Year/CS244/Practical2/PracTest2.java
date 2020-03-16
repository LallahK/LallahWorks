import java.util.*;

public class PracTest2 {
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

	while (input.hasNextLine()) {
      lines.add(input.nextLine());	
	}
    
    String f = lines.get(0);
    int i;
    for (i = 0; i < f.length() && !(f.charAt(i) == ' '); i++) {}
	length = Integer.parseInt(f.substring(0, i));
    f = f.substring(i + 1, f.length());
	for (i = 0; i < f.length() && !(f.charAt(i) == ' '); i++) {}
    words = Integer.parseInt(f.substring(0, i));
	f = f.substring(i + 1, f.length());
    for (i = 0; i < f.length() && !(f.charAt(i) == ' '); i++) {}
    possWords = Integer.parseInt(f);	  
      
	language = new String[words];
	for (i = 1; i <= words; i++)
	  language[i - 1] = lines.get(i);
        
        
	for (i = words + 1; i < lines.size(); i++) {
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
