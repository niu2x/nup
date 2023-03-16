package nup.z;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class GLView extends GLSurfaceView {
    public GLView(final Context context) {
        super(context);
        this.initView();
    }

    public GLView(final Context context, final AttributeSet attrs) {
        super(context, attrs);
        this.initView();
    }

    private void initView() {
        this.setEGLContextClientVersion(3);
        this.setFocusableInTouchMode(true);
        this.setEGLConfigChooser(8, 8, 8, 8, 16, 8);
        this.setRenderer(new GLRenderer());
    }
}
