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
public class TexturedFragmentShadedVP extends FragmentShadedVP {

  /**
   * @see VertexProcessor#nAttr()
   */
  public int nAttr() {
	  // TODO
	  return 0;
  }

  public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output) {
	  // TODO
	  // NOTE: You don't have to extend FragmentShadedVP to implement this class, but if you figure out how to, your code will be cleaner and less error-prone.
  }

  /**
   * @see VertexProcessor#triangle(Vector3f[], Color3f[], Vector3f[],
   *      Vector2f[], Vertex[])
   */
  public void triangle(Vector3f[] vs, Color3f[] cs_ign, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs) {

    for (int k = 0; k < 3; k++) {
      vertex(vs[k], null, ns[k], ts[k], outputs[k]);
    }
  }

}
