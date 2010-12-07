package ray.surface;

import java.util.ArrayList;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Matrix4;
import ray.math.Point3;
import ray.shader.Shader;

/**
 * Abstract base class for all surfaces.  Provides access for shader and
 * intersection uniformly for all surfaces.
 *
 * @author ags, ss932
 */
public abstract class Surface {
	
	/* tMat, tMatInv, tMatTInv are calculated and stored in each instance to avoid recomputing */
	
	/** The transformation matrix. */
	protected Matrix4 tMat;
	
	/** The inverse of the transformation matrix. */
	protected Matrix4 tMatInv;
	
	/** The inverse of the transpose of the transformation matrix. */
	protected Matrix4 tMatTInv;
	
	/** The average position of the surface. Usually calculated by taking the average of 
	 * all the vertices. This point will be used in AABB tree construction. */
	protected Point3 averagePosition;
	
	/** The smaller coordinate (x, y, z) of the bounding box of this surface */
	protected Point3 minBound;
	
	/** The larger coordinate (x, y, z) of the bounding box of this surface */
	protected Point3 maxBound; 

	/** Shader to be used to shade this surface. */
	protected Shader shader = Shader.DEFAULT_MATERIAL;
	public void setShader(Shader material) { this.shader = material; }
	public Shader getShader() { return shader; }
	
	public Point3 getAveragePosition() { return averagePosition; } 
	public Point3 getMinBound() { return minBound; }
	public Point3 getMaxBound() { return maxBound; }

	private Ray ray = new Ray();
	
	/**
	 * Un-transform rayIn using tMatInv 
	 * @param rayIn Input ray
	 * @return tMatInv * rayIn
	 */
	public Ray untransformRay(Ray rayIn)
	{
		ray.set(rayIn.origin, rayIn.direction);
		
		ray.start = rayIn.start;
		ray.end = rayIn.end;

		tMatInv.rightMultiply(ray.direction);
		tMatInv.rightMultiply(ray.origin);
		return ray;
	}
	
	public void setTransformation(Matrix4 a, Matrix4 aInv, Matrix4 aTInv) {
		tMat = a;
		tMatInv = aInv;
		tMatTInv = aTInv;
		computeBoundingBox();
	}
	
	/**
	 * Tests this surface for intersection with ray. If an intersection is found
	 * record is filled out with the information about the intersection and the
	 * method returns true. It returns false otherwise and the information in
	 * outRecord is not modified.
	 *
	 * @param outRecord the output IntersectionRecord
	 * @param ray the ray to intersect
	 * @return true if the surface intersects the ray
	 */
	public abstract boolean intersect(IntersectionRecord outRecord, Ray ray);

	/**
	 * Compute the bounding box and store the result in
	 * averagePosition, minBound, and maxBound.
	 */
	public abstract void computeBoundingBox();
	
	/**
	 * Add this surface to the array list in. This array list will be used
	 * in the AABB tree construction.
	 */
	public void appendRenderableSurfaces(ArrayList<Surface> in) {
		in.add(this);
	}
}
