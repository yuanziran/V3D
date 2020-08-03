#include<vtkImageData.h>
#include<vtkRenderWindow.h>
#include<vtkRenderer.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkVolumeProperty.h>
#include<vtkVolume.h>
#include<vtkPiecewiseFunction.h>
#include<vtkColorTransferFunction.h>
#include<vtkGPUVolumeRayCastMapper.h>
#include<vtkPointData.h>
#include<vtkCamera.h>
#include<vtkFloatArray.h>
#include<vtkScalarBarActor.h>
#include<vtkScalarBarWidget.h>
#include<vtkImageReader.h>
#include<vtkImageShiftScale.h>
#include<vtkNew.h>
#include<fstream>
#include<string>
#include<vtkSmartPointer.h>
#include"getopt.h"
#include<cstring>
#include<cstdlib>
#include<iostream>
#include<vtkAutoInit.h>
#include"colormap.hpp"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#define VTK_USE(type,name) vtkSmartPointer<type> name=vtkSmartPointer<type>::New();
int main(int argc,char**argv)
{
	const char* help="XVolume \n"
	"-n,--name:file name.\n"
	"-x,--xdim:raw data xdim.\n"
	"-y,--ydim:raw data ydim.\n"
	"-z,--zdim:raw data zdim.\n"
	"-c,--colormap: 0=Gray\n"
	"				1=Jet\n"
	"				2=Cool\n"
	"				3=Hot\n"
	"				4=Parula\n"
	"				5=Hsv\n"
	"				6=Spring\n"
	"				7=Summer\n"
	"				8=Autumn\n"
	"				9=Winter\n"
	"				10=Bone\n"
	"				11=Copper\n"
	"				12=Pink\n"
	"-h,--help:help user!";
	const char* short_opt="-n:x:y:z:c::h";
	const option long_opt[]={
		{"name",1,NULL,'n'},{"xdim",1,NULL,'x'},
		{"ydim",1,NULL,'y'},{"zdim",1,NULL,'z'},
		{"colormap",2,NULL,'c'},
		{"help",0,NULL,'h'},{0,0,0,0}
	};
	optind=1;
	int result=-1;
	int xdim=0;
	int ydim=0;
	int zdim=0;
	int colormap=0;
	std::string fileName;
	while((result=getopt_long(argc,argv,short_opt,long_opt,&optind))!=-1)
	{
		switch(result)
		{
			case 'x':
				sscanf(optarg,"%d",&xdim);
				break;
			case 'y':
				sscanf(optarg,"%d",&ydim);
				break;
			case 'z':
				sscanf(optarg,"%d",&zdim);
				break;
			case 'c':
				sscanf(optarg,"%d",&colormap);
				break;
			case 'n':
				fileName=optarg;
				break;
			default:
				std::cerr<<help<<std::endl;
				exit(EXIT_FAILURE);
		}
	}
	colormap%=COLORMAP_NUM;
	std::cout << "xdim : " << xdim << std::endl
		<< "ydim : " << ydim << std::endl
		<< "zdim : " << zdim << std::endl
		<< "file name : " << fileName << std::endl;
	//read raw data
	VTK_USE(vtkImageReader,reader)
	reader->SetFileName(fileName.c_str());
	reader->SetFileDimensionality(3);
	reader->SetNumberOfScalarComponents(1);
	reader->SetHeaderSize(0);
	reader->SetDataScalarType(VTK_FLOAT);
	reader->SetDataExtent(0,xdim-1,0,ydim-1,0,zdim-1);
	reader->SetDataSpacing(1,1,1);
	reader->Update();

	VTK_USE(vtkVolumeProperty,volumeProperty)
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkDataArray* array=reader->GetOutput()->GetPointData()->GetScalars();
	double range[2];
	array->GetRange(range);

	VTK_USE(vtkImageShiftScale,shift)
	shift->SetOutputScalarTypeToUnsignedChar();
	shift->SetInputData(reader->GetOutput());
	shift->SetScale(255/(range[1]-range[0]));
	shift->SetShift(-range[0]);
	shift->Update();


	//1D transfer functions
	//vtkNew<vtkColorTransferFunction> colorTF;
	VTK_USE(vtkColorTransferFunction,colorTF)
	V3D::vtkSetColorTransferFunction(colorTF,colormap);

	
	
	//vtkNew<vtkPiecewiseFunction> scalarTF;
	VTK_USE(vtkPiecewiseFunction,scalarTF) 
	scalarTF->AddPoint(0,0.0);
	scalarTF->AddPoint(255,0.9);

	//vtkNew<vtkPiecewiseFunction> gradientTF;
	VTK_USE(vtkPiecewiseFunction,gradientTF)
	gradientTF->AddPoint(0,0.0);
	gradientTF->AddPoint(255,1.0);

	volumeProperty->SetScalarOpacity(scalarTF);
	volumeProperty->SetGradientOpacity(gradientTF);
	volumeProperty->SetColor(colorTF);

	VTK_USE(vtkLookupTable, colorTable)
	V3D::vtkSetColorTable(colorTable,colormap);
	colorTable->SetRange(range[0],range[1]);
	VTK_USE(vtkScalarBarActor, scalarBar)
	scalarBar->SetOrientationToHorizontal();
	scalarBar->SetLookupTable(colorTable);

	
	//set up rendering context
	//vtkNew<vtkRenderWindow> renderWindow;
	VTK_USE(vtkRenderWindow,renderWindow)
	renderWindow->SetSize(800,800);
	renderWindow->SetMultiSamples(0);

	//mapping data
	VTK_USE(vtkGPUVolumeRayCastMapper,mapper)
	mapper->SetInputConnection(shift->GetOutputPort());
	mapper->SetBlendModeToComposite();
	mapper->SetUseJittering(1);

	//vtkNew<vtkRenderer> renderer;
	VTK_USE(vtkRenderer,renderer)
	renderWindow->AddRenderer(renderer);
	renderer->SetBackground(0,0,0);

	//vtkNew<vtkVolume> volume;
	VTK_USE(vtkVolume,volume)
	volume->SetMapper(mapper);
	volume->SetProperty(volumeProperty);
	renderer->AddVolume(volume);

	renderer->ResetCamera();
	renderer->GetActiveCamera()->Zoom(1.3);


	VTK_USE(vtkRenderWindowInteractor,interactor)
	interactor->SetRenderWindow(renderWindow);
	VTK_USE(vtkInteractorStyleTrackballCamera,style)
	interactor->SetInteractorStyle(style);
	VTK_USE(vtkScalarBarWidget, barWidget)
	barWidget->SetInteractor(interactor);
	barWidget->SetScalarBarActor(scalarBar);
	barWidget->On();
	renderWindow->Render();

	interactor->Start();
	return 0;

}
