package pipeline.vertex;

import javax.vecmath.Color3f;
import javax.vecmath.Point3f;
import javax.vecmath.Vector3f;
import javax.vecmath.Vector4f;

import pipeline.Pipeline;
import pipeline.math.Matrix4f;

/**
 * This is an interitance abstraction used to pull up some of the common
 * matrices and update functions shared by the smooth and flat shaded triangle
 * processors.
 *
 * Note that as of 2008, we no longer do flat-shading.
 * 
 * @author ags
 */
public abstract class ShadedVP extends VertexProcessor {

	public void updateTransforms(Pipeline pipe) {
		// TODO 
		// Note to students: You could optionally ignore this class and just implement this function in SmoothShadedVP, since that's the only sub-class of this.
	}

}
