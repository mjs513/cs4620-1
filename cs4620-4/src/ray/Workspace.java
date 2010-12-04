/*
 * Created on Aug 20, 2005
 * Copyright 2005 Program of Computer Grpahics, Cornell University
 */
package ray;

import java.util.Random;

import ray.math.Color;
import ray.math.Vector3;


/**
 * Each pixel requires a few data elements to compute the color.  Allocating them
 * new each pixel can be quite costly.  Further, each pixel is completely independent
 * so the data space could be reused each pixel without causing any computation errors.
 * By placing all the data elements into this short class, this working space can be
 * passed to the pixel shading method each time and avoid the unnecessary allocation.
 *
 * @author Adam Arbree
 * Aug 20, 2005
 * Copyright 2005 Program of Computer Graphics, Cornell University
 */
public class Workspace {

  /** Space for the eye ray intersection record */
  public final IntersectionRecord eyeRecord = new IntersectionRecord();

  /** Space for the shadow ray intersection record */
  public final IntersectionRecord shadowRecord = new IntersectionRecord();

  /** Space for an eye ray */
  public final Ray eyeRay = new Ray();
  
  /** Space for a shadow ray */
  public final Ray shadowRay = new Ray();

  /** Space for eye vector calculations * */
  public final Vector3 toEye = new Vector3();

  /** Space for light vector calculations * */
  public final Vector3 toLight = new Vector3();

  /** Space for the BRDF color * */
  public final Color color = new Color();
  
  /** Space for the active pixel color */
  public final Color pixelColor = new Color();
  
  /** Space for the active ray color */
  public final Color rayColor = new Color();

  /** A random number generator **/
  public final Random random = new Random();
  
}
