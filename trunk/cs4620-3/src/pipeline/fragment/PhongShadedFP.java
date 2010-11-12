package pipeline.fragment;

import javax.vecmath.Color3f;
import javax.vecmath.Vector4f;

import pipeline.Pipeline;
import pipeline.PointLight;
import pipeline.misc.Fragment;
import pipeline.misc.FrameBuffer;

/**
 * This is the fragment program which actually uses a shading model to compute
 * the color on a per fragment basis.
 * 
 * @author ags
 */
public class PhongShadedFP extends FragmentProcessor
{
	public int nAttr()
	{
		// 3 for color + 3 for position + 3 for normal
		return 9;
	}
	
	protected Color3f colorForFragment(Fragment f)
	{
		Color3f c = new Color3f(f.attrs[1], f.attrs[2], f.attrs[3]);
		Vector4f p = new Vector4f(f.attrs[4], f.attrs[5], f.attrs[6], 1);
		Vector4f n = new Vector4f(f.attrs[7], f.attrs[8], f.attrs[9], 0);
		Color3f c2 = new Color3f(Pipeline.ambientIntensity*c.x, Pipeline.ambientIntensity*c.y, Pipeline.ambientIntensity*c.z);
		
		n.normalize();
		
		// For each light, get the contribution to this fragment
		for(PointLight light : Pipeline.lights) {
			Vector4f l = new Vector4f(light.getPosition());
			Color3f lc = light.getIntensity();
			
			// Vector from vertex to light source -- normalized
			l.sub(p);
			l.normalize();
			
			float dot = l.dot(n);

			// Diffuse contribution
			if(dot > 0) {
				c2.x += c.x*dot*lc.x;
				c2.y += c.y*dot*lc.y;
				c2.z += c.z*dot*lc.z;
			}
			
			// Half vector for specular lighting
			Vector4f h = new Vector4f(l);
			
			h.add(n);
			h.normalize();
			
			dot = h.dot(l);
			
			// Specular contribution
			if(dot > 0) {
				dot = (float) Math.pow(dot, Pipeline.specularExponent);
				
				c2.x += dot*Pipeline.specularColor.x*lc.x;
				c2.y += dot*Pipeline.specularColor.y*lc.y;
				c2.z += dot*Pipeline.specularColor.z*lc.z;
			}
		}
		
		// Clamp rgb to [0,1]
		if(c2.x > 1) {
			c2.x = 1;
		}
		if(c2.y > 1) {
			c2.y = 1;
		}
		if(c2.z > 1) {
			c2.z = 1;
		}
		
		return c2;
	}

	public void fragment(Fragment f, FrameBuffer fb)
	{
		float z = f.attrs[0];
		
		if(z >= fb.getZ(f.x, f.y)) {
		    return;
		}

		Color3f c = colorForFragment(f);
		
		fb.set(f.x, f.y, c.x, c.y, c.z, z);
	}
}
