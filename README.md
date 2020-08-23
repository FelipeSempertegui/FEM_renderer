# Summary:
This project was built to both simulate the dynamic behavior of a human middle ear model via the Finite-Element-Method and to visualize the generated results via the OpenGL graphics API. 

The FEM solver was developed, so it could include the following peculiarities of a human middle ear:
* Orthotropic material properties of the eardrum’s radial and circumferential fibers, whose directions are taken into account by including a local coordinate system
* Distinct domains (eardrum, malleus, incus, stapes, etc) with their respective material properties
* Complex-valued global matrices to account for the domains’ internal damping and the impedance of the cochlear fluid

The OpenGL was used set, so it could display animations of the selected vibration patterns, while the user rotates the model with the mouse. These animations are comprised with the deformed mesh of the model, “thermal” colors displaying the displacement of the mesh and phong reflection on the model’s surface. 
Lastly, openMP was used to speed up the computation of the element’s matrices and post-processing of the displacement results.
