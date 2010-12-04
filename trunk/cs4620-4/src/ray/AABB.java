package ray;

import java.util.Arrays;
import java.util.Comparator;
import ray.math.Point3;
import ray.math.Vector3;
import ray.surface.Surface;
/**
 * Class for Axis-Aligned-Bounding-Box to speed up the intersection look up time.
 *
 * @author ss932
 */
public class AABB {
	boolean isLeaf;
	
	/** The current bounding box for this tree node.
	 *  The bounding box is described by 
	 *  (minPt.x, minPt.y, minPt.z) - (maxBound.x, maxBound.y, maxBound.z). */
	Point3 minBound, maxBound;
	
	/** child[0] is the left child. child[1] is the right child. */
	AABB []child;
	
	/** A shared surfaces array that will be used across every node in the tree. */
	static Surface []surfaces;
	
	/** left and right are indices for []surfaces,
	 * meaning that this tree node contains a set of surfaces 
	 * from surfaces[left] to surfaces[right-1]. */
	int left, right;
	
	/** A comparator class that can sort surfaces by x, y, or z coordinate. */
	static MyComparator cmp = new MyComparator();
	
	
	public AABB(boolean isLeaf, Point3 minBound, Point3 maxBound, AABB leftChild, AABB rightChild, int left, int right) {
		this.isLeaf = isLeaf;
		this.minBound = minBound;
		this.maxBound = maxBound;
		this.child = new AABB[2];
		this.child[0] = leftChild;
		this.child[1] = rightChild;
		this.left = left;
		this.right = right;
	}
	
	public AABB() {	}
	
	/**
	 * Set the shared static surfaces that every node in the tree will use by 
	 * using left and right indices.
	 */
	public static void setSurfaces(Surface []surfaces) {
		AABB.surfaces = surfaces;
	}
	
	/**
	 * Create an AABB [sub]tree.  This tree node will be responsible for storing
	 * and processing surfaces[left] to surfaces[right-1].
	 * @param left The left index of []surfaces
	 * @param left The right index of []surfaces
	 */
	public static AABB createTree(int left, int right) {
		// TODO(B): fill in this function.
		
		// ==== Step 1 ====
		// Find out the BIG bounding box enclosing all the surfaces in the range [left, right)
		// and store them in minB and maxB.
		// Hint: To find the bounding box for each surface, use getMinBound() and getMaxBound() */
		Point3 minB = new Point3(Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY, Double.POSITIVE_INFINITY); 
		Point3 maxB = new Point3(Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY, Double.NEGATIVE_INFINITY);
		
		
		// ==== Step 2 ====
		// Check for the base case. 
		// If the range [left, right) is small enough, just return a new leaf node.
		
		
		// ==== Step 3 ====
		// Figure out the widest dimension (x or y or z).
		// If x is the widest, set widestDim = 0. If y, set widestDim = 1. If z, set widestDim = 2.
		int widestDim = 0;

		
		// ==== Step 4 (DONE) ====
		// Sort surfaces according to the widest dimension.
		// You can also implement O(n) randomized splitting algorithm.
		cmp.setIndex(widestDim);
		Arrays.sort(surfaces, left, right, cmp);
		
		
		// ==== Step 5 ====
		// Recursively create left and right children.
		AABB leftChild = null, rightChild = null;

		
		return new AABB(false, minB, maxB, leftChild, rightChild, left, right);
	}
	
	/**
	 * Set outRecord to the first intersection of ray with the scene. Return true
	 * if there was an intersection and false otherwise. If no intersection was
	 * found outRecord is unchanged.
	 *
	 * @param outRecord the output IntersectionRecord
	 * @param ray the ray to intersect
	 * @param anyIntersection if true, will immediately return when found an intersection
	 * @return true if and intersection is found.
	 */
	public boolean intersect(IntersectionRecord outRecord, Ray rayIn, boolean anyIntersection) {
		// TODO(B): fill in this function.
		// Hint: For a leaf node, use a normal linear search. 
		// Otherwise, search in the left and right children.
		return false;
	}
	
	/** 
	 * Check if the ray intersects this bounding box.
	 * @param ray
	 * @return true if ray intersects this bounding box
	 */
	private boolean isIntersect(Ray ray) {
		// TODO(B): fill in this function.
		// Hint: reuse your code from box intersection.
		return false;
	}
}

class MyComparator implements Comparator<Surface> {
	int index;
	public MyComparator() {	}
	
	public void setIndex(int index) {
		this.index = index;
	}
	
	public int compare(Surface o1, Surface o2) {
		double v1 = o1.getAveragePosition().getE(index);
		double v2 = o2.getAveragePosition().getE(index);
		if(v1 < v2) return 1;
		if(v1 > v2) return -1;
		return 0;
	}
	
}
