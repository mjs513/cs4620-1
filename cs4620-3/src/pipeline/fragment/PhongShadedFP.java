package pipeline.fragment;

import javax.vecmath.Color3f;
import javax.vecmath.Vector3f;

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
	// Vectors needed to calculate specular and diffuse lighting contributions
	Vector3f E = new Vector3f(), L = new Vector3f(), N = new Vector3f(), H = new Vector3f(), V = new Vector3f();

	// Interpolated vertex colors
	Color3f c = new Color3f();
	
	// Color generated by contribution of all lights
	Color3f c2 = new Color3f();
	
	
	public int nAttr()
	{
		// 3 for color + 3 for position + 3 for normal
		return 9;
	}
	
	protected Color3f colorForFragment(Fragment f)
	{
		V.set(f.attrs[4], f.attrs[5], f.attrs[6]);
		
		// Vector from fragment to eye
		E.set(-V.x, -V.y, -V.z);
		E.normalize();

		// Normal
		N.set(f.attrs[7], f.attrs[8], f.attrs[9]);
		N.normalize();

		// Interpolated vertex colors
		c.set(f.attrs[1], f.attrs[2], f.attrs[3]);
		
		// Color generated by contribution of all lights
		// Ambient contribution
		c2.set(Pipeline.ambientIntensity*c.x, Pipeline.ambientIntensity*c.y, Pipeline.ambientIntensity*c.z);
		
		// For each light, get the contribution to this vertex
		for(PointLight light : Pipeline.lights) {
			Color3f lc = light.getIntensity();

			// Vector from vertex to light source -- normalized
			L.set(light.getPosition());
			L.sub(V);
			L.normalize();
			
			float dot = L.dot(N);

			// Diffuse contribution
			if(dot > 0) {
				c2.x += c.x*dot*lc.x;
				c2.y += c.y*dot*lc.y;
				c2.z += c.z*dot*lc.z;
			}
			
			// Half vector -- bisector of L and E
			H.set(L);
			H.add(E);
			H.normalize();
			
			dot = H.dot(N);
			
			// Specular contribution
			if(dot > 0) {
				dot = (float) Math.pow(dot, Pipeline.specularExponent);
				
				c2.x += dot*Pipeline.specularColor.x;
				c2.y += dot*Pipeline.specularColor.y;
				c2.z += dot*Pipeline.specularColor.z;
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