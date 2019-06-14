DIALOG MULTILIGHT_SELECTOR
{
    FIT_H; FIT_V; SCALE_V; SCALE_H;

	GROUP
	{		
		COLUMNS 1;
		FIT_H; SCALE_H;
		BORDERSIZE 50,0,50,0;
		GROUP
		{
			FIT_H; SCALE_H;
			BORDERSIZE 100,5,100,0;
			TREEVIEW DL_MULTI_LIGHT_TREEVIEW {FIT_H; SCALE_H; SIZE -300,-150; BORDER; HIDE_LINES;};
		}
		
		GROUP
		{
			BORDERSIZE 0,10,0,0;
			CENTER_H;
			CHECKBOX DL_DISPLAY_ALL_LIGHTS {NAME DL_DISPLAY_LIGHTS;};
		}
		GROUP
		{
			BORDERSIZE 0,5,0,0;
			CENTER_H;
			BUTTON DL_LIGHTS_REFRESH {NAME DL_LIGHTS_REFRESH_NAME;};
		}
	}
}