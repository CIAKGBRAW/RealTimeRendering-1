package com.astromedicomp.Hello_Two;

import android.app.Activity;
import android.os.Bundle;

import android.widget.TextView;
import android.graphics.Color;
import android.view.Window;
import android.view.WindowManager;
import android.content.pm.ActivityInfo;
import android.view.Gravity;

public class MainActivity extends Activity
{

    private MyView myView;

     @Override
    protected void onCreate(Bundle savedInstanceState)
  {
     super.onCreate(savedInstanceState);

     //Removing TitleBar 
     this.requestWindowFeature(Window.FEATURE_NO_TITLE);

     //Making FullScreen
     this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,WindowManager.LayoutParams.FLAG_FULLSCREEN);

    //Make Orientation Landscape
     MainActivity.this.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
  
     //Set Background Color to Black
      getWindow().getDecorView().setBackgroundColor(Color.rgb(0,0,0));

      myView=new MyView(this);
    
      setContentView(myView);
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