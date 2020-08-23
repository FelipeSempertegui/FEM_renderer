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

2. All global & sparse matrices (mass, damping & stiffness) and the load vector are computed and assembled in parallel via openMP. Right after this procedure, the fixed boundary conditions are applied to the generated matrices.

3. The frequencies of the excitation pressures are typed by the user, which -  in combination with constrained matrices – are used to solve the linear system and provide complex-valued displacement fields.

4. The facets, nodes & generated displacements of the model’s external surface are extracted and converted from the complex domain to the time domain. The goal of this step is to prepare the data to be rendered with OpenGL. The following data are generated:
   * Deformed mesh of the model as a function of time
   * Colors at the vertices as functions of the nodes’ absolute displacements. The chosen color distribution is similar to the “thermal” colormap of matplotlib
   * Vertex normals for every time-step, as it is a necessary input for the phong lightning set in our shader (“Project1/vertexFragmentShaders.shader”)
   
5. The data is visualized using the OpenGL API. The material is set as a combination of the data generated during the previous step (deformed mesh, colors and normals) and the vertex and fragment shaders.


# Instructions for Visual Studio 2019:

This program requires the addition of the following external libraries, most of which are present in this repository in the folder “Project1/Dependencies”:


* Matrix manipulations and sparse linear system solvers: Eigen, Intel® MKL (called from the Eigen libraries) & openMP
* Real-time rendering (dynamic draw calls & transformation matrices): GLFW, GLM & OpenGL

Among the aforementioned dependencies, the only one, which must be acquired by the user is the Intel® MKL. After acquiring it via the Intel’s website (link shown below) and configuring it, a new tab is available in Visual Studio (Properties/Configuration Properties/Intel Performance Libraries).

Link: https://software.intel.com/content/www/us/en/develop/articles/intel-math-kernel-library-intel-mkl-2020-install-guide.html?wapkw=MKL

With the dependencies available in the repository and the Intel MKL enabled from Visual Studio, the following changes should take place under “Properties/ Configuration Properties”:

## Intel Performance Libraries/Intel Math Kernel Library:
* Use Intel MKL: select “Sequential”
* Use ILP64 interfaces: select “No”
* Use MPI Library: select “Intel(R) MPI”


