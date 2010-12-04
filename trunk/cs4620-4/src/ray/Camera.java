package ray;

import ray.math.Matrix4;
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
	protected final Point3 viewEye = new Point3();
	protected final Vector3 viewFront = new Vector3(0, 0, -1);
	protected final Vector3 viewUp = new Vector3(0, 1, 0);
	protected final Vector3 viewRight = new Vector3(1, 0, 0);
	protected final Vector3 projNormal = new Vector3(0, 0, 1);
	protected double viewWidth = 1.0;
	protected double viewHeight = 1.0;
	protected double projDistance = 1.0;
	protected final Point3 planeTop = new Point3(), planeBottom = new Point3(), planeLeft = new Point3(),
		planeRight = new Point3(), planeCenter = new Point3();
	
	
	public void setViewPoint(Point3 viewPoint) { this.viewEye.set(viewPoint); }
	
	public void setViewDir(Vector3 viewDir) { this.viewFront.set(viewDir); }
	
	public void setViewUp(Vector3 viewUp) { this.viewUp.set(viewUp); }
	
	public void setProjNormal(Vector3 projNormal) { this.projNormal.set(projNormal); }
	
	public void setViewWidth(double viewWidth) { this.viewWidth = viewWidth; }
	
	public void setViewHeight(double viewHeight) { this.viewHeight = viewHeight; }
	
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
	
	// Has the view been initialized?
	protected boolean initialized = false;
	
	/**
	 * Initialize the derived view variables to prepare for using the camera.
	 */
	public void initView()
	{
		viewUp.normalize();
		viewFront.normalize();
		viewRight.cross(viewFront, viewUp);

		double fovx = Math.tan(0.5*viewWidth/projDistance);
		double fovy = Math.tan(0.5*viewHeight/projDistance);
		double lengthx = 1/Math.cos(fovx), lengthy = 1/Math.cos(fovy);
		
		Matrix4 ry = new Matrix4();
		Vector3 v = new Vector3();
		
		// top = Rotation(fovy, right)*front*lengthy + eye
		ry.setRotate(fovy, viewRight);
		v.set(viewFront);
		ry.rightMultiply(v);
		v.scale(lengthy);
		planeTop.set(viewEye);
		planeTop.add(v);
		
		// bottom = Rotation(-fovy, right)*front*lengthy + eye
		ry.setRotate(-fovy, viewRight);
		v.set(viewFront);
		ry.rightMultiply(v);
		v.scale(lengthy);
		planeBottom.set(viewEye);
		planeBottom.add(v);
		
		// left = Rotation(-fovx, up)*front*lengthx + eye
		ry.setRotate(-fovy, viewRight);
		v.set(viewFront);
		ry.rightMultiply(v);
		v.scale(lengthx);
		planeLeft.set(viewEye);
		planeLeft.add(v);
		
		// right = Rotation(fovx, up)*front*lengthx + eye
		ry.setRotate(fovy, viewRight);
		v.set(viewFront);
		ry.rightMultiply(v);
		v.scale(lengthx);
		planeRight.set(viewEye);
		planeRight.add(v);
		
		// center = eye + front
		planeCenter.set(viewEye);
		planeCenter.add(viewFront);

		System.out.println("w: " + viewWidth);
		System.out.println("h: " + viewHeight);
		System.out.println("d: " + projDistance);
		System.out.println("up: " + viewUp);
		System.out.println("front: " + viewFront);
		System.out.println("right: " + viewRight);
		System.out.println("eye: " + viewEye);
		System.out.println("fovx: " + fovx);
		System.out.println("fovy: " + fovy);
		System.out.println("lengthx: " + lengthx);
		System.out.println("lengthy: " + lengthy);
		System.out.println("planeTop: " + planeTop);
		System.out.println("planeBottom: " + planeBottom);
		System.out.println("planeLeft: " + planeLeft);
		System.out.println("planeRight: " + planeRight);
		System.out.println("planeCenter: " + planeCenter);
		
		initialized = true;
	}

	private Ray ray = new Ray();
	private Point3 px = new Point3(), py = new Point3(), p = new Point3();
	private Vector3 v = new Vector3();
	
	public Ray getRay(double pctx, double pcty)
	{
		if(!initialized) {
			initView();
		}
		
		// px = planeLeft + (planeRight - planeLeft)*pctx
		px.set(planeLeft);
		v.sub(planeRight, planeLeft);
		v.scale(pctx);
		px.add(v);
		
		// py = planeBottom + (planeTop - planeBottom)*pcty
		px.set(planeBottom);
		v.sub(planeTop, planeBottom);
		v.scale(pcty);
		px.add(v);

		// p = planeCenter + (px - planeCenter) + (py - planeCenter)
		p.set(planeCenter);
		v.sub(px, planeCenter);
		p.add(v);
		v.sub(py, planeCenter);
		p.add(v);
		
		// dir = p - eye
		v.sub(p, viewEye);
		
		ray.set(viewEye, v);
		
		//System.out.println(pctx + ", " + pcty + "; ray dir length: " + v.length() + ", dir: " + v);
		
		return ray;
	}
	
	/**
	 * Set outRay to be a ray from the camera through a point in the image.
	 *
	 * @param outRay The output ray (not normalized)
	 * @param inU The u coord of the image point (range [0,1])
	 * @param inV The v coord of the image point (range [0,1])
	 */
	public void getRay(Ray outRay, double inU, double inV)
	{
		if(!initialized) {
			initView();
		}
		
		
		// TODO(A): fill in this function.
		// Remap to UV coordinates and set the output ray
		
		
	}
}
