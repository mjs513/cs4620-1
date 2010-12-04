package ray.shader;

import java.util.ArrayList;

import ray.IntersectionRecord;
import ray.Ray;
import ray.Scene;
import ray.light.Light;
import ray.math.Color;
import ray.math.Vector3;

/**
 * This interface specifies what is necessary for an object to be a material.
 * The shade method is pretty obvious - a material should know how to "color"
 * itself.  The copy method is needed so that a deep copy may be performed by
 * Surface objects, which all have a generic reference to a material.
 * @author ags
 */
public abstract class Shader {
	
	/**
	 * The material given to all surfaces unless another is specified.
	 */
	public static final Shader DEFAULT_MATERIAL = new Lambertian();
	
	/**
	 * Calculate the BRDF value for this material at the intersection described in record.
	 * Returns the BRDF color in outColor.
	 * @param outColor Space for the output color
	 * @param scene The scene
	 * @param lights The lights
	 * @param toEye Vector pointing towards the eye
	 * @param record The intersection record, which hold the location, normal, etc.
	 * @param depth The depth of recursive calls. (Only for recursive ray tracing)
	 *        You can use it any way you want as long as it's consistent.
	 * @param contribution The contribution of the current ray. (Only for recursive ray tracing)
	 * 		  You can use it any way you want or ignore it.
	 * @param internal True if the current ray is inside the material (Only for glass)
	 *        You can use it any way you want or ignore it.
	 */
	public abstract void shade(Color outColor, Scene scene, ArrayList<Light> lights, Vector3 toEye, 
			IntersectionRecord record, int depth, double contribution, boolean internal);
	
	/**
	 * Utility method to compute shadows.
	 */
	protected boolean isShadowed(Scene scene, Light light, IntersectionRecord record) {
		
		Ray shadowRay = new Ray();
		
		// Setup the shadow ray to start at surface and end at light
		shadowRay.origin.set(record.location);
		shadowRay.direction.sub(light.position, record.location);
		
		//Set the ray to end at the light
		shadowRay.makeOffsetSegment(1.0);
		
		return scene.getAnyIntersection(shadowRay);
	}
}