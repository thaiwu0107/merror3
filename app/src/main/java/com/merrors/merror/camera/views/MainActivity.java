package com.merrors.merror.camera.views;

import android.Manifest;
import android.content.Context;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import com.merrors.merror.R;
import com.merrors.merror.mvp.ui.widget.CircleWidthView;
import com.merrors.merror.mvp.ui.widget.ModifaceView;

/**
 * \addtogroup Java
 *   @{
 */

/**
 Main activity that displays a view that shows camera frames with makeup applied.
 This activity also has a "Change Look" button to change the makeup applied.
 Moreover, this activity handles permission requests and starting the camera preview.
 */
public class MainActivity extends AppCompatActivity {

    ViewGroup makeupTypeHolder, makeupTypeHolderInner;

    // View that displays the result from the makeup engine
    ModifaceView modifaceView;

    MakeupManager makeupManager;

    ImageView pictureHolder;

    View splitCompareLine;
    boolean showSplitCompare = false;

    // Dangerous permissions required by this demo app
    private static final String[] RequiredDangerousPermissions = { Manifest.permission.CAMERA };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        /*
        Force portrait screen orientation
         */
        if (Build.VERSION.SDK_INT >= 18) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_USER_PORTRAIT);
        } else if (Build.VERSION.SDK_INT >= 9)
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
        else
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);


        /*
        Hide the status bar
         */
        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);


        /*
        Setup UI
         */
        setContentView(R.layout.activity_main);

        modifaceView = (ModifaceView) findViewById(R.id.main_makeup_view);

        splitCompareLine = findViewById(R.id.split_line);

        pictureHolder = (ImageView) findViewById(R.id.picture_holder);

        makeupTypeHolder = (ViewGroup) findViewById(R.id.horizontal_scroll_view);
        makeupTypeHolderInner = (ViewGroup) findViewById(R.id.linear_layout_inside_scroll_view);

        makeupManager = new MakeupManager();
        makeupManager.setViews(
                findViewById(R.id.makeup_container),
                ((TextView) findViewById(R.id.seek_bar_one_label)),
                ((SeekBar) findViewById(R.id.seek_bar_one)),
                ((TextView) findViewById(R.id.seek_bar_two_label)),
                ((SeekBar) findViewById(R.id.seek_bar_two)),
                ((TextView) findViewById(R.id.seek_bar_three_label)),
                ((SeekBar) findViewById(R.id.seek_bar_three)),
                ((Switch) findViewById(R.id.toggle)),
                (CircleWidthView) findViewById(R.id.makeup_container_color),
                (LinearLayout) findViewById(R.id.color_container),
                findViewById(R.id.toggle_style));
        makeupManager.setMakeupChangedListener(new MakeupManager.MakeupChangedListener() {
            @Override
            public void lipChanged(boolean enabled, int color, float opacity, float glitter, float gloss) {
                if (!enabled) modifaceView.removeLip();
                else modifaceView.addLip(color, opacity, glitter, gloss);
            }
            @Override
            public void foundationChanged(boolean enabled, int color, float opacity, float smoothing) {
                if (!enabled) modifaceView.removeFoundation();
                else modifaceView.addFoundation(color, opacity, smoothing);
            }
            @Override
            public void eyelinerChanged(boolean enabled, int color, int style, float opacity) {
                if (!enabled) modifaceView.removeEyeliner();
                else modifaceView.addEyeliner(color, style, opacity);
            }
            @Override
            public void lashChanged(boolean enabled, int color, int style, float opacity) {
                if (!enabled) modifaceView.removeLash();
                else modifaceView.addLash(color, style, opacity);
            }
            @Override
            public void blushChanged(boolean enabled, int color, int style, float opacity) {
                if (!enabled) modifaceView.removeBlush();
                else modifaceView.addBlush(color, style, opacity);
            }
            @Override
            public void browChanged(boolean enabled, int color, float opacity) {
                if (!enabled) modifaceView.removeBrow();
                else modifaceView.addBrow(color, opacity);
            }
            @Override
            public void liplinerChanged(boolean enabled, int color, float opacity) {
                if (!enabled) modifaceView.removeLipliner();
                else modifaceView.addLipliner(color, opacity);
            }
            @Override
            public void eyeshadowLidChanged(boolean enabled, int color, float opacity, float glitter) {
                if (!enabled) modifaceView.removeEyeshadowLid();
                else modifaceView.addEyeshadowLid(color, opacity, glitter);
            }
            @Override
            public void eyeshadowCreaseChanged(boolean enabled, int color, float opacity, float glitter) {
                if (!enabled) modifaceView.removeEyeshadowCrease();
                else modifaceView.addEyeshadowCrease(color, opacity, glitter);
            }
            @Override
            public void eyeshadowHighChanged(boolean enabled, int color, float opacity, float glitter) {
                if (!enabled) modifaceView.removeEyeshadowHigh();
                else modifaceView.addEyeshadowHigh(color, opacity, glitter);
            }
        });

        /*
        Start camera or request for permissions (which include camera permission)
         */
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.M && !isAllPermissionsGranted(this)) {
            this.requestPermissions(RequiredDangerousPermissions, 13);
        }
        else {
            modifaceView.startCamera();
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);

        /*
        Hide the status bar
         */
        View decorView = getWindow().getDecorView();
        int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
        decorView.setSystemUiVisibility(uiOptions);
    }

    public static boolean isAllPermissionsGranted(Context context) {
        for(String permission : RequiredDangerousPermissions) {
            if(ContextCompat.checkSelfPermission(context, permission) != PackageManager.PERMISSION_GRANTED) return false;
        }
        return true;
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        switch (requestCode) {
            case 13:
                // If request is cancelled, the result arrays are empty.
                boolean allDangerousPermissionsGranted = grantResults.length > 0 && permissions.length == grantResults.length;

                for(int granted : grantResults) {
                    allDangerousPermissionsGranted = allDangerousPermissionsGranted && (granted == PackageManager.PERMISSION_GRANTED);
                }

                if(!allDangerousPermissionsGranted) {
                    Log.e("SDKDemo", "not all dangerous permission granted");

                    // show error dialog
                    new AlertDialog.Builder(this)
                            .setTitle("Permissions Error")
                            .setMessage(
                                    "By not granting these permissions, this app may not work. \n" +
                                    "Please restart the app and grant the permissions. "
                            )
                            .show();
                }
                else {
                    modifaceView.startCamera();
                }


                break;

        }
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
    }

    @Override
    protected void onPause() {

        modifaceView.onPause();

        super.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();

        modifaceView.onResume();

        refreshSplitCompareOnResume();

        // Need to run this twice. Occasionally the texture gets destroyed after we create it
        modifaceView.post(()->modifaceView.onResume());
    }

    public void makeupContainerExitClicked(View view) {
        makeupTypeHolder.setOnTouchListener(null);
        makeupTypeHolder.setAlpha(1.0f);
        for (int i = 0; i < makeupTypeHolderInner.getChildCount(); i++) {
            View child = makeupTypeHolderInner.getChildAt(i);
            child.setClickable(true);
        }
        makeupManager.hide();
    }

    private void disableHorizontalScroller(){
        makeupTypeHolder.setOnTouchListener((v, event)->true);
        makeupTypeHolder.setAlpha(0.5f);
        for (int i = 0; i < makeupTypeHolderInner.getChildCount(); i++) {
            View child = makeupTypeHolderInner.getChildAt(i);
            child.setClickable(false);
        }
    }

    public void lipClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showLip();
    }

    public void foundationClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showFoundation();
    }

    public void eyelinerClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showEyeliner();
    }

    public void lashClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showLash();
    }

    public void eyeshadowLidClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showEyeshadowLid();
    }

    public void eyeshadowCreaseClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showEyeshadowCrease();
    }

    public void eyeshadowHighClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showEyeshadowHigh();
    }

    public void blushClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showBlush();
    }

    public void browClicked(View v) {
        disableHorizontalScroller();
        makeupManager.showBrow();
    }

    public void liplinerClicked(View view) {
        disableHorizontalScroller();
        makeupManager.showLipliner();
    }

    boolean usingFrontFacingCamera = true;

    public void flipCameraClicked(View view) {
        usingFrontFacingCamera = !usingFrontFacingCamera;
        modifaceView.startCamera(usingFrontFacingCamera);
    }

    public void captureBeforeClicked(View view) {
        modifaceView.captureBeforeBitmap(this::flashCapturedBitmap);
    }

    public void captureAfterClicked(View view) {
        modifaceView.captureAfterBitmap(this::flashCapturedBitmap);
    }

    public void compareBeforeAfterClicked(View view) {
        if (showSplitCompare){
            showSplitCompare = false;
            splitCompareLine.setVisibility(View.GONE);
        } else {
            showSplitCompare = true;
            splitCompareLine.setVisibility(View.VISIBLE);
        }
        modifaceView.toggleSplitCompare();
    }

    public void refreshSplitCompareOnResume(){
        if (showSplitCompare) {
            modifaceView.toggleSplitCompare();
        }
    }

    public static Bitmap rotate(Bitmap bitmap, float degrees) {
        Matrix matrix = new Matrix();
        matrix.postRotate(degrees);
        return Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
    }

    private void flashCapturedBitmap(final Bitmap bitmap) {
        new Handler(Looper.getMainLooper()).post(()->{
            pictureHolder.setAlpha(1.0f);
            pictureHolder.setImageBitmap(bitmap);
            pictureHolder.postDelayed(()->pictureHolder.animate().alpha(0.0f).setDuration(1000),1000);
        });
    }
}

/** @}*/
