package nup.z;

public class NupJni {
    public static native void on_gl_surface_created();
    public static native void on_gl_surface_changed(int width, int height);
    public static native void on_gl_surface_draw();
}
