package com.mobili.usbcamera.view;

import android.view.View;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Path;
import android.util.AttributeSet;

import java.util.ArrayList;
import java.util.List;

public class OverlayView extends View {

    private Paint paint_x;
    private Paint paint_y;
    private Paint paint_z;

    private Path axis_x = new Path();
    private Path axis_y = new Path();
    private Path axis_z = new Path();

    private float innerImageHeight = 0;
    public void setCenterImageHeight(int height) {
      innerImageHeight = height;
    }

    public OverlayView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }

    private void init() {
        paint_x = new Paint();
        paint_x.setColor(0xFFFF0000); // Red color
        paint_x.setStyle(Paint.Style.STROKE);
        paint_x.setStrokeWidth(5); // Set the stroke width
        paint_y = new Paint();
        paint_y.setColor(0xFF00FF00); // Green color
        paint_y.setStyle(Paint.Style.STROKE);
        paint_y.setStrokeWidth(5); // Set the stroke width
        paint_z = new Paint();
        paint_z.setColor(0xFF0000FF); // Blue color
        paint_z.setStyle(Paint.Style.STROKE);
        paint_z.setStrokeWidth(5); // Set the stroke width
    }

    private float transformX(float x) {
      return getWidth() * x;
    }

    private float transformY(float y) {
      return innerImageHeight * y + (getHeight() - innerImageHeight) * 0.5f;
    }

    public void addLineX(float startXPercent, float startYPercent, float endXPercent, float endYPercent) {
        axis_x.reset();
        axis_x.moveTo(transformX(startXPercent), transformY(startYPercent));
        axis_x.lineTo(transformX(endXPercent), transformY(endYPercent));
    }
    public void addLineY(float startXPercent, float startYPercent, float endXPercent, float endYPercent) {
        axis_y.reset();
        axis_y.moveTo(transformX(startXPercent), transformY(startYPercent));
        axis_y.lineTo(transformX(endXPercent), transformY(endYPercent));
    }
    public void addLineZ(float startXPercent, float startYPercent, float endXPercent, float endYPercent) {
        axis_z.reset();
        axis_z.moveTo(transformX(startXPercent), transformY(startYPercent));
        axis_z.lineTo(transformX(endXPercent), transformY(endYPercent));
        invalidate(); // Redraw the view
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        canvas.drawPath(axis_x, paint_x);
        canvas.drawPath(axis_y, paint_y);
        canvas.drawPath(axis_z, paint_z);
    }
}
