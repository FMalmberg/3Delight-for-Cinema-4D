CONTAINER oincandescencelight
{
	NAME oincandescencelight;
	INCLUDE Obase;

	GROUP ID_OBJECTPROPERTIES
	{
		COLOR INCANDESCENCE_LIGHT_COLOR{ANIM OFF;}
		REAL INCANDESCENCE_LIGHT_INTENSITY {MIN 0; MINSLIDER 0; MAXSLIDER 10; CUSTOMGUI REALSLIDER;}
		REAL INCANDESCENCE_LIGHT_EXPOSURE {MIN -5; MINSLIDER 0; MAXSLIDER 10; CUSTOMGUI REALSLIDER;}
		IN_EXCLUDE INCANDESCENCE_LIGHT_GEOMETRY {ANIM OFF;}
	}
}
