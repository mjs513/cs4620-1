package ray;

import java.util.ArrayList;
import java.util.Iterator;

import ray.light.Light;
import ray.math.Matrix4;
import ray.math.Point3;
import ray.shader.Shader;
import ray.surface.Surface;


/**
 * The scene is just a collection of objects that compose a scene. The camera,
 * the list of lights, and the list of surfaces.
 *
 * @author ags
 */
public class Scene {
	
	/** The camera for this scene. */
	protected Camera camera;
	public void setCamera(Camera camera) { this.camera = camera; }
	public Camera getCamera() { return this.camera; }
	
	/** The list of lights for the scene. */
	protected ArrayList<Light> lights = new ArrayList<Light>();
	public void addLight(Light toAdd) { lights.add(toAdd); }
	public ArrayList<Light> getLights() { return this.lights; }
	
	/** The list of surfaces for the scene. */
	protected ArrayList<Surface> surfaces = new ArrayList<Surface>();
	public void addSurface(Surface toAdd) { surfaces.add(toAdd); }
	
	/** The list of materials in the scene. */
	protected ArrayList<Shader> shaders = new ArrayList<Shader>();
	public void addShader(Shader toAdd) { shaders.add(toAdd); }
	
	/** Image to be produced by the renderer **/
	protected Image outputImage;
	public Image getImage() { return this.outputImage; }
	public void setImage(Image outputImage) { this.outputImage = outputImage; }

	/** samples^2 is the number of samples per pixel **/
	protected int samples;
	public int getSamples() { return this.samples==0 ? 1 : this.samples; }
	public void setSamples(int n) {	samples = (int)Math.round(Math.sqrt(n)); }
	
	// TODO(B): change AABBFake to AABB when you have implemented AABB
	protected AABBFake aabbTree;
	
	/**
	 * Work space for an intersection record needed in the
	 * getFirstIntersctionMethod(). Avoids creating a new record each ray cast,
	 * but not thread safe.
	 */
	private IntersectionRecord workRec = new IntersectionRecord();
	
	
	/**
	 * Initialize AABB by retrieving a list of all surfaces from the scene.
	 * Send the list to AABB and call createTree.
	 */
	public void initializeAABB() {
		ArrayList<Surface> allSurfaces = new ArrayList<Surface>();
		for (Iterator<Surface> iter = surfaces.iterator(); iter.hasNext();)
			iter.next().appendRenderableSurfaces(allSurfaces);
		Surface []surfaceArray = new Surface[allSurfaces.size()];
		int count = 0;
		for(Iterator<Surface> iter = allSurfaces.iterator(); iter.hasNext();)
			surfaceArray[count++] = iter.next();
		
		// TODO(B): change AABBFake to AABB when you have implemented AABB
		AABBFake.setSurfaces(surfaceArray);
		aabbTree = AABBFake.createTree(0, surfaceArray.length);
	}
	
	/**
	 * Initialize transformation matrices for entire tree hierarchy
	 */
	public void setTransform() {
		Matrix4 id = new Matrix4();
		id.setIdentity();
		for (Iterator<Surface> iter = surfaces.iterator(); iter.hasNext();) {
			Surface currSurface = iter.next();
			currSurface.setTransformation(id, id, id);
		}
	}
	
	/**
	 * Set outRecord to the first intersection of ray with the scene. Return true
	 * if there was an intersection and false otherwise. If no intersection was
	 * found outRecord is unchanged.
	 *
	 * @param outRecord the output IntersectionRecord
	 * @param ray the ray to intesect
	 * @return true if and intersection is found.
	 */
	public boolean getFirstIntersection(IntersectionRecord outRecord, Ray ray) {
		return aabbTree.intersect(outRecord, ray, false);
	}
	
	/**
	 * Shadow ray calculations can be considerably accelerated by not bothering to find the
	 * first intersection.  This record returns any intersection of the ray and the surfaces
	 * and returns true if one is found.
	 * @param ray the ray to intersect
	 * @return true if any intersection is found
	 */
	public boolean getAnyIntersection(Ray ray) {
		return aabbTree.intersect(new IntersectionRecord(), ray, true);	
	}
}