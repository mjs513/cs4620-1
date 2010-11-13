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
public class TexturedFP extends FragmentProcessor
{
	// Texture color
	Color3f tc = new Color3f();
	
	
	public int nAttr()
	{
		// 3 for color + 2 for texture
		return 5;
	}
	
	public void fragment(Fragment f, FrameBuffer fb)
	{
		float z = f.attrs[0];
		
		if(z >= fb.getZ(f.x, f.y)) {
			return;
		}
		
		texture.sample(new Vector2f(f.attrs[4], f.attrs[5]), tc);
		
		// Set color as texture times frament color
		fb.set(f.x, f.y, tc.x*f.attrs[1], tc.y*f.attrs[2], tc.z*f.attrs[3], z);
	}
}
