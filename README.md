# FovealRendering
A simple foveal rendering system using DirectX

# Foveal Rendering Level of Detail
### Eric Knapik & Vic McGowen

The goal is to improve rendering time of a complex scene by only rendering what the user is actually looking at. Using an eye tracker, the program will be able to determine the exact parts of the scene the user is looking at and then fully render geometry within the user’s focus. <br />
The fovea is a small region in the center retina that is responsible for sharp, accurate vision. The fovea has the largest concentration of cones, which are in turn responsible for color vision. As the fovea provides the sharpest vision where it is pointed towards (i.e. where the individual is looking at), the individual's visual acuity is greatest at point of focus with a quick fall-off in the periphery. <br />
Using the knowledge of how the human visual system works the program will be able to choose a less geometrically complex mesh to render in those regions where there is less visual acuity. To calculate the less geometrically complex meshes the program will implement a static or dynamic algorithm for determining the level of detail that a particular mesh needs to be. Programs in the past have utilized static and dynamic algorithms for determining mesh level of detail with respect to distance from the user, where meshes closer to the user have a higher level of detail than meshes at a distance. This program will apply similar distance based level of detail algorithms and apply them to the region of the scene that is within the user’s foveal focus. Meaning meshes within the user’s foveal focus will have a higher level of detail than meshes outside of the user’s foveal focus. <br />

# Usage Guide
Step 1... < br />
Step 2... < br />
Step 3... < br />

# Demo Video
[![demo](https://github.com/EKnapik/FovealRendering/blob/master/documentation/youtube.png)](https://youtu.be/5cbNQI526aU)

# References
G. Brian, et al. "Foveated 3D graphics." ACM Transactions on Graphics (TOG) 31 (6), pp. 164 
(2012).
H. Murphy, and A. T. Duchowski, "Gaze-contingent level of detail rendering," EuroGraphics 
2001, (2001).
M. Reddy, "Musings on Volumetric Level of Detail for Virtual Environments," Virtual Reality: 
Research, Development and Application, 1 (1), pp. 49-56 (1995).
M. Reddy, “Visual Perception and LOD, ” ACM SIGGRAPH, (2002)

# Timeline
Week 6: Class update presentation <br />
	  Eye Tracking Integration <br />
Week 7-8: Level of Detail Implemented <br />
	Scene graph. The rendering system inspects and can interpret a scene graph <br />
Week 9: Mid-semester presentations <br />
Week 10-13: Combined Level of Detail with eye tracking <br />
Week 14: Website finalization <br />
Week 15-16: Presentations <br />

# Roles
Engine Framework
	Model Loading
	Scene Layout (Vic)
	DirectX Rendering (Eric)
Eye Tracking Integration (Eric)
Level of Detail Algorithm (distance based) (Vic)
Foveal Level of Detail choosing (eye tracking based)
	Meshes within foveal projection (Vic)
Website/Final document (Both)


# Final Paper
Download final paper PDF [Here](https://github.com/EKnapik/FovealRendering/documentation/foveal-rendering.pdf) or continue to scroll
![page 1](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-1.png)
![page 2](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-2.png)
![page 3](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-3.png)
![page 4](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-4.png)
![page 5](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-5.png)
![page 6](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-6.png)
![page 7](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-7.png)
![page 8](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-8.png)
![page 9](https://github.com/EKnapik/FovealRendering/blob/master/documentation/foveal-rendering/foveal-rendering-9.png)
