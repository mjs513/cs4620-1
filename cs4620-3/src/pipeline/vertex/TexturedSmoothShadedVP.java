package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.misc.Vertex;

/**
 * This is the same vertex processor as SmoothShadedVP, but allows for
 * texturing in the fragment stage by interpolating texture coordinates.
 * 
 * @author ags
 */
public class TexturedSmoothShadedVP extends SmoothShadedVP
{
	public int nAttr()
	{
		// 3 for color + 2 for texture
		return 5;
	}
	
	public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
	{
		super.vertex(v, c, n, null, output);

		output.attrs[3] = t.x;
		output.attrs[4] = t.y;
	}
}
