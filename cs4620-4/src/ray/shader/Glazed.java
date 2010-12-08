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
			IntersectionRecord record, int depth, double contribution, boolean internal)
	{
		
		// r = 2(n.v)n - v
		double d = 2.0*(record.normal.dot(toEye));
		Vector3 reflectedRay = new Vector3();
		
		reflectedRay.set(record.normal);
		reflectedRay.scale(d);
		reflectedRay.sub(toEye);
		
		Workspace workspace = new Workspace();
		Ray ray = workspace.eyeRay;
		
		ray.set(record.location, reflectedRay);
		ray.direction.normalize();
		ray.makeOffsetRay();
		
		// Schlick's approximation to Fresnel
		
		double cos = ray.direction.dot(record.normal);
		double schlick = 0;
		
		if(cos >= 0) {
			double temp = 1.0 - ray.direction.dot(record.normal);
			double r0 = (refractiveIndex - 1.0)/(refractiveIndex + 1.0);
			
			r0 = r0*r0;
			
			schlick = r0 + (1.0 - r0)*temp*temp*temp*temp*temp;
		}
		
		if(schlick > 0) {
			RayTracer.shadeRay(workspace.rayColor, scene, ray, workspace, lights, depth + 1, contribution, internal);
		}
		
		substrate.shade(outColor, scene, lights, toEye, record, depth, contribution, internal);
		
		workspace.rayColor.scale(schlick);
		outColor.scale(1 - schlick);
		
		outColor.add(workspace.rayColor);
	}
}