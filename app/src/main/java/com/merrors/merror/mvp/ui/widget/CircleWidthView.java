package com.merrors.merror.mvp.ui.widget;

import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.PaintDrawable;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by Jeff on 2017-08-08.
 */

public class CircleWidthView extends View {
    public CircleWidthView(Context context) {
        super(context);
        init();
    }

    public CircleWidthView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    public CircleWidthView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init();
    }

    private void init() {
        ColorDrawable buttonColor = (ColorDrawable) getBackground();
        if (buttonColor == null) return;
        int color = buttonColor.getColor();

        PaintDrawable mDrawable = new PaintDrawable();
        mDrawable.getPaint().setColor(color);
        mDrawable.setCornerRadius(10000);
        setBackground(mDrawable);
    }

    public void setCircleColor(int color) {
        PaintDrawable mDrawable = new PaintDrawable();
        mDrawable.getPaint().setColor(color);
        mDrawable.setCornerRadius(10000);
        setBackground(mDrawable);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec){
        heightMeasureSpec = MeasureSpec.makeMeasureSpec(MeasureSpec.getSize(widthMeasureSpec), MeasureSpec.getMode(widthMeasureSpec));
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }
}
