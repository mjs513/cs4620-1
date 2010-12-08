package ray.surface;

import java.util.ArrayList;
import java.util.Iterator;

import ray.IntersectionRecord;
import ray.Ray;
import ray.math.Matrix4;
import ray.math.Point3;
import ray.math.Vector3;

public class Group extends Surface {

	/** List of objects under this group. */
	ArrayList<Surface> objs = new ArrayList<Surface>();
	
	/** The transformation matrix associated with this group. */
	private Matrix4 transformMat;
	
	/** A shared temporary matrix */
	static Matrix4 tmp = new Matrix4();
	
	
	public Group() {
		transformMat = new Matrix4();
		transformMat.setIdentity();
	}
	
	/**
	 * Compute tMat, tMatInv, tMatTInv for this group and propagate values to the children under it.
	 * @param cMat The transformation matrix of the parent for this node.
	 * @param cMatInv The inverse of cMat.
	 * @param cMatTInv The inverse of the transpose of cMat.
	 */
	public void setTransformation(Matrix4 cMat, Matrix4 cMatInv, Matrix4 cMatTInv)
	{
		tMat = new Matrix4();
		tMatInv = new Matrix4();
		tMatTInv = new Matrix4();
		
		cMat.rightMultiply(transformMat, tMat);
		tMatInv.invert(tMat);
		tMatTInv.transpose(tMat);
		tMatTInv.invert();
		
		for(Surface s : objs) {
			Matrix4 m = new Matrix4(), mInv = new Matrix4(), mTInv = new Matrix4();
			
			if(s.tMat != null) {
				m.set(s.tMat);
			}
			
			// Compose surface transformation with this.tMat
			tMat.rightMultiply(m, m);
			
			// Invert surface transformation
			mInv.invert(m);
			
			// Invert the transpose of the surface transformation
			mTInv.transpose(m);
			mTInv.invert();
			
			s.setTransformation(m, mInv, mTInv);
		}
		
		computeBoundingBox();
	}
	
	
	public void setTranslate(Vector3 T)
	{
		tmp.setTranslate(T);
		transformMat.rightCompose(tmp);
	}
	
	private Vector3 v = new Vector3();
	
	public void setRotate(Point3 R)
	{
		v.set(0, 0, 1);
		tmp.setRotate(R.z, v);
		transformMat.rightCompose(tmp);
		
		v.set(0, 1, 0);
		tmp.setRotate(R.y, v);
		transformMat.rightCompose(tmp);
		
		v.set(1, 0, 0);
		tmp.setRotate(R.x, v);
		transformMat.rightCompose(tmp);
	}
	
	public void setScale(Vector3 S)
	{
		tmp.setScale(S);
		transformMat.rightCompose(tmp);
	}
	
	public void addSurface(Surface a) {
		objs.add(a);
	}
	
	public boolean intersect(IntersectionRecord outRecord, Ray ray) { return false; }
	
	public void computeBoundingBox() {	}

	public void appendRenderableSurfaces (ArrayList<Surface> in) {
		for (Iterator<Surface> iter = objs.iterator(); iter.hasNext();) {
			Surface s = iter.next();
			s.appendRenderableSurfaces(in);
		}
	}
}
