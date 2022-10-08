# Spark - An OpenGL Renderer

Spark is an hobby project of a Bachelor student in AI with a passion in Computer Graphics, as such, it is built incrementally as my knowledge of CG and C++17 increases, in whatever free slots of time I can find from my classes and other activities.

The application, in its current state, is capable of rendering any Wavefront .obj file with a rudimentary lightning model, changes to the scene have to be coded in the project itself, in the future, I hope to make it possible to alter the environment through a GUI.

## Major Features

* RAII principles respected in most of the application, not a single `new` in sight.
* Localized lightning model featuring Gaussian specular highlights, normal and specular mapping and physically present lights.
* Objects can be moved around the scene, scaled, and rotated and the lightning will change accordingly.
* The possibility to import *any* .obj file and have it rendered in the scene.
* A Series of abstractions for GL constructs such as VBOs, EBOs and VAOs that make it possible to prepare data for rendering in an easier way.
* A VAO based rendering scheme that reduces the amount of binds-per-draw, improving performance
* Instance Based rendering that greatly reduces the amount of VRAM required (just one Buffer per object model).

## TO-DOs

There is a great need of an architectural refactoring that would allow the application to scale up in terms of complexity and usability:

- [ ] User-Friendly GUIs that allows modification of uniforms
- [ ] A more structured and data-driven rendering pipeline, consisting of:
    - [ ] A `SparkObject` abstraction that wraps any object that takes part in a rendering call (models, textures, etc...)
    - [ ] A Scene Tree that can be iterated in a Depth-First-Search like mechanism that allows us to perform bindings hierarchically, saving resources and greatly simplifying the rendering process, moreover, the possibility of storing this tree in a separate file
    - [ ] A `Renderer` class and a `ResourceManager` class to abstract away complex behaviour such as importing objects or rendering a scene
- [ ] Integration of the new rendering pipeline with the GUI

## Visuals
Here we show some screens of the current iteration of Spark at work, note that the lack of skybox makes the scenarios look *pretty* empty:

#### Around a thousand cubes floating around two light sources
![cubes](https://user-images.githubusercontent.com/77234595/194707722-76b33d2a-abc4-4e7c-862e-0bb255d475b8.png)
*Here the cubes' material properties have been altered to give them an uncharacteristic shininess, note the specular highlight **leaking** through the cube's edge.*

#### An imported .obj model of a Robot

![robot](https://user-images.githubusercontent.com/77234595/194708308-2f649f9d-8e58-46be-8995-499cb16a7fec.png)
*The robot stands with two lights overhead, the ambient light has been tinted a slight shade of yellow and the robot's shininess has been tuned way up.*

## Credits

The main resources for building this project have been: 
1. TheCherno's OpenGL tutorial, which was originally the foundation of this project before the `custom-refactor` branch: https://thecherno.com/opengl 
2. The ubiquitous LearnOpenGL website, which provides lots of resources for beginners: https://learnopengl.com/Model-Loading/Model

## Third Party Libraries:
In order to work, Spark delegates some functions to outside third party libraries, namely:
* [Tiny Obj Loader](https://github.com/tinyobjloader/tinyobjloader) to help with reading .obj files.
* [`stb_image.h`](https://github.com/nothings/stb/blob/master/stb_image.h) in order to help loading textures.

