package ray.surface;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Point3;
import ray.math.Vector3;
import ray.shader.Shader;

/**
 * Represents a sphere as a center and a radius.
 *
 * @author ags
 */
public class TriangleMesh extends Surface {
	/** The normal vector of this triangle mesh */
	Vector3 norm;
	
	/** The mesh that contains this triangle mesh */
	Mesh owner;
	
	/** 3 indices to the vertices of this triangle. */
	int []index;
	
	public TriangleMesh(Mesh owner, int index0, int index1, int index2, Shader material) {
		this.owner = owner;
		index = new int[3];
		index[0] = index0;
		index[1] = index1;
		index[2] = index2;
		
		Point3 v0 = owner.getVertex(index0);
		Point3 v1 = owner.getVertex(index1);
		Point3 v2 = owner.getVertex(index2);
		
		if(!owner.existsNormals()) {
			Vector3 e0 = new Vector3(), e1 = new Vector3();
			e0.sub(v1, v0);
			e1.sub(v2, v0);
			norm = new Vector3();
			norm.cross(e0, e1);
		}
				
		this.setShader(material);
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
	public boolean intersect(IntersectionRecord outRecord, Ray rayIn)
	{
		// intersect with plane
		// find dominant axis of normal
		// project points of triangle to plane that has normal = 1 where domaxis, 0 else
		//   (do by zeroing the dom axis)
		// 
		
		// TODO(B): fill in this function.
		// Hint: This object can be transformed by a transformation matrix.
		// So the rayIn needs to be processed so that it is in the same coordinate as the object.
		
		return false;
	}

	public void computeBoundingBox()
	{
		// TODO(B): Compute the bounding box and store the result in
		// averagePosition, minBound, and maxBound.
	}

	/**
	 * @see Object#toString()
	 */
	public String toString() {
		return "Triangle ";
	}
}