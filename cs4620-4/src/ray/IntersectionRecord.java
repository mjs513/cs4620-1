package ray;

import ray.math.Point2;
import ray.math.Point3;
import ray.math.Vector3;
import ray.surface.Surface;

/**
 * This class is really just a struct, holding necessary information about a
 * particular intersection point.
 *
 * @author ags
 */
public class IntersectionRecord {

  /** The location where the intersection occurred. */
  public final Point3 location = new Point3();

  /** The normal of the surface at the intersection location. */
  public final Vector3 normal = new Vector3();

  /** The texture coordinates of the intersection point */
  public final Point2 texCoords = new Point2();
  
  /** A reference to the actual surface. */
  public Surface surface = null;

  /** The t value along the ray at which the intersection occurred. */
  public double t = 0;
  
  /**
   * Set this intersection record to the value of inRecord
   *
   * @param inRecord the input record
   */
  public void set(IntersectionRecord inRecord) {

    location.set(inRecord.location);
    normal.set(inRecord.normal);
    texCoords.set(inRecord.texCoords);
    surface = inRecord.surface;
    t = inRecord.t;

  }

}