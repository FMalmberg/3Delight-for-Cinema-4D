CONTAINER Tmotionsamples
{
	NAME Tmotionsamples;
	INCLUDE Tbase;
	
	GROUP ID_TAGPROPERTIES
	{
		BOOL USE_TRANSFORMATION_BLUR{}
		LONG TRANSFORMATION_EXTRA_SAMPLES{MIN 0;}
		BOOL USE_DEFORMATION_BLUR{}
		LONG DEFORMATION_EXTRA_SAMPLES{MIN 0;}
			
	}
	
	
}
