package nup.z;

import android.opengl.GLSurfaceView;
import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLRenderer implements GLSurfaceView.Renderer {
    @Override
    public void onSurfaceCreated(final GL10 pGL10, final EGLConfig pEGLConfig) {
        Log.d("niu2x", "onSurfaceCreated");
        NupJni.on_gl_surface_created();
    }

    @Override
    public void onSurfaceChanged(final GL10 pGL10, final int pWidth, final int pHeight) {
        Log.d("niu2x", "onSurfaceChanged " + pWidth + "," + pHeight);
        NupJni.on_gl_surface_changed(pWidth, pHeight);
    }
    @Override
    public void onDrawFrame(final GL10 gl) {
        NupJni.on_gl_surface_draw();
    }
}
