package pipeline.vertex;

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
public class FragmentShadedVP extends VertexProcessor {
  /**
   * @see VertexProcessor#nAttr()
   */
  public int nAttr() {
	  // TODO
	  return 0;
  }

  /**
   * @see VertexProcessor#updateTransforms(Pipeline)
   */
  public void updateTransforms(Pipeline pipe) {
	  // TODO
  }

  public void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output) {
	  // TODO
  }

  /**
   * @see VertexProcessor#triangle(Vector3f[], Color3f[], Vector3f[],
   *      Vector2f[], Vertex[])
   */
  public void triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs) {

    for (int k = 0; k < 3; k++) {
      vertex(vs[k], cs[k], ns[k], null, outputs[k]);
    }
  }

}
