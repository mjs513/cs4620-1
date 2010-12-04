package ray;

import java.io.File;
import java.util.ArrayList;

import ray.light.Light;
import ray.math.Color;
import ray.math.Vector3;

/**
 * A simple ray tracer.
 *
 * @author ags
 */
public class RayTracer {

	public static String testFolderPath;
	
	public static String getTestFolderPath() { return testFolderPath; }
	/**
	 * If filename is a directory, set testFolderPath = fn.
	 * And return a list of all .xml files inside the directory
	 * @param fn Filename or directory
	 * @return fn itself in case fn is a file, or all .xml files inside fn
	 */
	public static final ArrayList<String> getFileLists(String fn) {
		if(fn.endsWith("/"))
			fn.substring(0, fn.length()-1);
		
		File file = new File(fn);
		ArrayList<String> output = new ArrayList<String>();
		if(file.exists()) {
			if(file.isFile()) {
				if(file.getParent() != null)
					testFolderPath = file.getParent() + "/";
				else
					testFolderPath = "";
				output.add(fn);
			} else {
				testFolderPath = fn + "/";
				for(String fl : file.list()) {
					if(fl.endsWith(".xml")) {
						output.add(testFolderPath + fl);
					}
				}	
			}
		} else
			System.out.println("File not found.");

		return output;
	}
	/**
	 * The main method takes all the parameters an assumes they are input files
	 * for the ray tracer. It tries to render each one and write it out to a PNG
	 * file named <input_file>.png.
	 *
	 * @param args
	 */
	public static final void main(String[] args) {

		Parser parser = new Parser();
		for (int ctr = 0; ctr < args.length; ctr++) {

			ArrayList<String> fileLists = getFileLists(args[ctr]);
			
			for (String inputFilename : fileLists) {
				String outputFilename = inputFilename + ".png";
	
				// Parse the input file
				Scene scene = (Scene) parser.parse(inputFilename, Scene.class);
				
				// Propagate transformation matrix thru the tree hierarchy
				scene.setTransform();
				
				// Initialize AABB tree
				scene.initializeAABB();
				
				// Render the scene
				renderImage(scene);
	
				// Write the image out
				scene.getImage().write(outputFilename);
			}
		}
	}

	/**
	 * The renderImage method renders the entire scene.
	 *
	 * @param scene The scene to be rendered
	 */
	public static void renderImage(Scene scene) {

		// Get the output image
		Image image = scene.getImage();
		Camera cam = scene.getCamera();

		// Set the camera aspect ratio to match output image
		int width = image.getWidth();
		int height = image.getHeight();

		// Timing counters
		long startTime = System.currentTimeMillis();

		// Do some basic setup
		Workspace work = new Workspace();
		Ray ray = work.eyeRay;
		Color pixelColor = work.pixelColor;
		Color rayColor = work.rayColor;


		int total = height * width;
		int counter = 0;
		int lastShownPercent = 0;
		int samples = scene.getSamples();
		
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				double pctx = (x + 0.5)/width, pcty = (y + 0.5)/height;
				
				ray = cam.getRay(pctx, pcty);
				
				// TODO(B): Support Anti-Aliasing
				
				// TODO(A): Compute the "ray"
				
			    // solving the quadratic equation for t at the pts of intersection
				// don't forget to check the discriminant and update the intersection record
								
				
				shadeRay(rayColor, scene, ray, work, scene.getLights(), 1, 1, false);
				pixelColor.set(rayColor);
				
				//Gamma correct and clamp pixel values
				pixelColor.gammaCorrect(2.2);
				pixelColor.clamp(0, 1);
				image.setPixelColor(pixelColor, x, y);
				
				counter ++;
				if((int)(100.0 * counter / total) != lastShownPercent) {
					lastShownPercent = (int)(100.0*counter / total);
					System.out.println(lastShownPercent + "%");
				}
			}
		}

		// Output time
		long totalTime = (System.currentTimeMillis() - startTime);
		System.out.println("Done.  Total rendering time: "
				+ (totalTime / 1000.0) + " seconds");

	}

	/**
	 * This method returns the color along a single ray in outColor.
	 *
	 * @param outColor output space
	 * @param scene the scene
	 * @param ray the ray to shade
	 */
	public static void shadeRay(Color outColor, Scene scene, Ray ray, Workspace workspace, 
			ArrayList<Light> lights, int depth, double contribution, boolean internal)
	{
		// TODO(B): Set base case for recursive ray tracing
		
		// Reset the output color
		double x = ray.direction.length() - 7;
		outColor.set(x/3, x/3, x/3);
		
		
		// Rename all the workspace entries to avoid field accesses
		// and alot of typing "workspace."
		IntersectionRecord eyeRecord = workspace.eyeRecord;
		Vector3 toEye = workspace.toEye;

		// TODO(A): Find the first intersection of "ray" with the scene.
		// Record intersection in eyeRecord. If it doesn't hit anything, just return (exit function).
		
		
		// TODO(A): Compute "toEye" from eyeRecord.
		// toEye is the ray from the hit position to the eye.


		// Compute color
		//eyeRecord.surface.getShader().shade(outColor, scene, lights, toEye, eyeRecord, depth, contribution, internal);
	}

}
