
public class ReverseBST<Key extends Comparable<Key>, Value>
{
	Node root;
	
	public void reverse(Node x)
	{
		if (x == null) 
			return;
		else 
			reverse(x.right);
		reverse(x.left);
		Node temp = x.right;
		x.right = x.left;
		x.left = temp;		
	}
	
	public void put(Key key, Value value)
	{
		root = put(root, key, value);
	}
	
	public Node put(Node x, Key key, Value value)
	{
		if (x == null)
			return new Node(key, value);
		
		int compare = key.compareTo(x.key);
		if (compare < 0)
			x.left = put(x.left, key, value);
		else if (compare > 0)
			x.right = put(x.right, key, value);
		else
			x.val = value;
		
		return x;
	}
	
	public Value get(Key key)
	{
		return get(root, key);
	}
	
	public Value get(Node x, Key key)
	{
		if (x == null)
			return null;
		
		int compare = key.compareTo(x.key);
		if (compare < 0)
			return get(x.left, key);
		else if (compare > 0)
			return get(x.right, key);
		else		
			return x.val;
	}
	
	public void reconstruct(Key[] keys, Value[] values)
	{
		Stack BST = new Stack();
		for (int i = 0; i < keys.length; i++)
			BST.push(keys[i], values[i]);
		
		while (BST.last != null)
		{
			Node popped = BST.pop();
			put(popped.key, popped.val);
		}
	}
	
	class Node
	{
		private Key key;
		private Value val;
		private Node left, right;
		
		public Node(Key key, Value val)
		{
			this.key = key;
			this.val = val;
		}
		
	}
	
	class Stack
	{
		Node last;
		
		public void push(Key key, Value val)
		{
			if (last.key == null)
				last = new Node(key, val);
			else
			{
				Node nNode = new Node(key, val);
				nNode.right = last;
				last = nNode;
			}
		}
		
		public Node pop()
		{
			Node rNode = new Node(last.key, last.val);
			last.right = last;
			return rNode;
		}
		
	}
	
}
