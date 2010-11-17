package pipeline.fragment;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;

import pipeline.misc.Fragment;
import pipeline.misc.FrameBuffer;

/**
 * This FP works just like the PhongShadedFP, but also incorporates a texture.
 * 
 * @author ags
 */
public class TexturedPhongFP extends PhongShadedFP
{
	// Texture color
	Color3f tc = new Color3f();
	
	
	public int nAttr()
	{
		// 3 for color + 3 for position + 3 for normal + 2 for texture
		return 11;
	}
	
	public void fragment(Fragment f, FrameBuffer fb)
	{
		float z = f.attrs[0];
		
		if(z >= fb.getZ(f.x, f.y)) {
		    return;
		}
		
		// Do not combine with fragment color
		f.attrs[1] = f.attrs[2] = f.attrs[3] = 1;

		Color3f c = colorForFragment(f);
		
		texture.sample(new Vector2f(f.attrs[10], f.attrs[11]), tc);
		
		fb.set(f.x, f.y, tc.x*c.x, tc.y*c.y, tc.z*c.z, z);
	}
}
