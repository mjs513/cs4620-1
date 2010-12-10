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

public class Glass extends Shader
{
	protected double refractiveIndex;
	public void setRefractiveIndex(double refractiveIndex) { this.refractiveIndex = refractiveIndex; }
	
	/** The exponent controlling the sharpness of the specular reflection. */
	protected double exponent = 1.0;
	public void setExponent(double exponent) { this.exponent = exponent; }
	
	public Glass() { }
	
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
		Workspace wsReflection = new Workspace();
		Workspace wsRefraction = new Workspace();
		
		double n1, n2;
		
		// n2 is air
		if(internal) {
			n1 = refractiveIndex;
			n2 = 1;
		}
		
		// n1 is air
		else {
			n1 = 1;
			n2 = refractiveIndex;
		}
		
		// r = 2(n.v)n - v
		double d = 2.0*(record.normal.dot(toEye));
		
		Ray reflectedRay = wsReflection.eyeRay;
		
		reflectedRay.direction.set(record.normal);
		reflectedRay.direction.scale(d);
		reflectedRay.direction.sub(toEye);
		reflectedRay.direction.normalize();
		
		reflectedRay.origin.set(record.location);
		
		reflectedRay.makeOffsetRay();
		
		// Schlick's approximation to Fresnel
		
		double cosIncidence = reflectedRay.direction.dot(record.normal);
		double schlick = 0;
		
		if(cosIncidence >= 0) {
			double temp = 1.0 - cosIncidence;
			double r0 = (n2 - n1)/(n2 + n1);
			
			r0 = r0*r0;
			
			schlick = r0 + (1.0 - r0)*temp*temp*temp*temp*temp;
		}
		
		// Reflection
		if(schlick > 0) {
			RayTracer.shadeRay(wsReflection.rayColor, scene, reflectedRay, wsReflection, lights, depth + 1, contribution*schlick, internal);
		}
		
		double dot = -record.normal.dot(toEye);
		double discr = 1 - n1*n1*(1 - dot*dot)/(n2*n2);

		// Refraction
		if(discr >= 0) {
			Ray refractedRay = wsRefraction.eyeRay;
			
			// refractedRay = (d - n*dot(d, n))*n1/n2 - normal*sqrt(discr)    [d = -toEye]
			refractedRay.direction.set(toEye);
			refractedRay.direction.scale(-1);
			refractedRay.direction.scaleAdd(-dot, record.normal);
			refractedRay.direction.scale(n1/n2);
			
			refractedRay.origin.set(record.location);
			
			refractedRay.makeOffsetRay();
			
			RayTracer.shadeRay(wsRefraction.rayColor, scene, refractedRay, wsRefraction, lights, depth, contribution*(1 - schlick), internal);
		}
		
		wsReflection.rayColor.scale(schlick);
		wsRefraction.rayColor.scale(1 - schlick);
		
		outColor.set(wsReflection.rayColor);
		outColor.add(wsRefraction.rayColor);
	}
}