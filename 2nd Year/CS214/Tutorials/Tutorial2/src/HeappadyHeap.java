
public class HeappadyHeap 
{
	public static void main()
	{
		
	}
	
	public static void insert()
	{
		
	}
	
	public static void swim(Node node)
	{
		Node thisNode = node;
		while(!(thisNode.parent == null) && thisNode ) 
	}
	
	public static void sink()
	{
		
	}
	
}

class Node<Item>
{
	public Comparable item;
	public Node parent = null;
	public Node left = null;
	public Node right = null;
	
	public Node(Comparable item)
	{
		this.item = item;
	}
}
