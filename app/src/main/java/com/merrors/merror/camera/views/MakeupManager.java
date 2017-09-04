package com.merrors.merror.camera.views;

import android.graphics.Color;
import android.view.View;
import android.widget.LinearLayout;
import android.widget.SeekBar;
import android.widget.Switch;
import android.widget.TextView;

import com.merrors.merror.R;
import com.merrors.merror.mvp.ui.widget.CircleWidthView;

/**
 * Created by Jeff on 2017-08-08.
 */

public class MakeupManager {

    private static final int maxColorCount = 10;
    private static final int seekCount = 3;

    MakeupType currentType;

    TextView seekLabels[] = new TextView[seekCount];
    SeekBar seekBars[] = new SeekBar[seekCount];
    Switch toggle;
    CircleWidthView colorsButton;
    LinearLayout colorsContainer;
    View mainContainer;
    View toggleStyle;

    MakeupChangedListener changedListener;

    public void hide() {
        colorsContainer.setVisibility(View.GONE);
        mainContainer.setVisibility(View.GONE);
    }

    public interface MakeupChangedListener {
        void lipChanged(boolean enabled, int color, float opacity, float glitter, float gloss);
        void foundationChanged(boolean enabled, int color, float opacity, float smoothing);
        void eyelinerChanged(boolean enabled, int color, int style, float opacity);
        void lashChanged(boolean enabled, int color, int style, float opacity);
        void blushChanged(boolean enabled, int color, int style, float opacity);
        void browChanged(boolean enabled, int color, float opacity);
        void liplinerChanged(boolean enabled, int color, float opacity);
        void eyeshadowLidChanged(boolean enabled, int color, float opacity, float glitter);
        void eyeshadowCreaseChanged(boolean enabled, int color, float opacity, float glitter);
        void eyeshadowHighChanged(boolean enabled, int color, float opacity, float glitter);
    }
    public void setMakeupChangedListener(MakeupChangedListener listener) {
        this.changedListener = listener;
    }

    public void setViews(View mainContainer, TextView seekOneLabel, SeekBar seekOne, TextView seekTwoLabel, SeekBar seekTwo,
                         TextView seekThreeLabel, SeekBar seekThree, Switch toggle, CircleWidthView colorButton,
                         LinearLayout colorContainer, View toggleStyle) {

        seekLabels[0] = seekOneLabel;
        seekBars[0] = seekOne;
        seekLabels[1] = seekTwoLabel;
        seekBars[1] = seekTwo;
        seekLabels[2] = seekThreeLabel;
        seekBars[2] = seekThree;

        this.toggle = toggle;
        this.colorsButton = colorButton;
        this.colorsContainer = colorContainer;
        this.mainContainer = mainContainer;
        this.toggleStyle = toggleStyle;

        for (int i = 0; i < seekCount; i++) {
            seekBars[i].setOnSeekBarChangeListener(getSeekBarChangeListener(i));
        }

        toggle.setOnCheckedChangeListener((buttonView, isChecked)->{
            currentType.enabled = isChecked;
            updateCurrentType();
        });

        colorButton.setOnClickListener(v->MakeupManager.this.colorsContainer.setVisibility(View.VISIBLE));

        LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(0,1,1);
        int margin = (int) colorsContainer.getContext().getResources().getDimension(R.dimen.color_circle_margin);
        lp.setMargins(margin,margin,margin,margin);
        for (int i = 0; i < maxColorCount; i++) {
            CircleWidthView circleWidthView = new CircleWidthView(colorsContainer.getContext());
            colorsContainer.addView(circleWidthView, lp);
            final int index = i;
            circleWidthView.setOnClickListener(v->{
                currentType.color = currentType.colors[index];
                colorsButton.setCircleColor(currentType.color);
                updateCurrentType();
            });
        }

        toggleStyle.setOnClickListener(v->{
            currentType.currentStyle += 1;
            if (currentType.currentStyle >= currentType.maxStyles) currentType.currentStyle = 0;
            updateCurrentType();
        });

    }

    private SeekBar.OnSeekBarChangeListener getSeekBarChangeListener(final int seekBarIndex) {
        return new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                currentType.seekValues[seekBarIndex] = progress;
                updateCurrentType();
            }
            @Override public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override public void onStopTrackingTouch(SeekBar seekBar) {}
        };
    }

    private void updateCurrentType() {
        if (changedListener == null) return;
        if (currentType == lip) {
            changedListener.lipChanged(lip.enabled, lip.color,((float)lip.seekValues[0])/100f,
                    ((float)lip.seekValues[1])/100f, ((float)lip.seekValues[2])/100f);
        } else if (currentType == foundation) {
            changedListener.foundationChanged(foundation.enabled, foundation.color,((float)foundation.seekValues[0])/100f,
                    ((float)foundation.seekValues[1])/100f);
        } else if (currentType == eyeliner) {
            changedListener.eyelinerChanged(eyeliner.enabled, eyeliner.color, eyeliner.currentStyle, ((float)eyeliner.seekValues[0])/100f);
        } else if (currentType == lash) {
            changedListener.lashChanged(lash.enabled, lash.color, lash.currentStyle, ((float)lash.seekValues[0])/100f);
        } else if (currentType == shadowLid) {
            changedListener.eyeshadowLidChanged(shadowLid.enabled, shadowLid.color, ((float)shadowLid.seekValues[0])/100f,
                    ((float)shadowLid.seekValues[1])/100f);
        } else if (currentType == shadowCrease) {
            changedListener.eyeshadowCreaseChanged(shadowCrease.enabled, shadowCrease.color, ((float)shadowCrease.seekValues[0])/100f,
                    ((float)shadowCrease.seekValues[1])/100f);
        } else if (currentType == shadowHigh) {
            changedListener.eyeshadowHighChanged(shadowHigh.enabled, shadowHigh.color, ((float)shadowHigh.seekValues[0])/100f,
                    ((float)shadowHigh.seekValues[1])/100f);
        } else if (currentType == blush) {
            changedListener.blushChanged(blush.enabled,blush.color,blush.currentStyle, ((float)blush.seekValues[0])/100f);
        } else if (currentType == brow) {
            changedListener.browChanged(brow.enabled,brow.color, ((float)brow.seekValues[0])/100f);
        } else if (currentType == lipliner) {
            changedListener.liplinerChanged(lipliner.enabled,lipliner.color, ((float)lipliner.seekValues[0])/100f);
        }
    }

    public void showLip(){show(lip);}
    public void showFoundation(){show(foundation);}
    public void showEyeliner(){show(eyeliner);}
    public void showLash() {show(lash);}
    public void showEyeshadowLid() {show(shadowLid);}
    public void showEyeshadowCrease() {show(shadowCrease);}
    public void showEyeshadowHigh() {show(shadowHigh);}
    public void showBlush() {show(blush);}
    public void showBrow() {show(brow);}
    public void showLipliner() {show(lipliner);}

    private void show(MakeupType type) {
        mainContainer.setVisibility(View.VISIBLE);

        currentType = type;
        toggle.setChecked(type.enabled);

        for (int i = 0; i < seekCount; i++) {
            if (type.seekNames[i] != null) {
                seekLabels[i].setVisibility(View.VISIBLE);
                seekBars[i].setVisibility(View.VISIBLE);
                seekLabels[i].setText(type.seekNames[i]);
                seekBars[i].setProgress(type.seekValues[i]);
            } else {
                seekLabels[i].setVisibility(View.GONE);
                seekBars[i].setVisibility(View.GONE);
            }
        }

        if (type.maxStyles > 0) toggleStyle.setVisibility(View.VISIBLE);
        else toggleStyle.setVisibility(View.GONE);

        colorsButton.setCircleColor(type.color);
        for (int i = 0; i < colorsContainer.getChildCount(); i++) {
            if (i < type.colors.length) colorsContainer.getChildAt(i).setVisibility(View.VISIBLE);
            else {
                colorsContainer.getChildAt(i).setVisibility(View.GONE);
                continue;
            }
            ((CircleWidthView) colorsContainer.getChildAt(i)).setCircleColor(type.colors[i]);
        }
    }

    public class MakeupType {
        boolean enabled = false;

        int maxStyles;
        int currentStyle = 0;

        int[] colors;
        int color;

        String[] seekNames = new String[seekCount];
        int[] seekValues = new int[seekCount];

        public MakeupType(int[] colors, int defaultColorIndex, String seekOneName, int seekOneValue,
                          String seekTwoName, int seekTwoValue, String seekThreeName, int seekThreeValue, int maxStyles) {
            this.colors = colors;
            this.color = colors[defaultColorIndex];
            seekNames[0] = seekOneName;
            seekValues[0] = seekOneValue;
            seekNames[1] = seekTwoName;
            seekValues[1] = seekTwoValue;
            seekNames[2] = seekThreeName;
            seekValues[2] = seekThreeValue;
            this.maxStyles = maxStyles;
        }
    }

    MakeupType lip = new MakeupType(new int[]{
            Color.parseColor("#ce6db9"),
            Color.parseColor("#d849a1"),
            Color.parseColor("#c43a6d"),
            Color.parseColor("#b22148"),
            Color.parseColor("#c4333f"),
            Color.parseColor("#cc5151"),
            Color.parseColor("#e0796b")},
            0,
            "opacity", 100,
            "glitter", 0,
            "gloss", 0,
            0);

    MakeupType foundation = new MakeupType(new int[]{
            Color.parseColor("#e5bfae"),
            Color.parseColor("#e0baa1"),
            Color.parseColor("#d6a691"),
            Color.parseColor("#c69f85"),
            Color.parseColor("#bf8d6d"),
            Color.parseColor("#aa7d58")},
            0,
            "tint", 0,
            "smoothing",100,
            null, 0,
            0);

    MakeupType eyeliner = new MakeupType(new int[]{
            Color.parseColor("#040433"),
            Color.parseColor("#370a4c"),
            Color.parseColor("#260130"),
            Color.parseColor("#561239"),
            Color.parseColor("#0f080c"),
            Color.parseColor("#494f5b")},
            0,
            "opacity", 100,
            null,0,
            null, 0,
            4);

    MakeupType lash = new MakeupType(new int[]{
            Color.parseColor("#040433"),
            Color.parseColor("#370a4c"),
            Color.parseColor("#260130"),
            Color.parseColor("#561239"),
            Color.parseColor("#0f080c"),
            Color.parseColor("#494f5b")},
            0,
            "opacity", 100,
            null,0,
            null, 0,
            4);

    MakeupType blush = new MakeupType(new int[]{
            Color.parseColor("#f7b2da"),
            Color.parseColor("#f7b4c3"),
            Color.parseColor("#ffbcb7"),
            Color.parseColor("#ff483a"),
            Color.parseColor("#ffa654"),
            Color.parseColor("#ff5481")},
            0,
            "opacity", 100,
            null,0,
            null, 0,
            4);

    MakeupType brow = new MakeupType(new int[]{
            Color.parseColor("#774507"),
            Color.parseColor("#472514"),
            Color.parseColor("#2d1b15"),
            Color.parseColor("#ddbe7e"),
            Color.parseColor("#a3593c")},
            0,
            "opacity", 100,
            null,0,
            null, 0,
            0);

    MakeupType lipliner = new MakeupType(new int[]{
            Color.parseColor("#ce6db9"),
            Color.parseColor("#d849a1"),
            Color.parseColor("#c43a6d"),
            Color.parseColor("#b22148"),
            Color.parseColor("#c4333f"),
            Color.parseColor("#cc5151"),
            Color.parseColor("#e0796b")},
            0,
            "opacity", 100,
            null,0,
            null, 0,
            0);

    MakeupType shadowLid = new MakeupType(new int[]{
            Color.parseColor("#4286f4"),
            Color.parseColor("#9630c1"),
            Color.parseColor("#a50d4c"),
            Color.parseColor("#994a23"),
            Color.parseColor("#d8c563"),
            Color.parseColor("#337716"),
            Color.parseColor("#33564c")},
            0,
            "opacity", 100,
            "glitter",0,
            null, 0,
            0);

    MakeupType shadowCrease = new MakeupType(new int[]{
            Color.parseColor("#4286f4"),
            Color.parseColor("#9630c1"),
            Color.parseColor("#a50d4c"),
            Color.parseColor("#994a23"),
            Color.parseColor("#d8c563"),
            Color.parseColor("#337716"),
            Color.parseColor("#33564c")},
            0,
            "opacity", 100,
            "glitter",80,
            null, 0,
            0);

    MakeupType shadowHigh = new MakeupType(new int[]{
            Color.parseColor("#4286f4"),
            Color.parseColor("#9630c1"),
            Color.parseColor("#a50d4c"),
            Color.parseColor("#994a23"),
            Color.parseColor("#d8c563"),
            Color.parseColor("#337716"),
            Color.parseColor("#33564c")},
            0,
            "opacity", 80,
            "glitter",50,
            null, 0,
            0);

}
