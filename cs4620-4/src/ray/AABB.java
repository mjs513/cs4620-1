package ray;

import java.util.Arrays;
import java.util.Comparator;

import ray.math.Point3;
import ray.surface.Box;
import ray.surface.Surface;
/**
 * Class for Axis-Aligned-Bounding-Box to speed up the intersection look up time.
 *
 * @author ss932
 */
public class AABB
{
	/** The current bounding box for this tree node.
	 *  The bounding box is described by 
	 *  (minPt.x, minPt.y, minPt.z) - (maxBound.x, maxBound.y, maxBound.z). */
	Point3 minBound, maxBound;
	
	int ntotal = 0,nfailed = 0;
	
	/** child[0] is the left child. child[1] is the right child. */
	AABB leftChild, rightChild;
	
	/** A shared surfaces array that will be used across every node in the tree. */
	static Surface []surfaces;
	
	/** left and right are indices for []surfaces,
	 * meaning that this tree node contains a set of surfaces 
	 * from surfaces[left] to surfaces[right-1]. */
	int begin, end;
	
	/** A comparator class that can sort surfaces by x, y, or z coordinate. */
	static MyComparator cmp = new MyComparator();
	
	
	public AABB(boolean ___, Point3 minBound, Point3 maxBound, AABB leftChild, AABB rightChild, int begin, int end)
	{
		this.minBound = minBound;
		this.maxBound = maxBound;
		
		this.leftChild = leftChild;
		this.rightChild = rightChild;
		
		this.begin = begin;
		this.end = end;
	}
	
	public AABB() {	}
	
	/**
	 * Set the shared static surfaces that every node in the tree will use by 
	 * using left and right indices.
	 */
	public static void setSurfaces(Surface []surfaces) {
		AABB.surfaces = surfaces;
	}
	
	public boolean isLeaf()
	{
		return leftChild == null && rightChild == null;
	}
	
	/**
	 * Create an AABB [sub]tree.  This tree node will be responsible for storing
	 * and processing surfaces[left] to surfaces[right-1].
	 * @param left The left index of []surfaces
	 * @param left The right index of []surfaces
	 */
	public static AABB createTree(int begin, int end)
	{
		Point3 min = new Point3(1,1,1); 
		Point3 max = new Point3(1,1,1);
		
		min.scale(Double.POSITIVE_INFINITY);
		max.scale(Double.NEGATIVE_INFINITY);
		
		for(int i = begin; i < end; ++i) {
			Point3 smin = surfaces[i].getMinBound(), smax = surfaces[i].getMaxBound();
			
			for(int j = 0; j < 3; ++j) {
				double xmin = smin.getE(j), xmax = smax.getE(j);
				
				if(xmin < min.getE(j)) {
					min.setE(j, xmin);
				}
				
				if(xmax > max.getE(j)) {
					max.setE(j, xmax);
				}
			}
		}
		
		AABB leftChild = null, rightChild = null;
		
		// Enough surfaces to justify using the tree
		if(end - begin > 10) {
			int maxDiffIndex = -1;
			double maxDiff = Double.NEGATIVE_INFINITY;
			
			// Find dimension with largest differences
			for(int i = 0; i < 3; ++i) {
				double diff = max.getE(i) - min.getE(i);
				
				if(diff > maxDiff) {
					maxDiff = diff;
					maxDiffIndex = i;
				}
			}
			
			cmp.setIndex(maxDiffIndex);
			
			Arrays.sort(surfaces, begin, end, cmp);
			
			int middle = begin + (end - begin)/2;
			
			leftChild = createTree(begin, middle);
			rightChild = createTree(middle, end);
		}
		
		// Reminder: First parameter is ignored
		return new AABB(false, min, max, leftChild, rightChild, begin, end);
	}

	IntersectionRecord tmp = new IntersectionRecord(), tmp2 = new IntersectionRecord();
	Ray ray = new Ray();
	
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
	public boolean intersect(IntersectionRecord outRecord, Ray rayIn, boolean anyIntersection)
	{
		++ntotal;
		
		if(!Box.intersects(rayIn, minBound, maxBound)) {
			++nfailed;
			
			return false;
		}
		
		// Leaf uses linear search
		if(isLeaf()) {
			boolean ret = false;
			
			ray.set(rayIn.origin, rayIn.direction);
			ray.start = rayIn.start;
			ray.end = rayIn.end;
			
			for(int i = begin; i < end; i++) {
				if(surfaces[i].intersect(tmp, ray) && (tmp.t < ray.end) && (tmp.t > ray.start) ) {
					if(anyIntersection) {
						return true;
					}
					
					ret = true;
					ray.end = tmp.t;
					
					if(outRecord != null) {
						outRecord.set(tmp);
					}
				}
			}
			
			return ret;
		}
		
		// Intersect with leaves
		else {
			boolean ret = false;
			
			ray.set(rayIn.origin, rayIn.direction);
			ray.start = rayIn.start;
			ray.end = rayIn.end;
			
			if(leftChild.intersect(tmp, ray, anyIntersection)) {
				ret = true;
				ray.end = tmp.t;
				
				if(outRecord != null) {
					outRecord.set(tmp);
				}
			}
			
			if(rightChild.intersect(tmp, ray, anyIntersection)) {
				ret = true;
				
				if(outRecord != null) {
					outRecord.set(tmp);
				}
			}
			
			return ret;
		}
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
