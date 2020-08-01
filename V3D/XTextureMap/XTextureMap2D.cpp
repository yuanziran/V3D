#include<vtkSmartPointer.h>
#include<vtkPointData.h>
#include<vtkDataArray.h>
#include<vtkImageData.h>
#include<vtkImageReader.h>
#include<vtkGPUVolumeRayCastMapper.h>
#include<vtkInteractorStyleTrackballCamera.h>
#include<vtkVolumeProperty.h>
#include<vtkColorTransferFunction.h>
#include<vtkPiecewiseFunction.h>
#include<vtkRenderer.h>
#include<vtkRenderWindow.h>
#include<vtkRenderWindowInteractor.h>
#include<vtkImageShiftScale.h>
#include<getopt.h>
#include<cstring>
#include<cstdlib>
#include<iostream>
#include"v3d.hpp"

int main(int argc,char**argv)
{
		const char* help="RawDataReader \n"
	"-n,--name:file name.\n"
	"-x,--xdim:raw data xdim.\n"
	"-y,--ydim:raw data ydim.\n"
	"-z,--zdim:raw data zdim.\n"
	"-h,--help:help user!";
	const char* short_opt="-n:x:y:z:h";
	const option long_opt[]={
		{"name",1,NULL,'n'},{"xdim",1,NULL,'x'},
		{"ydim",1,NULL,'y'},{"zdim",1,NULL,'z'},
		{"help",0,NULL,'h'},{0,0,0,0}
	};
	optind=1;
	int result=-1;
	int xdim=0;
	int ydim=0;
	int zdim=0;
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
			case 'n':
				fileName=optarg;
				break;
			default:
				std::cerr<<help<<std::endl;
				exit(EXIT_FAILURE);
		}
	}


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

    //get the real range in hounsfield
	vtkDataArray* array=reader->GetOutput()->GetPointData()->GetScalars();
	double range[2];
	array->GetRange(range);
	//先平移后缩放
	VTK_USE(vtkImageShiftScale,shift)
	shift->SetOutputScalarTypeToUnsignedChar();
	shift->SetInputData(reader->GetOutput());
	shift->SetScale(255/(range[1]-range[0]));
	shift->SetShift(-range[0]);
	shift->Update();

	//vtkNew<vtkVolumeProperty> volumeProperty;
	VTK_USE(vtkVolumeProperty,volumeProperty)
	volumeProperty->ShadeOn();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);
    volumeProperty->SetAmbient(0.1);//阴影
    volumeProperty->SetDiffuse(0.6);//反射
    volumeProperty->SetSpecular(0.3);//高光
	
    VTK_USE(vtkPiecewiseFunction,scalarTF) 
	scalarTF->AddPoint(0,0.0);
	scalarTF->AddPoint(127,0.5);
	scalarTF->AddPoint(255,0.9);
    volumeProperty->SetScalarOpacity(scalarTF);

    VTK_USE(vtkColorTransferFunction,colorTF)
    colorTF->AddRGBPoint(0,0.0,0.0,0.0);
    colorTF->AddRGBPoint(64,1.0,0.52,0.30);
    colorTF->AddRGBPoint(190,1.00,1.00,1.00);
    colorTF->AddRGBPoint(220,0.2,0.2,0.2);
    volumeProperty->SetColor(colorTF);

    VTK_USE(vtkVolumeTexture,mapper)
	
    mapper->SetInputConnection(shift->GetOutputPort());

    VTK_USE(vtkVolume,volume)
	volume->SetMapper(mapper);
	volume->SetProperty(volumeProperty);
	renderer->AddVolume(volume);

 
	VTK_USE(vtkRenderer,renderer)
	renderWindow->AddRenderer(renderer);
	renderer->SetBackground(0,0,0);
    renderer->AddVolume(volume);
	renderer->ResetCamera();
	renderer->GetActiveCamera()->Zoom(1.3);
    VTK_USE(vtkRenderWindowInteractor,interactor)
	interactor->SetRenderWindow(renderWindow);
	VTK_USE(vtkInteractorStyleTrackballCamera,style)

	interactor->SetInteractorStyle(style);
	renderWindow->Render();
	interactor->Start();
	return 0;
}
