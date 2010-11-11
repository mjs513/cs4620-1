package pipeline.fragment;

import pipeline.misc.Fragment;
import pipeline.misc.FrameBuffer;

/**
 * This fragment processor will place the indicated color into the framebuffer
 * only if the fragment passes the z buffer test (ie - it isn't occluded by
 * another fragment).
 * 
 * @author ags
 */
public class ColorZBufferFP extends FragmentProcessor
{
	public int nAttr()
	{
		return 3;
	}
	
	public void fragment(Fragment f, FrameBuffer fb)
	{
		float z = f.attrs[0];
		
		if(z < fb.getZ(f.x, f.y)) {
		    fb.set(f.x, f.y, f.attrs[1], f.attrs[2], f.attrs[3], z);
		}
	}
}
