package ray.surface;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Point3;
import ray.math.Vector3;

public class Box extends Surface {
	
	/* The corner of the box with the smallest x, y, and z components. */
	protected final Point3 minPt = new Point3();
	public void setMinPt(Point3 minPt) { this.minPt.set(minPt); }
	
	/* The corner of the box with the largest x, y, and z components. */
	protected final Point3 maxPt = new Point3();
	public void setMaxPt(Point3 maxPt) { this.maxPt.set(maxPt); }

	Vector3 tmin = new Vector3(), tmax = new Vector3();
	Vector3 tfirst = new Vector3(), tsecond = new Vector3();
	Vector3 inv = new Vector3();
	
	/**
	 * Tests this surface for intersection with ray. If an intersection is found
	 * record is filled out with the information about the intersection and the
	 * method returns true. It returns false otherwise and the information in
	 * outRecord is not modified.
	 *
	 * @param record the output IntersectionRecord
	 * @param ray the ray to intersect
	 * @return true if the surface intersects the ray
	 */
	public boolean intersect(IntersectionRecord record, Ray rayIn)
	{
		Ray ray = untransformRay(rayIn);

		// t[i] = (planeP[i] - rayP[i])/rayD[i]
		tmin.sub(minPt, ray.origin);
		tmax.sub(maxPt, ray.origin);
		
		tmin.x /= ray.direction.x;
		tmin.y /= ray.direction.y;
		tmin.z /= ray.direction.z;
		
		tmax.x /= ray.direction.x;
		tmax.y /= ray.direction.y;
		tmax.z /= ray.direction.z;
		
		// Order intervals
		for(int i = 0; i < 3; ++i) {
			double tminv = tmin.getE(i), tmaxv = tmax.getE(i);

			if(tminv < tmaxv) {
				tfirst.setE(i, tminv);
				tsecond.setE(i, tmaxv);
			
				// First normal is from min plane
				inv.setE(i, -1);
			}
			else {
				tsecond.setE(i, tminv);
				tfirst.setE(i, tmaxv);
				
				// First normal is from max plane
				inv.setE(i, 1);
			}
		}
		
		double maxtfirst = Double.NEGATIVE_INFINITY, mintsecond = Double.POSITIVE_INFINITY;
		int firstAxis = -1, secondAxis = -1;
		
		// Intersect all t ranges found
		for(int i = 0; i < 3; ++i) {
			double tfv = tfirst.getE(i), tsv = tsecond.getE(i);
			
			if(tfv > maxtfirst) {
				maxtfirst = tfv;
				firstAxis = i;
			}
			
			if(tsv < mintsecond) {
				mintsecond = tsv;
				secondAxis = i;
			}
		}
		
		if((maxtfirst > mintsecond) || (mintsecond < 0)) {
			return false;
		}
		
		record.t = maxtfirst;
		record.surface = this;
		
		rayIn.evaluate(record.location, record.t);
		
		record.normal.set(0, 0, 0);
		record.normal.setE(firstAxis, inv.getE(firstAxis));

		return true;
	}

	public void computeBoundingBox()
	{
		// TODO(B): Compute the bounding box and store the result in
		// averagePosition, minBound, and maxBound.
		// Hint: The bounding box is not the same as just minPt and maxPt, because 
		// this object can be transformed by a transformation matrix.
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		return "Box ";
	}
}

