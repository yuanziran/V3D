#include"v3d.hpp"
#include<vtkUnstructuredGridReader.h>
#include<vtkDataSetMapper.h>
#include<vtkUnstructuredGrid.h>
#include<vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkScalarBarWidget.h>
#include<vtkScalarBarActor.h>
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

int main(int argc,char**argv)
{
    VTK_USE(vtkUnstructuredGridReader,reader)
    reader->SetFileName(argv[1]);
    reader->Update();

    VTK_USE(vtkDataSetMapper,mapper)
    mapper->SetInputData(reader->GetOutput());
    mapper->SetScalarRange(reader->GetOutput()->GetScalarRange());

    VTK_USE(vtkActor,actor)
    actor->SetMapper(mapper);

    VTK_USE(vtkScalarBarActor,scalarBar)
    scalarBar->SetLookupTable(mapper->GetLookupTable());
    scalarBar->SetOrientationToHorizontal();
    VTK_USE(vtkRenderer,renderer)
    renderer->AddActor(actor);
    //renderer->AddActor(scalarBar);
    renderer->SetBackground(0,0,0);
    
    VTK_USE(vtkRenderWindow,window)
    window->AddRenderer(renderer);
    window->SetSize(800,800);

    VTK_USE(vtkRenderWindowInteractor,interactor)
    VTK_USE(vtkInteractorStyleTrackballCamera,style)
	interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(window);
	VTK_USE(vtkScalarBarWidget, barWidget)
	barWidget->SetInteractor(interactor);
	barWidget->SetScalarBarActor(scalarBar);
	barWidget->On();
    interactor->Initialize();
    interactor->Start();
    return 0;
}