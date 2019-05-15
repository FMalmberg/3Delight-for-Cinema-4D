CONTAINER dl_open_vdb_node
{
	NAME dl_open_vdb_node;

	GROUP OPENVDB_NODE
	{
		GROUP FILE_GROUP
		{
			DEFAULT 1;
			FILENAME FILE_FILENAME
			{
			  ANIM OFF;
			}
			
			BOOL FILE_USE_FILE_SEQUENCE {ANIM OFF;}
			LONG FILE_INDEX_RANGE_START{ANIM OFF;}
			LONG FILE_INDEX_RANGE_END{ANIM OFF;}
			
			LONG FILE_OUTSIDE_RANGE
			{
				ANIM OFF;
				CYCLE
				{
					FILE_OUTSIDE_RANGE_HOLD_FIRST;
					FILE_OUTSIDE_RANGE_REPEAT;
				}
			}
			
			LONG FILE_START_OFFSET {ANIM OFF;}
			LONG FILE_INCREMENT {ANIM OFF;}
			BOOL FILE_CONFINE_TO_FRAME_RANGE {ANIM OFF;}
			GROUP
			{
				LAYOUTGROUP; COLUMNS 2;
				GROUP
				{
					LONG FILE_FRAME_RANGE_START {ANIM OFF;}
				}

				GROUP
				{
					LONG FILE_FRAME_RANGE_END {ANIM OFF;}
				}
			}
			
		}
		
		GROUP GRIDS_GROUP
		{
			DEFAULT 1;
			LONG GRIDS_GROUP_SMOKE
			{
				ANIM OFF;
				CYCLE
				{
					GRIDS_GROUP_SMOKE_NONE;
				}
			}
			
			LONG GRIDS_GROUP_TEMPERATURE
			{
				ANIM OFF;
				CYCLE
				{
					GRIDS_GROUP_TEMPERATURE_NONE;
				}
			}
			
			LONG GRIDS_GROUP_EMISSION_INTENSITY
			{
				ANIM OFF;
				CYCLE
				{
					GRIDS_GROUP_EMISSION_INTENSITY_NONE;
				}
			}
			
			LONG GRIDS_GROUP_VELOCITY
			{
				ANIM OFF;
				CYCLE
				{
					GRIDS_GROUP_VELOCITY_NONE;
				}
			}
			REAL GRIDS_GROUP_VELOCITY_SCALE{ANIM OFF;}
		}
		
		GROUP OBJECT_DISPLAY_GROUP
		{
			DEFAULT 1;
			BOOL OBJECT_DISPLAY_DRAW_POINTS{ANIM OFF;}
		}
	
		GROUP VISIBILITY_GROUP
		{
			DEFAULT 1;
			BOOL VISIBILITY_VISSIBLE_IN_DIFFUSE{ANIM OFF;}
		}
	}
	
	GROUP OPENVDB_SHADER_GROUP
	{
		GROUP SCATTERING_GROUP
		{
			DEFAULT 1;
			REAL SCATTERING_DENSITY{ANIM OFF;}
			COLOR SCATTERING_COLOR {ANIM OFF;}
			SHADERLINK SCATTERING_COLOR_SHADER{ANIM OFF;}	
			REAL SCATTERING_ANISOTROPY{MIN -1; MAX 1; MINSLIDER -1; MAXSLIDER 1; STEP 0.005; CUSTOMGUI REALSLIDER;}
			SHADERLINK SCATTERING_ANISOTROPY_SHADER{ANIM OFF;}	
			BOOL SCATTERING_MULTIPLE_SCATTERING{ANIM OFF;}	
		}
		
		GROUP DENSITY_RAMP_GROUP
		{
			DEFAULT 1;
			BOOL DENSITY_RAMP_ENABLE{ANIM OFF;}
			GROUP
			{
				LAYOUTGROUP; COLUMNS 2;
				GROUP
				{
					REAL DENSITY_RAMP_RANGE_START{ANIM OFF;}
				}

				GROUP
				{
					REAL DENSITY_RAMP_RANGE_END{ANIM OFF;}
				}
			}
			
			
			SPLINE DENSITY_RAMP_RAMP
			{
			  SHOWGRID_H;
			  SHOWGRID_V;
			  EDIT_H;
			  EDIT_V;
			  NO_FLOATING_WINDOW;
			  NOPRESETS;
			}
		}
		GROUP TRANSPARENCY_GROUP 
		{
			DEFAULT 1;
			COLOR TRANSPARENCY_COLOR {ANIM OFF;}
			SHADERLINK TRANSPARENCY_COLOR_SHADER {ANIM OFF;}
			REAL TRANSPARENCY_SCALE{MIN 0; MINSLIDER 0; STEP 0.1; CUSTOMGUI REALSLIDER;}

		}
		
		GROUP EMMISSION_GROUP
		{
			DEFAULT 1;
			REAL EMMISSION_SCALE{MIN 0; MINSLIDER 0; STEP 0.1; CUSTOMGUI REALSLIDER;}
		}
	
		GROUP INTENSITY_GROUP
		{
			DEFAULT 1;
			BOOL INTENSITY_USE_GRID{ANIM OFF;}
			GROUP
			{
				LAYOUTGROUP; COLUMNS 2;
				GROUP
				{
					REAL INTENSITY_RANGE_START{ANIM OFF;}
				}

				GROUP
				{
					REAL INTENSITY_RANGE_END{ANIM OFF;}
				}
			}
			
			SPLINE INTENSITY_RAMP
			{
			  SHOWGRID_H;
			  SHOWGRID_V;
			  EDIT_H;
			  EDIT_V;
			  NO_FLOATING_WINDOW;
			  NOPRESETS;
			}
		}
		
		GROUP BLACKBODY_GROUP
		{
			DEFAULT 1;
			REAL BLACKBODY_INTENSITY{ANIM OFF;}
			LONG BLACKBODY_MODE
			{
				ANIM OFF;
				CYCLE
				{
					BLACKBODY_PHYSICALLY_CORRECT;
					BLACKBODY_NORMALIZED;
					BLACKBODY_ARTISTIC;
				}
			}
			REAL BLACKBODY_KELVIN{ANIM OFF; MIN 0; MINSLIDER 5000; STEP 10; CUSTOMGUI REALSLIDER;}
			COLOR BLACKBODY_TINT{ANIM OFF;}
			GROUP
			{
				LAYOUTGROUP; COLUMNS 2;
				GROUP
				{
					REAL BLACKBODY_RANGE_START {ANIM OFF;}
				}

				GROUP
				{
					REAL BLACKBODY_RANGE_END {ANIM OFF;}
				}
			}
			
			SPLINE BLACKBODY_RAMP
			{
			  SHOWGRID_H;
			  SHOWGRID_V;
			  EDIT_H;
			  EDIT_V;
			  NO_FLOATING_WINDOW;
			}
		}
		
		GROUP RAMP_GROUP
		{
			DEFAULT 1;
			REAL RAMP_INTENSITY {ANIM OFF;}
			COLOR RAMP_TINT {ANIM OFF;}
			GROUP
			{
				LAYOUTGROUP; COLUMNS 2;
				GROUP
				{
					REAL RAMP_RANGE_START {ANIM OFF;}
				}

				GROUP
				{
					REAL RAMP_RANGE_END {ANIM OFF;}
				}
			}
			GRADIENT EMISSION_RAMP
			{ANIM OFF; COLOR;}
		}
	}
}
