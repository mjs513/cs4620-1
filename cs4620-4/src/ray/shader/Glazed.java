package ray.shader;

import java.util.ArrayList;

import ray.IntersectionRecord;
import ray.Ray;
import ray.RayTracer;
import ray.Scene;
import ray.Workspace;
import ray.light.Light;
import ray.math.Color;
import ray.math.Vector3;

public class Glazed extends Shader {
	
	protected double refractiveIndex;
	public void setRefractiveIndex(double refractiveIndex) { this.refractiveIndex = refractiveIndex; }
	
	protected Shader substrate;
	public void setSubstrate(Shader substrate) { this.substrate = substrate; }
	
	/** The exponent controlling the sharpness of the specular reflection. */
	protected double exponent = 1.0;
	public void setExponent(double exponent) { this.exponent = exponent; }
	
	public Glazed() { }
	
	/**
	 * Calculate the BRDF value for this material at the intersection described in record.
	 * Returns the BRDF color in outColor.
	 * @param outColor Space for the output color
	 * @param scene The scene
	 * @param lights The lights
	 * @param toEye Vector pointing towards the eye
	 * @param record The intersection record, which hold the location, normal, etc.
	 * @param depth The depth of recursive calls.
	 *        You can use it any way you want as long as it's consistent.
	 * @param contribution The contribution of the current ray.
	 * 		  You can use it any way you want.
	 * @param internal You can ignore this for glazed.
	 */
	public void shade(Color outColor, Scene scene, ArrayList<Light> lights, Vector3 toEye, 
			IntersectionRecord record, int depth, double contribution, boolean internal) {
		// TODO(B): fill in this function.
		//Implement Fresnel equations, make a new Workspace and recursive call to shadeRay
		
		// r = 2(n.v)n - v
		double d = 2.0*(record.normal.dot(toEye));
		Vector3 reflectedRay = new Vector3();
		reflectedRay.set(record.normal);
		reflectedRay.scale(d);
		reflectedRay.sub(toEye);
		
		
		
		
		
		
		
	}
}