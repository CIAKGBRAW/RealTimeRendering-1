package com.astromedicomp.Hello_Two;

import android.widget.TextView;
import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;

public class MyView extends TextView{
  
    MyView(Context context)
    {
	super(context);
	
	setText("Hello Galaxy!!!");
	setTextSize(60);
	setTextColor(Color.GREEN);
	setGravity(Gravity.CENTER);
    }


}