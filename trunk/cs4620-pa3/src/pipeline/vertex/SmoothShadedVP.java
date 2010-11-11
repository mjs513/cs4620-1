package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.Pipeline;
import pipeline.math.Matrix4f;
import pipeline.misc.Vertex;

/**
 * This triangle processor smoothly interpolates the color across the face of
 * the triangle. This is better than flat shading, but still not as nice as
 * fragment (aka phong) shading.
 * 
 * @author ags
 */
public class SmoothShadedVP extends ShadedVP
{
	/** This is the composed modelling, viewing, projection, and viewport matrix. */
	protected Matrix4f m = new Matrix4f();
	
	/**
	 * @see VertexProcessor#nAttr()
	 */
	public int nAttr()
	{
		return 4;
	}

	public void updateTransforms(Pipeline pipe)
	{
		m.set(pipe.modelviewMatrix);
		m.leftCompose(pipe.projectionMatrix);
		m.leftCompose(pipe.viewportMatrix);
	}

	public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
	{
		output.v.set(v.x, v.y, v.z, 1);
		m.rightMultiply(output.v);
		
		output.setAttrs(nAttr());
		output.attrs[0] = c.x;
		output.attrs[1] = c.y;
		output.attrs[2] = c.z;
		output.attrs[3] = v.z;
	}

	/**
	 * @see VertexProcessor#triangle(Vector3f[], Color3f[], Vector3f[],
	 *      Vector2f[], Vertex[])
	 */
	public void triangle(Vector3f[] v, Color3f[] c, Vector3f[] n, Vector2f[] t, Vertex[] output)
	{
		for (int k = 0; k < 3; k++) {
			vertex(v[k], c[k], n[k], t[k], output[k]);
		}
	}


}
