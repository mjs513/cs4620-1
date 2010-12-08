package ray.surface;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Matrix4;
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
		minBound = new Point3();
		maxBound = new Point3();
		averagePosition = new Point3();
		
		boundingBoxOfTransformedBox(minPt, maxPt, tMat, minBound, maxBound, averagePosition);
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		return "Box ";
	}
	
	public static void boundingBoxOfTransformedBox(Point3 boxMin, Point3 boxMax, Matrix4 m, Point3 outMin,
			Point3 outMax, Point3 outAvg)
	{
		Point3[] minmax = { boxMin, boxMax };
		
		outMin.set(1,1,1);
		outMin.scale(Double.POSITIVE_INFINITY);
		
		outMax.set(1,1,1);
		outMax.scale(Double.NEGATIVE_INFINITY);
		
		// Transform each point of the cube and get axis aligned min and max values
		for(int i = 0; i <= 1; ++i) {
			for(int j = 0; j <= 1; ++j) {
				for(int k = 0; k <= 1; ++k) {
					Point3 p = new Point3(minmax[i].x, minmax[j].y, minmax[k].z);
					
					m.rightMultiply(p);
					
					for(int l = 0; l < 3; ++l) {
						double x = p.getE(l);
						
						if(x < outMin.getE(l)) {
							outMin.setE(l, x);
						}
						
						if(x > outMax.getE(l)) {
							outMax.setE(l, x);
						}
					}
				}
			}
		}
		
		Vector3 diff = new Vector3();
		
		// avg = min + (max - min)/2
		diff.sub(outMax, outMin);
		diff.scale(0.5);
		outAvg = new Point3(outMin);
		outAvg.add(diff);
	}

	private static Vector3 stmin = new Vector3(), stmax = new Vector3();
	private static Vector3 stfirst = new Vector3(), stsecond = new Vector3();
	private static Vector3 sinv = new Vector3();
	
	public static boolean intersects(Ray ray, Point3 minPt, Point3 maxPt)
	{
		// t[i] = (planeP[i] - rayP[i])/rayD[i]
		stmin.sub(minPt, ray.origin);
		stmax.sub(maxPt, ray.origin);
		
		stmin.x /= ray.direction.x;
		stmin.y /= ray.direction.y;
		stmin.z /= ray.direction.z;
		
		stmax.x /= ray.direction.x;
		stmax.y /= ray.direction.y;
		stmax.z /= ray.direction.z;
		
		// Order intervals
		for(int i = 0; i < 3; ++i) {
			double tminv = stmin.getE(i), tmaxv = stmax.getE(i);

			if(tminv < tmaxv) {
				stfirst.setE(i, tminv);
				stsecond.setE(i, tmaxv);
			
				// First normal is from min plane
				sinv.setE(i, -1);
			}
			else {
				stsecond.setE(i, tminv);
				stfirst.setE(i, tmaxv);
				
				// First normal is from max plane
				sinv.setE(i, 1);
			}
		}
		
		double maxtfirst = Double.NEGATIVE_INFINITY, mintsecond = Double.POSITIVE_INFINITY;
		
		// Intersect all t ranges found
		for(int i = 0; i < 3; ++i) {
			double tfv = stfirst.getE(i), tsv = stsecond.getE(i);
			
			if(tfv > maxtfirst) {
				maxtfirst = tfv;
			}
			
			if(tsv < mintsecond) {
				mintsecond = tsv;
			}
		}
		
		return !((maxtfirst > mintsecond) || (mintsecond < 0));
	}
}

