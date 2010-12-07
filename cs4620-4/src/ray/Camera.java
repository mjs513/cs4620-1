package ray;

import ray.math.Point3;
import ray.math.Vector3;

/**
 * Represents a simple camera.
 */
public class Camera {
	
	/*
	 * Fields that are read in from the input file to describe the camera.
	 * You'll probably want to store some derived values to make ray generation easy.
	 */
	protected final Point3 viewPoint = new Point3();
	public void setViewPoint(Point3 viewPoint) { this.viewPoint.set(viewPoint); }
	
	protected final Vector3 viewDir = new Vector3(0, 0, -1);
	public void setViewDir(Vector3 viewDir) { this.viewDir.set(viewDir); }
	
	protected final Vector3 viewUp = new Vector3(0, 1, 0);
	public void setViewUp(Vector3 viewUp) { this.viewUp.set(viewUp); }
	
	protected final Vector3 projNormal = new Vector3(0, 0, 1);
	public void setProjNormal(Vector3 projNormal) { this.projNormal.set(projNormal); }
	
	protected double viewWidth = 1.0;
	public void setViewWidth(double viewWidth) { this.viewWidth = viewWidth; }
	
	protected double viewHeight = 1.0;
	public void setViewHeight(double viewHeight) { this.viewHeight = viewHeight; }
	
	protected double projDistance = 1.0;
	public void setprojDistance(double projDistance) { this.projDistance = projDistance; }
	
	
	
	/*
	 * Derived values that are computed before ray generation.
	 * basisU, basisV, and basisW form an orthonormal basis.
	 * basisW is parallel to projNormal.
	 */
	protected final Vector3 basisU = new Vector3();
	protected final Vector3 basisV = new Vector3();
	protected final Vector3 basisW = new Vector3();
	protected final Vector3 centerDir = new Vector3();
	protected final Vector3 projCenter = new Vector3();
	
	// Has the view been initialized?
	protected boolean initialized = false;
	
	/**
	 * Initialize the derived view variables to prepare for using the camera.
	 */
	public void initView()
	{
		viewUp.normalize();
		viewDir.normalize();
		
		Vector3 v = new Vector3();
		Point3 planeP = new Point3();
		
		// planeP = eye + front*d
		planeP.set(viewPoint);
		v.set(viewDir);
		v.scale(projDistance);
		planeP.add(v);
		
		Vector3 planeN = new Vector3();
		
		planeN.set(projNormal);
		planeN.normalize();
		
		// v = project(up, planeP) = up - project(up, planeN) = up - planeN*dot(up, planeN)   >> planeN is normalized
		basisV.set(viewUp);
		v.set(planeN);
		v.scale(viewUp.dot(planeN));
		basisV.sub(v);
		basisV.normalize();
		basisV.scale(viewHeight);
		
		// w = planeN
		basisW.set(planeN);
		
		// u = cross(v, w)
		basisU.cross(basisV, basisW);
		basisU.normalize();
		basisU.scale(viewWidth);
		
		projCenter.set(planeP);

		initialized = true;
	}

	private Point3 p = new Point3();
	private Vector3 v = new Vector3();
	
	/**
	 * Set outRay to be a ray from the camera through a point in the image.
	 *
	 * @param outRay The output ray (not normalized)
	 * @param inU The u coord of the image point (range [-0.5, 0.5])
	 * @param inV The v coord of the image point (range [-0.5, 0.5])
	 */
	public void getRay(Ray ray, double inU, double inV)
	{
		if(!initialized) {
			initView();
		}
		
		ray.origin.set(viewPoint);
		
		// p = projCenter + inU*u + inV*v
		p.set(projCenter);
		v.set(basisU);
		v.scale(inU);
		p.add(v);
		v.set(basisV);
		v.scale(inV);
		p.add(v);
		
		ray.direction.sub(p, ray.origin);
		ray.direction.normalize();
		ray.start = 0;
		ray.end = Double.POSITIVE_INFINITY;
	}
}
