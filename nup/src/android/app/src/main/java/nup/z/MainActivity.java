package nup.z;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.TextView;

import nup.z.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'z' library on application startup.
    static {
       System.loadLibrary("nup");
    }

    private ActivityMainBinding binding;
    private FrameLayout glContainer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        glContainer = (FrameLayout)findViewById(R.id.gl_container);
        GLSurfaceView glView = new GLView(this);
        glView.setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        glContainer.addView(glView);
    }
}