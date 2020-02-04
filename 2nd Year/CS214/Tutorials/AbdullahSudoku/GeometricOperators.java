import java.util.*;

/**
 * 
 */

/**
 * @author ahmfar
 *
 */
public class GeometricOperators {

  /**
   * @param args
   */
  public static void main(String[] args) {
    // TODO Auto-generated method stub

  }

  public static Integer[][] PMXCrossover(Integer[][] eggBest, Integer[][] egg) {
    Integer[][] child = new Integer[9][9];
    for (int i = 0; i < 9; i++) {
      Random rand = new Random();
      int a = rand.nextInt(9) + 1;
      int b = rand.nextInt(9) + 1;
      if (a == b) {
        swap(a, b);
      }
      child[i] = PMX.pmx(a, b, eggBest[i], egg[i]);
    }
    return child;
  }

  public static Integer[][] MultiParentalCrossover(Integer[][] eggBest, Integer[][] egg,
      Integer[][] lastBest) {
    Integer[][] child = new Integer[9][9];
    for (int i = 0; i < 9; i++) {
      Integer[] mask = MPSX.generateMask();
      child[i] = MPSX.mPMX(eggBest[i], egg[i], lastBest[i], mask);
    }
    return child;
  }

  public static void swap(int a, int b) {
    int temp = 0;
    temp = a;
    a = b;
    b = temp;
  }

  public static Integer[][] FeasGeoMutation(Integer[][] prefilter, Integer[][] egg) {
    return FeasibleGeometricMutation.FGM(prefilter, egg);

  }

  public static Integer[][] GeometricOperator(Integer[][] eggBest, Integer[][] egg,
      Integer[][] lastBest, int[][][] Nests, Integer[][] prefilter, float P_multi, float P_mutate) {
    for (int i = 0; i < Nests[i].length; i++) {
      if (egg.equals(eggBest) == false) {
        if (Math.random() < P_multi) {
          egg = PMXCrossover(eggBest, egg);
        } else {
          egg = MultiParentalCrossover(eggBest, egg, lastBest);
        }
        if (Math.random() < P_mutate) {
          egg = FeasGeoMutation(prefilter, egg);
        }
      }
    }
    return egg;
  }
}
