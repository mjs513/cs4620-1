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
		for(Surface s : objs) {
			Matrix4 sm = s.tMat, sminv = s.tMatInv, smtinv = s.tMatTInv;
			
			if(sm == null) {
				sm = s.tMat = new Matrix4();
			}
			
			// Compose: SM = CMAT*SM
			sm.rightCompose(cMat);

			if(sminv == null) {
				sminv = s.tMatInv = new Matrix4();
			}
			
			// Invert SM
			sminv.invert(sm);

			if(smtinv == null) {
				smtinv = s.tMatTInv = new Matrix4();
			}
			
			// Invert the transposed SM
			smtinv.transpose(sm);
			smtinv.invert(smtinv);
		}
		
		computeBoundingBox();
	}
	
	
	public void setTranslate(Vector3 T)
	{ 
		tmp.setTranslate(T);
		transformMat.rightCompose(tmp);
	}
	
	public void setRotate(Point3 R)
	{
		// TODO(A): add rotation to transformMat
	}
	
	public void setScale(Vector3 S)
	{ 
		// TODO(A): add scale to transformMat
	}
	
	public void addSurface(Surface a) {
		objs.add(a);
	}
	
	public boolean intersect(IntersectionRecord outRecord, Ray ray) { return false; }
	public void computeBoundingBox() {	}

	public void appendRenderableSurfaces (ArrayList<Surface> in) {
		for (Iterator<Surface> iter = objs.iterator(); iter.hasNext();)
			iter.next().appendRenderableSurfaces(in);
	}
}
