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

	Vector3 planeN = new Vector3();
	Vector3 v21 = new Vector3(), v31 = new Vector3();
	Vector3 v = new Vector3();
	Vector3 v1 = new Vector3(), v2 = new Vector3(), v3 = new Vector3();
	Point3 p = new Point3();
	
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
	public boolean intersect(IntersectionRecord record, Ray rayIn)
	{
		Ray ray = untransformRay(rayIn);
		Point3 p1 = owner.getVertex(index[0]), p2 = owner.getVertex(index[1]), p3 = owner.getVertex(index[2]);
		
		// planeN = cross(p2 - p1, p3 - p1);   planeP = p1
		v21.sub(p2, p1);
		v31.sub(p3, p1);
		planeN.cross(v21, v31);
		
		int domAxis = -1;
		double maxAxis = Double.NEGATIVE_INFINITY;
		
		for(int i = 0; i < 3; ++i) {
			double x = Math.abs(planeN.getE(i));
			
			if(x > maxAxis) {
				maxAxis = x;
				domAxis = i;
			}
		}

		int i1 = (domAxis + 1)%3, i2 = (domAxis + 2)%3;
		double t;
		
		// t = dot(planeN, planeP - rayP)/dot(planeN, rayD)
		v.sub(p1, ray.origin);
		t = planeN.dot(v)/planeN.dot(ray.direction);
		
		ray.evaluate(p, t);
		
		double a1, a2, a3;
		double p1x = p1.getE(i1), p1y = p1.getE(i2), p2x = p2.getE(i1), p2y = p2.getE(i2);
		double p3x = p3.getE(i1), p3y = p3.getE(i2), px = p.getE(i1), py = p.getE(i2);
		double detT = (p1x - p3x)*(p2y - p3y) - (p2x - p3x)*(p1y - p3y);
		
		a1 = ((p2y - p3y)*(px - p3x) + (p3x - p2x)*(py - p3y))/detT;
		a2 = ((p3y - p1y)*(px - p3x) + (p1x - p3x)*(py - p3y))/detT;
		a3 = 1 - a1 - a2;
		/*
		double a1, a2, a3;
		double detT = (p1.x - p3.x)*(p2.y - p3.y) - (p2.x - p3.x)*(p1.y - p3.y);
		
		a1 = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y))/detT;
		a2 = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y))/detT;
		a3 = 1 - a1 - a2;*/
		
		// Barycentric coordinates must sum 1 and be all positive
		if((a1 < 0) || (a2 < 0) || (a3 < 0)) {
			return false;
		}

		record.surface = this;
		record.t = t;
		
		rayIn.evaluate(record.location, record.t);
		
		// Use model's normals
		if(owner.existsNormals()) {
			Vector3 n1 = owner.getNormal(index[0]), n2 = owner.getNormal(index[1]), n3 = owner.getNormal(index[2]);

			// v[i] = a[i]*normal[i]
			v1.set(n1);
			v1.scale(a1);

			v2.set(n2);
			v2.scale(a2);

			v3.set(n3);
			v3.scale(a3);
			
			// normal = sum(a[i]*normal[i]); i = 0,1,2
			record.normal.set(0, 0, 0);
			record.normal.add(v1);
			record.normal.add(v2);
			record.normal.add(v3);
		}
		
		// Use plane's normal
		else {
			record.normal.set(planeN);
		}
		
		tMatTInv.rightMultiply(record.normal);
		record.normal.normalize();
		
		return true;
	}
	
	public void computeBoundingBox()
	{
		Point3 min = new Point3(1,1,1), max = new Point3(1,1,1);
		
		min.scale(Double.POSITIVE_INFINITY);
		max.scale(Double.NEGATIVE_INFINITY);
		
		for(int i = 0; i < 3; ++i) {
			for(int j = 0; j < 3; ++j) {
				double x = owner.getVertex(index[i]).getE(j);

				if(x < min.getE(j)) {
					min.setE(j, x);
				}

				if(x > max.getE(j)) {
					max.setE(j, x);
				}
			}
		}

		minBound = new Point3();
		maxBound = new Point3();
		averagePosition = new Point3();
		
		Box.boundingBoxOfTransformedBox(min, max, tMat, minBound, maxBound, averagePosition);
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		return "Triangle ";
	}
}