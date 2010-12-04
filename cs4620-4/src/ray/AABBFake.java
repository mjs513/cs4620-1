package ray;

import java.util.Arrays;
import java.util.Comparator;
import ray.math.Point3;
import ray.math.Vector3;
import ray.surface.Surface;
/**
 * Provide a fake AABB that performs a linear search.
 * 
 * @author ss932
 */
public class AABBFake {
	
	/** A shared surfaces array that will be used across every node in the tree */
	static Surface []surfaces;
	
	/** left and right are indices for []surfaces,
	 * meaning that this tree node contains a set of surfaces 
	 * from surfaces[left] to surfaces[right-1]. */
	int left, right;

	public AABBFake(int left, int right) {
		this.left = left;
		this.right = right;
	}
	public AABBFake() {	}
	
	/**
	 * Set the shared static surfaces that every node in the tree will use by 
	 * using left and right indices.
	 */
	public static void setSurfaces(Surface []surfaces) {
		AABBFake.surfaces = surfaces;
	}
	
	public static AABBFake createTree(int left, int right) {
		return new AABBFake(left, right);
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
		boolean ret = false;
		IntersectionRecord tmp = new IntersectionRecord();
		Ray ray = new Ray(rayIn.origin, rayIn.direction);
		ray.start = rayIn.start;
		ray.end = rayIn.end;
		for(int i = left; i < right; i++) {
			if(surfaces[i].intersect(tmp, ray) && tmp.t < ray.end ) {
				if(anyIntersection) return true;
				ret = true;
				ray.end = tmp.t;
				if(outRecord != null)
					outRecord.set(tmp);
			}
		}
		return ret;
	}
}

