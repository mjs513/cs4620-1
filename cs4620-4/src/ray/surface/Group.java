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
		// TODO(A): Compute tMat, tMatInv, tMatTInv using transformMat.
        // Hint: We apply the transformation from bottom up the tree. 
		// i.e. The child's transformation will be applied to objects before its parent's.
		
		tMat = new Matrix4();
		tMatInv = new Matrix4();
		tMatTInv = new Matrix4();
		
		cMat.rightMultiply(transformMat, tMat);
		tMatInv.invert(tMat);
		tMatTInv.transpose(tMat);
		tMatTInv.invert();
		
		// TODO: implementei aqui... não sei o que eu fiz direito
		for(Surface s : objs) {
			
			if(s.tMat == null) {
				s.tMat = new Matrix4();
			}
			
			if(s.tMatInv == null) {
				s.tMatInv = new Matrix4();
			}

			if(s.tMatTInv == null) {
				s.tMatTInv = new Matrix4();
			}
			
			// Compose: s.tMat = cMat*transformMat*s.tMat
			tMat.rightMultiply(s.tMat, s.tMat);

			// Invert SM
			s.tMatInv.invert(s.tMat);

			// Invert the transposed SM
			s.tMatTInv.transpose(s.tMat);
			s.tMatTInv.invert(s.tMatTInv);
		}
		
		computeBoundingBox();
	}
	
	
	public void setTranslate(Vector3 T)
	{
		// TODO: esse já tava implementado assim
		tmp.setTranslate(T);
		transformMat.rightCompose(tmp);
	}
	
	private Vector3 v = new Vector3();
	
	public void setRotate(Point3 R)
	{
		// TODO(A): add rotation to transformMat
		// implementei esse como no setTranslate

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
		// TODO(A): add scale to transformMat
		// TODO: implementei esse como no setTranslate
		tmp.setScale(S);
		transformMat.rightCompose(tmp);
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
