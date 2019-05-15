# 3Delight-for-Cinema-4D
A 3Delight rendering plugin for MAXON Cinema 4D.

## Build instructions
These instructions assume that you are already familiar with building plugins for Cinema 4D. If not, please see www.plugincafe.com for general information about Cinema 4D plugin development. It is also assumed that you have already installed the 3Delight renderer itself (www.3delight.com). 

3Delight for Cinema 3D consists of four separate Cinema 4D plugins: 

- 3DLfC4D
- 3DLfC4D_geometry
- 3DLfC4D_shaders
- 3DLfC4D_lights. 

Each of these plugins comes with a "projectdefinition.txt" file that can be used together with the Cinema 4d project tool to generate build solutions for your desired platform. 

To run the plugin after compilation, you also need to place a copy of "3delight.dll" in the same folder as the Cinema 4d executable.
