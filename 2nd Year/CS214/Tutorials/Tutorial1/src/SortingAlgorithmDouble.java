public class SortingAlgorithmDouble
{

	public static void main(String[] args)
	{
		double[] sortThis = new double[300000];
		
			//random
			/*for (int i = 0; i < sortThis.length; i++)
			{
				sortThis[i] = Math.random()*100000;
			}*/
			//sorted
			for (int i = 0; i < sortThis.length; i++)
			{
				sortThis[i] = Math.random() + i;
			}
			/*//reverse
			for (int i = 0; i < sortThis.length; i++)
			{
				sortThis[i] = sortThis.length - i - Math.random();
			}
			//duplicates
			int dups = (int)(Math.random()*19) + 1;;
			boolean repeat = false;
			int repeats = 0;	
			for (int i = 0; i < sortThis.length; i++)
			{
				if (i > 0)
					if (i % dups == 0 || repeat)
					{
						if (!repeat)
							repeats = (int)(Math.random()*5) + 1;
						repeat = true;
						sortThis[i] = sortThis[i - 1];
						repeats--;
						if (repeats == 0)
						{
							repeat = false;
							dups = (int)(Math.random()*19) + 1;
						}
					}
					else
						sortThis[i] = Math.random()*100000;
				else
					sortThis[i] = Math.random()*100000;
			}*/
		
		double[] copy = new double[sortThis.length];
		
		for (int i = 0; i < sortThis.length; i++)
			copy[i] = sortThis[i];
		double startTime = System.currentTimeMillis();
		InsertionSort(copy);
		double endTime = System.currentTimeMillis();
		System.out.println("Insertion Sort: " + (endTime - startTime));
		
		for (int i = 0; i < sortThis.length; i++)
			copy[i] = sortThis[i];
		startTime = System.currentTimeMillis();
		SelectionSort(copy);
		endTime = System.currentTimeMillis();
		System.out.println("Selection Sort: " + (endTime - startTime));
		
		for (int i = 0; i < sortThis.length; i++)
			copy[i] = sortThis[i];
		startTime = System.currentTimeMillis();
		ShellSort(copy);
		endTime = System.currentTimeMillis();
		System.out.println("Shell Sort: " + (endTime - startTime));
		
		for (int i = 0; i < sortThis.length; i++)
			copy[i] = sortThis[i];
		startTime = System.currentTimeMillis();
		MergeSort(copy);
		endTime = System.currentTimeMillis();
		System.out.println("Merge Sort: " + (endTime - startTime));
		
		for (int i = 0; i < sortThis.length; i++)
			copy[i] = sortThis[i];
		startTime = System.currentTimeMillis();
		QuickSort(copy);
		endTime = System.currentTimeMillis();
		System.out.println("Quick Sort: " + (endTime - startTime));
	}
	
	public static void InsertionSort(double[] sortThis)
	{
		int N = sortThis.length;
		for (int i = 1; i < N; i++)
			for (int j = i; j > 0 && sortThis[j] < sortThis[j - 1]; j--)
			{
				double temp = sortThis[j - 1];
				sortThis[j - 1] = sortThis[j];
				sortThis[j] = temp;
			}
		
	}
	
	public static void SelectionSort(double[] sortThis)
	{
		int N = sortThis.length;
		for (int i = 0; i < N; i++)
		{
			int min = i;
			for (int j = i + 1; j < N; j++)
				if (sortThis[j] < sortThis[min])
					min = j;
			double temp = sortThis[min];
			sortThis[min] = sortThis[i];
			sortThis[i] = temp;
		}
	}
	
	public static void ShellSort(double[] sortThis)
	{
		int N = sortThis.length;
		int jump = 1;
		while (jump < N/3)
			jump = 3*jump + 1;
		
		while (jump >= 1)
		{
			for (int i = jump; i < N; i++)
			{
				for (int j = i; j >= jump && sortThis[j] < sortThis[j - jump]; j = j - jump)
				{
					double temp = sortThis[j - jump];
					sortThis[j - jump] = sortThis[j];
					sortThis[j] = temp;
				}	
			}
			jump = jump/3;
		}
	}
	
	public static double[] aux;
	public static void MergeSort(double[] sortThis)
	{
		aux = new double[sortThis.length];
		sort(sortThis, 0, sortThis.length - 1);
	}
	public static void merge(double[] sortThis, int lo, int mid, int hi)
	{
		int i = lo;
		int k = mid + 1;
		
		for (int j = lo; j <= hi; j++)
			aux[j] = sortThis[j];
		
		for (int j = lo; j <= hi; j++)
		{
			if (i > mid)
				sortThis[j] = aux[k++];
			else if (k > hi)
				sortThis[j] = aux[i++];
			else if (aux[k] < aux[i])
				sortThis[j] = aux[k++];
			else
				sortThis[j] = aux[i++];
		}
	}
	public static void sort(double[] sortThis, int lo, int hi)
	{
		if (hi <= lo)
			return;
		int mid = lo + (hi - lo)/2;
		sort(sortThis, lo, mid);
		sort(sortThis, mid + 1, hi);
		merge(sortThis, lo, mid, hi);
	}
	
	public static void QuickSort(double[] sortThis)
	{
		StdRandom.shuffle(sortThis);
		quick(sortThis, 0, sortThis.length - 1);
	}
	public static void quick(double[] sortThis, int lo, int hi)
	{
		if (hi <= lo) return;
		int j = partition(sortThis, lo, hi);
		quick(sortThis, lo, j - 1);
		quick(sortThis, j + 1, hi);
	}
	public static int partition(double[] sortThis, int lo, int hi)
	{
		int i = lo;
		int k = hi + 1;
		double v = sortThis[lo];
		while (true)
		{
			while(sortThis[++i] < v)
				if (i == hi)
					break;
			while (sortThis[--k] > v)
				if (k == lo)
					break;
			if (i >= k)
				break;
			
			double temp = sortThis[i];
			sortThis[i] = sortThis[k];
			sortThis[k] = temp;
		}
		double temp = sortThis[lo];
		sortThis[lo] = sortThis[k];
		sortThis[k] = temp;
		return k;
	}
}
