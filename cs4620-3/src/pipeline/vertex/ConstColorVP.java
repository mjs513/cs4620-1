package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.Pipeline;
import pipeline.math.Matrix4f;
import pipeline.misc.Vertex;

/**
 * Sets each vertex up with a constant color, no matter what the viewing
 * conditions.
 * 
 * @author ags
 */
public class ConstColorVP extends VertexProcessor {

	/** This is the composed modelling, viewing, projection, and viewport matrix. */
	protected Matrix4f m = new Matrix4f();

	/**
	 * @see VertexProcessor#nAttr()
	 */
	public int nAttr() {

		return 3;
	}

	/**
	 * @see VertexProcessor#updateTransforms(Pipeline)
	 */
	public void updateTransforms(Pipeline pipe) {

		m.set(pipe.modelviewMatrix);
		m.leftCompose(pipe.projectionMatrix);
		m.leftCompose(pipe.viewportMatrix);
	}

	/**
	 * @see VertexProcessor#triangle(Vector3f[], Color3f[], Vector3f[],
	 *      Vector2f[], Vertex[])
	 */
	public void triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns_ign, Vector2f[] ts_ign, Vertex[] output) {

		for (int k = 0; k < 3; k++) {
			vertex(vs[k], cs[k], null, null, output[k]);
		}
	}

	public void vertex(Vector3f v, Color3f c, Vector3f n_ign, Vector2f t_ign, Vertex output) {
		output.v.set(v.x, v.y, v.z, 1);
		m.rightMultiply(output.v);

		output.setAttrs(nAttr());
		output.attrs[0] = c.x;
		output.attrs[1] = c.y;
		output.attrs[2] = c.z;
	}

}