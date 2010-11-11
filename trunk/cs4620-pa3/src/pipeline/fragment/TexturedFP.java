package pipeline.fragment;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;

import pipeline.misc.Fragment;
import pipeline.misc.FrameBuffer;

/**
 * This FP does a texture lookup rather to determine the color of a fragment. It
 * also uses the z-buffering technique to draw the correct fragment.
 * 
 * @author ags
 */
public class TexturedFP extends FragmentProcessor {

  public int nAttr() {
	  // TODO
	  return 0;
  }

  public void fragment(Fragment f, FrameBuffer fb) {
	  // TODO
  }

}
