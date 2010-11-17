package pipeline.fragment;

import javax.vecmath.Color3f;
import javax.vecmath.Vector2f;
import javax.vecmath.Vector3f;

import pipeline.Pipeline;
import pipeline.PointLight;
import pipeline.misc.Fragment;
import pipeline.misc.FrameBuffer;

public class ReflectionMapFP extends FragmentProcessor {
	
	Vector3f eye = new Vector3f();
	Vector3f normal = new Vector3f();
	
	Vector3f L = new Vector3f();
	Vector3f H = new Vector3f();
	Vector3f n = new Vector3f();
	Vector3f V = new Vector3f();
	
	// Reflection vector
	Vector3f reflection = new Vector3f();
	
	// Texture coords vector
	Vector2f textureCoords = new Vector2f();
	
	// texture's sample colors
	Color3f texColor = new Color3f();
	
	Color3f c2 = new Color3f();
	
	
	public int nAttr()
	{
	    return 9;
	}
	
	public void fragment(Fragment f, FrameBuffer fb)
	{
		float z = f.attrs[0];
		
		if(z >= fb.getZ(f.x, f.y)) {
		    return;
		}
		
		// Vector to x, y, z
		eye.set( f.attrs[4], f.attrs[5], f.attrs[6] );
		eye.normalize();
		
		// Normal vector and its copy
		normal.set( f.attrs[7], f.attrs[8], f.attrs[9] );
		normal.normalize();
		n.set( f.attrs[7], f.attrs[8], f.attrs[9] );
		n.normalize();
		
		
		// Calculate reflection r = 2(n.u)n - u
		float dotProd = n.dot(eye);
		n.scale( (float)2.0*dotProd ) ;
		reflection.set(eye);
		reflection.sub( n );
		
		// Calculate parameter m = 2*sqrt(rx^2 + ry^2 + (rz + 1)^2  )
		float m = (float) (2.0 * Math.sqrt( reflection.x*reflection.x + 
				reflection.y*reflection.y + 
				(reflection.z + 1)*(reflection.z + 1) ));
		
		// Get texture coords
		textureCoords.x = reflection.x/m + 0.5f;
		textureCoords.y = reflection.y/m + 0.5f;
		
		// Get sample of texture
		texture.sample(textureCoords, texColor);
		
		
		///////////////////////
		
		// Do not combine with fragment color
		f.attrs[1] = f.attrs[2] = f.attrs[3] = 1;
		c2.set(f.attrs[1], f.attrs[2], f.attrs[3]);
		
		c2.x = c2.x * Pipeline.ambientIntensity;
		c2.y = c2.y * Pipeline.ambientIntensity;
		c2.z = c2.z * Pipeline.ambientIntensity;
		
		// Set vertex position for illumination
		V.set(f.attrs[4], f.attrs[5], f.attrs[6]);
		
		// Eye vector for lighting calculations is from vertex to origin
		eye.set(-V.x, -V.y, -V.z);
		eye.normalize();
		
		// For each light, get the contribution to this vertex
		for(PointLight light : Pipeline.lights) {
			Color3f lc = light.getIntensity();

			// Vector from vertex to light source -- normalized
			L.set(light.getPosition());
			L.sub(V);
			L.normalize();
			
			float dot = L.dot(normal);

			// Diffuse contribution
			if(dot > 0) {
				c2.x += c2.x*dot*lc.x;
				c2.y += c2.y*dot*lc.y;
				c2.z += c2.z*dot*lc.z;
			}
			
			// Half vector -- bisector of L and E
			H.set(L);
			H.add(eye);
			H.normalize();
			
			dot = H.dot(normal);
			
			// Specular contribution
			if(dot > 0) {
				dot = (float) Math.pow(dot, Pipeline.specularExponent);
				
				c2.x += dot*Pipeline.specularColor.x;
				c2.y += dot*Pipeline.specularColor.y;
				c2.z += dot*Pipeline.specularColor.z;
			}
		}

		c2.x *= texColor.x;
		c2.y *= texColor.y;
		c2.z *= texColor.z;
		
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
		
		
		fb.set(f.x, f.y, c2.x, c2.y, c2.z, z);
		//fb.set(f.x, f.y, texColor.x, texColor.y, texColor.z, z);
		
	}
}
