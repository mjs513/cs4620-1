package ray.shader;

import java.util.ArrayList;

import ray.IntersectionRecord;
import ray.Scene;
import ray.light.Light;
import ray.math.Color;
import ray.math.Vector3;

/**
 * A Lambertian material scatters light equally in all directions. BRDF value is
 * a constant
 *
 * @author ags
 */
public class Lambertian extends Shader {
	
	/** The color of the surface. */
	protected final Color diffuseColor = new Color(1, 1, 1);
	public void setDiffuseColor(Color inDiffuseColor) { diffuseColor.set(inDiffuseColor); }
	
	public Lambertian() { }

	Vector3 L = new Vector3();
	
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
			}
		}
	}
	
	/**
	 * @see Object#toString()
	 */
	public String toString() {
		
		return "lambertian: " + diffuseColor;
	}
	
}