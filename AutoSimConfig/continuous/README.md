#  AutoSimConfig Test FMU with capability to continue integration

> [!WARNING]  
> **CAREFUL: RESEARCH RESULT**  
> Please be warned that the content in this folder is the result of a research project. **It is no productive code!** The code serves as a prototype to support the research project(s) involved and communicate results. You can use the code under the license terms but without warranty, mostly for educational purposes.

This FMU was initially developed in the [Modapto project](https://modapto.eu/) as a result of the research carried out there.
In order to provide a test case for evaluating, the model was adjusted to allow for continued integration.

As part of the work in the project, the sustainability of production plants and modules are to be evaluated. This optimization can be done both on basis of the raw energy consumption and the emitted carbon dioxide during production of the same amount of energy.

This FMU calculates the energy and the carbon footprint based on the used (electrical) power of a productin plant and the momentary power mix.
For a more in depth documentation, see the FMU-internal documentation.

This repository/folder contains the sources to build the FMU from scratch.
The attached Makefile serves as an example on how to build a FMU with cross-compiling for both windows and linux usage. It assumes the GNU GCC is installed in appropriate versions.

Please note that this is just an example code. Use it at your own risk.
