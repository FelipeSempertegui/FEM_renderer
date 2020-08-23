# Summary:
This project was built to both simulate the dynamic behavior of a human middle ear model via the Finite-Element-Method and to visualize the generated results via the OpenGL graphics API. 

The FEM solver was developed, so it could include the following peculiarities of a human middle ear:
* Orthotropic material properties of the eardrum’s radial and circumferential fibers, whose directions are taken into account by including a local coordinate system
* Distinct domains (eardrum, malleus, incus, stapes, etc) with their respective material properties
* Complex-valued global matrices to account for the domains’ internal damping and the impedance of the cochlear fluid

The OpenGL was used set, so it could display animations of the selected vibration patterns, while the user rotates the model with the mouse. These animations are comprised with the deformed mesh of the model, “thermal” colors displaying the displacement of the mesh and phong reflection on the model’s surface. 
Lastly, openMP was used to speed up the computation of the element’s matrices and post-processing of the displacement results.

# This program can be subdivided into 5 main steps:

1. A middle ear mesh (GMSH) based on the works of Greef et al. is imported together with model’s several material properties (Young’s moduli, densities & internal damping loss factor). The GMSH file contains the information regarding the mesh’s domains (i.e., eardrum, malleus, incus, etc) and a group of boundaries conditions. The latter can be divided into 3 distinct boundary types:
   * Fixed boundary conditions: located at the outer circumference of the eardrum and the end of both tendons and ligaments
   * Uniform & harmonic excitation pressures: set at the boundaries describing the lateral region of the eardrum. It simulates the sound stimuli that reaches that membrane
   * Damping: the acoustic resistance of the cochlear fluid (which damps the stapes movement) is set at surface of the stapes footplate.
