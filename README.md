# 3Delight-for-Cinema-4D
A 3Delight rendering plugin for MAXON Cinema 4D. To download the latest version, please visit the <a href="https://github.com/FMalmberg/3Delight-for-Cinema-4D/releases">releases</a> page

## Build instructions
These instructions assume that you are already familiar with building plugins for Cinema 4D. If not, please see www.plugincafe.com for general information about Cinema 4D plugin development. It is also assumed that you have already installed the 3Delight renderer itself (www.3delight.com). 

3Delight for Cinema 3D consists of four separate Cinema 4D plugins: 

- 3DLfC4D
- 3DLfC4D_geometry
- 3DLfC4D_shaders
- 3DLfC4D_lights. 

For each of these plugins, create a Cinema 4D plugin build project for your desired Cinema 4D version and platform. For each plugin project, additionally perform the following steps:

- Add $DELIGHT/include to the include path of the project.
- Add $DELIGHT/lib to the library path of the project, and add 3Delight.lib as a project dependency. 
- Add ...3DLfC4D/API/include to the path of the project.
    
Now, you should be able to build all four projects. 
