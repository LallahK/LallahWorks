import java.util.Scanner;

public class Palindrome {
  public static void main(String[] args) {
    Scanner scan = new Scanner(System.in);
    String word = "";
    
    while (true) {
      System.out.println("Enter next character");
      String c = scan.next();
      word = word + c;
      System.out.println("your word: " + word);
      if (rabinKarp(word)) {
        if (palindrome(word)) {
          System.out.println("PALINDROME");
        }
      }
    }   
  }
  
  private static boolean palindrome(String word) {
    int middle = (int)((word.length() + 1) / 2 - 1); 
    for (int i = 0; i <= middle; i++) {
      if (!(word.charAt(i) == word.charAt(word.length() - i - 1))) {
        return false;
      }
    }
    return true;
  }
  
  private static boolean rabinKarp(String word) {
    int forward = 0;
    int backward = 0;
    int middle = (int)((word.length() + 1) / 2 - 1);
    
    for (int i = 0; i <= middle; i++) {
      forward = (26 * forward + word.charAt(i)) % 101;
      backward = (26 * backward + word.charAt(word.length() - i - 1)) % 101;
    }
  
    return forward == backward;
  }
}
