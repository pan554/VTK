catch {load vtktcl}
if { [catch {set VTK_TCL $env(VTK_TCL)}] != 0} { set VTK_TCL "../../examplesTcl" }
if { [catch {set VTK_DATA $env(VTK_DATA)}] != 0} { set VTK_DATA "../../../vtkdata" }

# this is a tcl version to decimtae fran's face
# get the interactor ui
source $VTK_TCL/vtkInt.tcl

# Create the RenderWindow, Renderer and both Actors
#
vtkRenderer ren1
vtkRenderWindow renWin
    renWin AddRenderer ren1
vtkRenderWindowInteractor iren
    iren SetRenderWindow renWin

# create a cyberware source
#
vtkPolyDataReader cyber
    cyber SetFileName "$VTK_DATA/fran_cut.vtk"
#7347 triangles remain
vtkDecimate deci; 
    deci SetInput [cyber GetOutput]
    deci SetTargetReduction 0.9
    deci SetAspectRatio 20
    deci SetInitialError 0.0002
    deci SetErrorIncrement 0.0005
    deci SetMaximumIterations 6
    deci SetInitialFeatureAngle 45
vtkPolyDataNormals normals
    normals SetInput [deci GetOutput]
vtkPolyDataMapper cyberMapper
    cyberMapper SetInput [normals GetOutput]
vtkActor cyberActor
    cyberActor SetMapper cyberMapper
    eval [cyberActor GetProperty] SetColor 1.0 0.49 0.25
    eval [cyberActor GetProperty] SetRepresentationToWireframe

# Add the actors to the renderer, set the background and size
#
ren1 AddActor cyberActor
ren1 SetBackground 1 1 1
renWin SetSize 500 500

# render the image
#
iren SetUserMethod {wm deiconify .vtkInteract}

vtkCamera cam1
    cam1 SetClippingRange 0.0475572 2.37786
    cam1 SetFocalPoint 0.052665 -0.129454 -0.0573973
    cam1 SetPosition 0.327637 -0.116299 -0.256418
    cam1 SetViewUp -0.0225386 0.999137 0.034901
ren1 SetActiveCamera cam1

iren Initialize
#renWin SetFileName "deciFran.tcl.ppm"
#renWin SaveImageAsPPM

# prevent the tk window from showing up then start the event loop
wm withdraw .


