package ray.surface;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Point3;
import ray.math.Vector3;

/**
 * Represents a sphere as a center and a radius.
 *
 * @author ags
 */
public class Sphere extends Surface
{
	/** The center of the sphere. */
	protected final Point3 center = new Point3();
	public void setCenter(Point3 center) { this.center.set(center); }
	
	/** The radius of the sphere. */
	protected double radius = 1.0;
	public void setRadius(double radius) { this.radius = radius; }
	
	public Sphere() { }

	Vector3 v = new Vector3();
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
	public boolean intersect(IntersectionRecord outRecord, Ray rayIn)
	{
		Ray ray = untransformRay(rayIn);
		double A, B, C;
		
		// A = dot(rayD, rayD)
		v.set(ray.direction);
		A = v.dot(v);
		
		// B = 2*dot(rayD, rayP - center)
		v.sub(ray.origin, center);
		B = 2*ray.direction.dot(v);
		
		// C = dot(rayP - center, rayP - center) - R*R
		C = v.dot(v) - radius*radius;
		
		double discr = B*B - 4*A*C;
		
		if(discr < 0) {
			return false;
		}
		
		discr = Math.sqrt(discr);
		
		double t2 = (-B - discr)/(2*A), t1 = (-B + discr)/(2*A);
		
		// t2 positive and before t1
		if((t2 > 0) && (t2 < t1)) {
			t1 = t2;
		}
		
		// Intersection must be positive
		if(t1 < 0) {
			return false;
		}

		outRecord.t = t1;
		rayIn.evaluate(outRecord.location, outRecord.t);
		
		outRecord.surface = this;
		
		ray.evaluate(p, t1);
		
		// n = p - center   (in object coords(
		v.sub(p, center);
		v.normalize();
		
		outRecord.normal.set(v);
		
		// Transform normal to world coords and normalize
		tMatTInv.rightMultiply(outRecord.normal);
		outRecord.normal.normalize();
		
		return true;
	}
	
	public void computeBoundingBox()
	{
		Point3 min = new Point3(center), max = new Point3(center);
		Vector3 v = new Vector3(radius, radius, radius);

		// min = center - (R,R,R)
		// max = center + (R,R,R)
		min.sub(v);
		max.add(v);
		
		minBound = new Point3();
		maxBound = new Point3();
		averagePosition = new Point3();
		
		Box.boundingBoxOfTransformedBox(min, max, tMat, minBound, maxBound, averagePosition);
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		return "sphere " + center + " " + radius + " " + shader + " end";
	}

}