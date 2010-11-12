package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.misc.Vertex;

/**
 * This is the same triangle processor as FragmentShadedTP, but it also
 * interpolates texture coordinates, allowing for texturing to be done in the
 * fragment stage.
 * 
 * @author ags
 */
public class TexturedFragmentShadedVP extends FragmentShadedVP
{
	public int nAttr()
	{
		// 3 for color + 3 for position + 3 for normal + 2 for texture
		return 11;
	}
	
	public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
	{
		super.vertex(v, c, n, t, output);

		output.attrs[9] = t.x;
		output.attrs[10] = t.y;
	}
}
