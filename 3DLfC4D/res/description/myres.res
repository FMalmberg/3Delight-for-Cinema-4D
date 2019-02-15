CONTAINER myres
{
	NAME myres;
	INCLUDE VPbase;
	INCLUDE GVbase;

	LONG DL_RENDER_LAYER
	{
		ANIM OFF;
		CYCLE
		{
			DL_MASTER_LAYER;
			DL_LAYER_1;
			DL_LAYER_2;
		}
	}

		LONG DL_ACTIVE_RENDER_SETTINGS
		{
			ANIM OFF;
			CYCLE
			{
				DL_RENDER_SETTINGS_1;
				DL_RENDER_SETTINGS_2;
			}

		}




		GROUP DL_GROUP_QUALITY
		{
		
			GROUP
			{
				COLUMNS 2;
				SCALE_H; FIT_H;
				BUTTON DL_CREATE_RENDER_SETTINGS
				{
					ANIM OFF;
				}
				
				BUTTON DL_EXPORT_RENDER_SETTINGS
				{
					ANIM OFF;
				}
			}
			
			STATICTEXT DL_ISCLICKED { ANIM OFF; HIDDEN; }
			FILENAME DL_FOLDER_OUTPUT{ANIM OFF; HIDDEN;}
			
			LONG DL_SHADING_SAMPLES
			{
				ANIM OFF;
				MIN 0;
				MINSLIDER 0;
				MAXSLIDER 100;
				CUSTOMGUI LONGSLIDER;
			}

			LONG DL_PIXEL_SAMPLES
			{
				ANIM OFF;
				MIN 0;
				MAX 256;
				MINSLIDER 0;
				CUSTOMGUI LONGSLIDER;

			}

			LONG DL_VOLUME_SAMPLES
			{	ANIM OFF;
				MIN 0;
				MINSLIDER 0;
				MAXSLIDER 256;
				CUSTOMGUI LONGSLIDER;
			}

			SEPARATOR {LINE; }


			LONG DL_PIXEL_FILTER
			{
				ANIM OFF;
				CYCLE
				{
					DL_BLACKMAN_HARRIS;
					DL_MITCHELL;
					DL_CATMULL_ROM;
					DL_SINC;
					DL_BOX;
					DL_TRIANGLE;
					DL_GAUSSIAN;

				}
			}

			REAL DL_FILTER_WIDTH
			{
				ANIM OFF;
				MIN 0;
				MINSLIDER 0;
				MAXSLIDER 10;
				STEP 0.1;
				CUSTOMGUI REALSLIDER;
			}


			SEPARATOR {LINE; }


			BOOL DL_MOTION_BLUR
			{
				ANIM OFF;
			}


			SEPARATOR {LINE; }

			LONG DL_MAX_DIFFUSE_DEPTH
			{
				ANIM OFF;
			}

			LONG DL_MAX_REFLECTION_DEPTH
			{
				ANIM OFF;
			}

			LONG DL_MAX_REFRACTION_DEPTH
			{
				ANIM OFF;
			}

			LONG DL_MAX_HAIR_DEPTH
			{
				ANIM OFF;
			}

			REAL DL_MAX_DISTANCE
			{
				ANIM OFF;
			}


		}


		//GROUP DL_SCENE_ELEMENTS
		//{

		//	LONG DL_CAMERA_VIEW
		//	{
		//		ANIM OFF;
		//		CYCLE
		//		{
		//			DL_CAMERA_NONE;
		//			DL_CAMERA_FRONTSHAPE;
		//			DL_CAMERA_PERPSHAPE;
		//			DL_CAMERA_SIDESHAPE;
		//			DL_CAMERA_TOPSIDE;
		//		}
		//	}

		//	LONG DL_ENVIRONMENT_VIEW
		//	{
		//		ANIM OFF;
		//		CYCLE
		//		{
		//			DL_ENVIRONMENT_NONE;
		//			DL_ENVIRONMENT_SHAPE1;
		//			DL_ENVIRONMENT_SHAPE2;
		//			DL_ENVIRONMENT_SHAPE3;

		//		}
		//	}

		//	LONG DL_ATMOSPHERE_VIEW
		//	{
		//		ANIM OFF;
		//		CYCLE
		//		{
		//			DL_ATMOSPHERE_NONE;
		//			DL_ATMOSPHERE_SHAPE1;
		//		}
		//	}

		//	LONG DL_OBJECTS_TO_RENDER_VIEW
		//	{
		//		ANIM OFF;
		//		CYCLE
		//		{
		//			DL_OBJECTS_TO_RENDER_ALL;
		//		}
		//	}

		//	LONG DL_LIGHTS_TO_RENDER_VIEW
		//	{
		//		ANIM OFF;
		//		CYCLE
		//		{
		//			DL_LIGHTS_TO_RENDER_ALL;
		//		}
		//	}

		//}

		//GROUP DL_GROUP_FRAME_RANGE
		//{
		//	BOOL DL_FRAME_RANGE_CHECK  {ANIM OFF; }


		//	GROUP
		//	{

		//		LAYOUTGROUP;
		//		COLUMNS 2;
		//		GROUP
		//		{
		//			REAL DL_FRAME_RANGE
		//			{
		//				ANIM OFF;
		//				STEP 0.001;
		//			}
		//		}

		//		GROUP
		//		{
		//			REAL DL_FRAME_RANGE_MAX
		//			{
		//				ANIM OFF;
		//				STEP 0.001;

		//			}
		//		}

		//	}

		//	REAL DL_FRAME_INCREMENT
		//	{
		//		ANIM OFF;
		//		STEP 0.001;

		//	}

		//}



		//GROUP DL_GROUP_IMAGES_AND_CROP
		//{
		//	BOOL DL_USE_RENDER_IMAGE_SIZE  {ANIM OFF; }

		//	SEPARATOR {LINE; }

		//	BOOL DL_IMAGE_CROP_CHECK  {ANIM OFF; }

		//	GROUP
		//	{

		//		LAYOUTGROUP;
		//		COLUMNS 2;
		//		GROUP
		//		{
		//			REAL DL_CROP_MIN_FROM
		//			{
		//				ANIM OFF;
		//				STEP 0.001;

		//			}
		//		}

		//		GROUP
		//		{
		//			REAL DL_CROP_MIN_TO
		//			{
		//				PARENTID DL_CROP_MIN_FROM;
		//				ANIM OFF;
		//				STEP 0.001;
		//			}
		//		}

		//	}

		//	GROUP
		//	{

		//		LAYOUTGROUP;
		//		COLUMNS 2;
		//		GROUP
		//		{
		//			REAL DL_CROP_MAX_FROM
		//			{
		//				ANIM OFF;
		//				STEP 0.001;
		//			}
		//		}

		//		GROUP
		//		{
		//			REAL DL_CROP_MAX_TO
		//			{
		//				ANIM OFF;
		//				STEP 0.001;
		//			}
		//		}

		//	}

		//}

		GROUP DL_GROUP_IMAGE_LAYERS
		{
							SCALE_H;FIT_H;

		GROUP{
							SCALE_H;FIT_H;

				FILENAME DL_DEFAULT_IMAGE_FILENAME
				{
				  ANIM OFF;
				  DIRECTORY;	
				}

				
					LONG DL_DEFAULT_IMAGE_FORMAT
					{
						ANIM OFF;
						CYCLE
						{
							DL_FORMAT_TIFF;
							DL_FORMAT_OPEN_EXR;
							DL_FORMAT_OPEN_EXR_DEEP;
							DL_FORMAT_JPEG;
							DL_FORMAT_PNG;
						}
					}
					
					LONG DL_DEFAULT_IMAGE_BITDEPTH
					{
						ANIM OFF;
						HIDDEN;	
						CYCLE
						{
							DL_EIGHT_BIT;
							DL_SIXTEEN_BIT;
							DL_SIXTEEN_BIT_FLOAT;
							DL_THIRTYTWO_BIT;
						}
					}
					
					LONG DL_DEFAULT_IMAGE_OUTPUT
					{
						ANIM OFF;
						CYCLE
						{
							DL_SIXTEEN_BIT_FLOAT;
							DL_THIRTYTWO_BIT;
						}
					}
					
					
					LONG DL_DEFAULT_IMAGE_OUTPUT_TIFF
					{
						ANIM OFF;
						HIDDEN;
						CYCLE
						{
							DL_EIGHT_BIT;
							DL_SIXTEEN_BIT;
							DL_THIRTYTWO_BIT;
						}
					}
					
					LONG DL_DEFAULT_IMAGE_OUTPUT_JPG
					{
						ANIM OFF;
						HIDDEN;
						CYCLE
						{
							DL_EIGHT_BIT;
						}
					}
					
					LONG DL_DEFAULT_IMAGE_OUTPUT_PNG
					{
						ANIM OFF;
						HIDDEN;
						CYCLE
						{
							DL_EIGHT_BIT;
							DL_SIXTEEN_BIT;
						}
					}
				
				

				

				BOOL DL_SAVE_IDS_AS_CRYPTOMATTE
				{
					ANIM OFF;
				}

				LONG DL_BATCH_OUTPUT_MODE
				{
					ANIM OFF;
					CYCLE
					{
						DL_ENABLE_AS_SELECTED;
						DL_ENABLE_ALL;
					}
				}

				LONG DL_INTERACTIVE_OUTPUT_MODE
				{
					ANIM OFF;
					CYCLE
					{
						DL_ENABLE_AS_SELECTED_INTERACTIVE;
						DL_ENABLE_ONLY_FOR_SELECTED;
						DL_DISABLE_FILE_OUTPUT;
					}
				}

				SEPARATOR {LINE; }

				DYNAMIC GV_ABS_INPUT
				{
					INPORT;
					CREATEPORT 6;
					MINPORTS 5;
					MAXPORTS 10;
				}
				DYNAMIC GV_ABS_OUTPUT
				{
					OUTPORT;
					CREATEPORT;
				}
			}
			
			GROUP
			{
				
				DELIGHTLAYERCUSTOMTYPE DL_CUSTOM_AOV_LAYER
				{
				ANIM OFF;
				CUSTOMGUI DELIGHTLAYERCUSTOMGUI;
				}
			}
			

		}

		GROUP DL_GROUP_OVERRIDES
		{
			BOOL DL_ENABLE_INTERACTIVE_PREVIEW
			{
				ANIM OFF;
			}

			BOOL DL_DISABLE_MOTION_BLUR
			{
				ANIM OFF;
			}

			BOOL DL_DISABLE_DEPTH_OF_FIELD
			{
				ANIM OFF;
			}
			BOOL DL_DISABLE_DISPLACEMENT
			{
				ANIM OFF;
			}
			BOOL DL_DISABLE_SUBSURFACE
			{
				ANIM OFF;
			}

			LONG DL_RESOLUTION
			{
				ANIM OFF;
				CYCLE
				{
					DL_FULL;
					DL_HALF;
					DL_QUARTER;
					DL_EIGHTH;
				}

			}

			LONG DL_SAMPLING
			{
				ANIM OFF;
				CYCLE
				{
					DL_HUNDRED_PERCENT;
					DL_TWENTYFIVE_PERCENT;
					DL_TEN_PERCENT;
					DL_FOUR_PERCENT;
					DL_ONE_PERCENT;
				}

			}

		}

		//GROUP DL_GROUP_EXTRA_ATTRIBUTES
		//{
		//	REAL DL_PIXEL_ASPECT_RATIO
		//	{
		//		STEP 0.001;
		//	}
		//}



}