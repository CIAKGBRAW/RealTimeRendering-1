package com.astromedicomp.Hello_Events;

import android.app.Activity;
import android.os.Bundle;
import android.graphics.Color;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;

public class MainActivity extends Activity
{

	private MyView myview;
	
	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		//to get rid of actionBar
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		//this is done to make fullscreen
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);

		super.onCreate(savedInstanceState);

		MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
	
		myview=new MyView(this);

		//set background color to black
		   getWindow().getDecorView().setBackgroundColor(Color.rgb(1,1,1));


		//set view as content view of the activity
		setContentView(myview);		
	}
	@Override
	protected void onPause()
	{
		super.onPause();
	}
	@Override 
	protected void onResume()
	{
		super.onResume();
	}

}