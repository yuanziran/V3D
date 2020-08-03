#include"v3d.hpp"
#include<vtkPolyDataMapper.h>
#include<vtkStructuredGridReader.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);
int main(int argc,char**argv)
{
    VTK_USE(vtkStructuredGridReader,reader)
    reader->SetFileName(argv[1]);
    reader->Update();

    VTK_USE(vtkStructuredGridGeometryFilter,geometryFilter)
    geometryFilter->SetInputConnection(reader->GetOutputPort());

    VTK_USE(vtkPolyDataMapper,mapper)
    mapper->SetInputConnection(geometryFilter->GetOutputPort());

    VTK_USE(vtkActor,actor)
    actor->SetMapper(mapper);

    VTK_USE(vtkRenderer,renderer)
    renderer->AddActor(actor);
    renderer->SetBackground(0,0,0);
    
    VTK_USE(vtkRenderWindow,window)
    window->AddRenderer(renderer);
    window->SetSize(800,800);

    VTK_USE(vtkRenderWindowInteractor,interactor)
    VTK_USE(vtkInteractorStyleTrackballCamera,style)
	interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(window);
    interactor->Initialize();
    interactor->Start();
    return 0;
}