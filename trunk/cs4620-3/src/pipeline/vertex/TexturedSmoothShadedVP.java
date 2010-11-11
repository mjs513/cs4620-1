package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.misc.Vertex;

/**
 * This is the same vertex processor as SmoothShadedTP, but allows for
 * texturing in the fragment stage by interpolating texture coordinates.
 * 
 * @author ags
 */
public class TexturedSmoothShadedVP extends SmoothShadedVP {

	/**
	 * @see VertexProcessor#nAttr()
	 */
	public int nAttr() {
		// TODO
		return 0;
	}

	public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output) {
		// TODO
		// NOTE: You don't have to extend SmoothShadedVP to implement this class, but if you figure out how to, your code will be cleaner and less error-prone.
	}

}
