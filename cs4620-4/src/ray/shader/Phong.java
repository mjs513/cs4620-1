package ray.shader;

import java.util.ArrayList;

import ray.IntersectionRecord;
import ray.Scene;
import ray.light.Light;
import ray.math.Color;
import ray.math.Vector3;

/**
 * A Phong material. Uses the Modified Blinn-Phong model which is energy
 * preserving and reciprocal.
 *
 * @author ags
 */
public class Phong extends Shader {
	
	/** The color of the diffuse reflection. */
	protected final Color diffuseColor = new Color(1, 1, 1);
	public void setDiffuseColor(Color diffuseColor) { this.diffuseColor.set(diffuseColor); }
	
	/** The color of the specular reflection. */
	protected final Color specularColor = new Color(1, 1, 1);
	public void setSpecularColor(Color specularColor) { this.specularColor.set(specularColor); }
	
	/** The exponent controlling the sharpness of the specular reflection. */
	protected double exponent = 1.0;
	public void setExponent(double exponent) { this.exponent = exponent; }
	
	public Phong() { }

	Vector3 L = new Vector3(), H = new Vector3();
	
	/**
	 * Calculate the BRDF value for this material at the intersection described in record.
	 * Returns the BRDF color in outColor.
	 * @param outColor Space for the output color
	 * @param scene The scene
	 * @param lights The lights
	 * @param toEye Vector pointing towards the eye
	 * @param record The intersection record, which hold the location, normal, etc.
	 * @param depth The depth of recursive calls. You can ignore this parameter.
	 * @param contribution The contribution of the current ray. You can ignore this parameter.
	 * @param internal You can ignore this parameter.
	 */
	public void shade(Color outColor, Scene scene, ArrayList<Light> lights, Vector3 toEye, 
			IntersectionRecord record, int depth, double contribution, boolean internal)
	{
		for(Light light : lights) {
			if(!isShadowed(scene, light, record)) {
				L.sub(light.position, record.location);
				L.normalize();
				
				double dot = L.dot(record.normal);
				
				if(dot > 0) {
					outColor.r += dot*light.intensity.r*diffuseColor.r;
					outColor.g += dot*light.intensity.g*diffuseColor.g;
					outColor.b += dot*light.intensity.b*diffuseColor.b;
				}
				
				H.set(L);
				H.add(toEye);
				H.normalize();
				
				dot = H.dot(record.normal);
				
				if(dot > 0) {
					dot = Math.pow(dot, exponent);
					
					outColor.r += dot*light.intensity.r*specularColor.r;
					outColor.g += dot*light.intensity.g*specularColor.g;
					outColor.b += dot*light.intensity.b*specularColor.b;
				}
			}
		}
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		
		return "phong " + diffuseColor + " " + specularColor + " " + exponent + " end";
	}
}