package pipeline.vertex;

import java.util.Vector;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import pipeline.Pipeline;
import pipeline.math.Matrix4f;
import pipeline.misc.Vertex;

/**
 * Passes the normals and the colors of each vertex on to be rasterized, and
 * later shaded during the fragment stage of the pipeline. This results in the
 * highest quality images, but results in costly computation.
 * 
 * @author ags
 */
public class FragmentShadedVP extends VertexProcessor
{
	protected Matrix4f m = new Matrix4f();
	protected Matrix4f modelview = new Matrix4f();
	protected Vector<Vector4f> lightSources;
	protected Vector<Color3f> lightColors;

	Vector4f v2 = new Vector4f();
	Vector4f n2 = new Vector4f();
	
	
	public int nAttr()
	{
		// 3 for color + 3 for position + 3 for normal
		return 9;
	}

	public void updateTransforms(Pipeline pipe)
	{
		modelview.set(pipe.modelviewMatrix);
		
		m.set(pipe.modelviewMatrix);
		m.leftCompose(pipe.projectionMatrix);
		m.leftCompose(pipe.viewportMatrix);
	}
	
	public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
	{
		// Position of vextex in eye coordinates
		v2.set(v.x, v.y, v.z, 1);
		modelview.rightMultiply(v2);
		
		// Normal in eye coordinates
		n2.set(n.x, n.y, n.z, 0);
		modelview.rightMultiply(n2);
		
		output.v.set(v.x, v.y, v.z, 1);
		m.rightMultiply(output.v);
		
		output.setAttrs(nAttr());
		
		// Colors
		output.attrs[0] = c.x;
		output.attrs[1] = c.y;
		output.attrs[2] = c.z;
		
		// Point
		output.attrs[3] = v2.x;
		output.attrs[4] = v2.y;
		output.attrs[5] = v2.z;
		
		// Normal
		output.attrs[6] = n2.x;
		output.attrs[7] = n2.y;
		output.attrs[8] = n2.z;
	}
	
	public void triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs)
	{
		for (int k = 0; k < 3; k++) {
			vertex(vs[k], cs[k], ns[k], ts[k], outputs[k]);
		}
	}
}
