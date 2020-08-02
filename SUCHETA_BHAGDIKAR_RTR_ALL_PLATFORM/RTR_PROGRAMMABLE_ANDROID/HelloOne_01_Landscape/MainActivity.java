package com.astromedicomp.Hello_One;

import android.app.Activity;
import android.os.Bundle;

import android.widget.TextView;
import android.graphics.Color;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.view.Gravity;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

       //Removing TitleBar same as we remove caption in windows
       this.requestWindowFeature(Window.FEATURE_NO_TITLE);

      //Making FullScreen
      this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN); 
	 
      //Make Orientation Landscape
      MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

     //Set background Color to BLACK
     getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));

     //Our OwnView
     TextView myTextView  = new TextView(this);

      myTextView.setText("Hello World!!!");
      myTextView.setTextSize(60);
      myTextView.setTextColor(Color.GREEN);
      myTextView.setGravity(Gravity.CENTER);
  

       setContentView(myTextView);
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
