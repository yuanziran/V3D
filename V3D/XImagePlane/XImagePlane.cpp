#include"v3d.hpp"
#include "vtkActor.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkImageReader.h"
#include "vtkImageActor.h"
#include "vtkImageReslice.h"
#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkImageMapToColors.h"
#include "vtkImagePlaneWidget.h"
#include "vtkImageReader.h"
#include "vtkInteractorEventRecorder.h"
#include "vtkLookupTable.h"
#include "vtkOutlineFilter.h"
#include "vtkDICOMImageReader.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkScalarBarActor.h"
#include "vtkScalarBarWidget.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkPlaneSource.h"
#include "vtkPlane.h"
#include "vtkImageShiftScale.h"
#include "vtkResliceCursorActor.h"
#include "vtkResliceCursorPolyDataAlgorithm.h"
#include "vtkResliceCursor.h"
#include "vtkResliceCursorWidget.h"
#include "vtkResliceCursorLineRepresentation.h"
#include "vtkBiDimensionalWidget.h"
#include"vtkAutoInit.h"
#include"vtkAxesActor.h"
#include"vtkTransform.h"
#include"vtkTextActor.h"
#include"vtkProperty2D.h"
#include"vtkImageReader.h"
#include"colormap.hpp"
#include<getopt.h>
#include<string>
#include<cstring>
#include<cstdlib>
#include<iostream>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);

int main(int argc, char**argv)
{
	const char* help = "RawDataReader \n"
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
	const char* short_opt = "-n:x:y:z:c::h";
	const option long_opt[] = {
		{"name",1,NULL,'n'},{"xdim",1,NULL,'x'},
		{"ydim",1,NULL,'y'},{"zdim",1,NULL,'z'},
		{"colormap",2,NULL,'c'},
		{"help",0,NULL,'h'},{0,0,0,0}
	};
	optind = 1;
	int result = -1;
	int xdim = 0;
	int ydim = 0;
	int zdim = 0;
	int colormap=0;
	std::string fileName;
	while ((result = getopt_long(argc, argv, short_opt, long_opt, &optind)) != -1)
	{
		switch (result)
		{
		case 'x':
			sscanf(optarg, "%d", &xdim);
			break;
		case 'y':
			sscanf(optarg, "%d", &ydim);
			break;
		case 'z':
			sscanf(optarg, "%d", &zdim);
			break;
		case 'n':
			fileName = optarg;
			break;
		case 'c':
			sscanf(optarg,"%d",&colormap);
			break;
		default:
			std::cerr << help << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	std::cout << "xdim : " << xdim << std::endl
		<< "ydim : " << ydim << std::endl
		<< "zdim : " << zdim << std::endl
		<< "file name : " << fileName << std::endl;
	//read raw data
	VTK_USE(vtkImageReader, reader)
	reader->SetFileName(fileName.c_str());
	reader->SetFileDimensionality(3);
	reader->SetNumberOfScalarComponents(1);
	reader->SetHeaderSize(0);
	reader->SetDataScalarType(VTK_FLOAT);
	reader->SetDataExtent(0, xdim - 1, 0, ydim - 1, 0, zdim - 1);//�˴�Ҫ�������
	reader->SetDataSpacing(1, 1, 1);
	reader->Update();
	//��ȡ�����С
	vtkDataArray* array = reader->GetOutput()->GetPointData()->GetScalars();
	double range[2];
	array->GetRange(range);
	std::cout << "data ranage [0] " << range[0] << std::endl
		<< "data ranage [1] " << range[1] << std::endl;
	//����ת��
#if 0
	VTK_USE(vtkImageShiftScale, shift)
	shift->SetOutputScalarTypeToUnsignedChar();
	shift->SetInputData(reader->GetOutput());
	shift->SetScale(255 / (range[1] - range[0]));
	shift->SetShift(-range[0]);
	shift->Update();
#endif

	VTK_USE(vtkLookupTable, colorTable)
	V3D::vtkSetColorTable(colorTable,colormap);
	colorTable->SetRange(range[0],range[1]);
	VTK_USE(vtkScalarBarActor, scalarBar)
	scalarBar->SetOrientationToHorizontal();
	scalarBar->SetLookupTable(colorTable);

#if 0
	VTK_USE(vtkImageMapToColors, colorMap)
	colorMap->SetInputConnection(shift->GetOutputPort());
	colorMap->SetLookupTable(colorTable);
	colorMap->Update();
#endif
	//����û����
	

	VTK_USE(vtkRenderer, renderer)
	//renderer->AddActor(actor);
	renderer->SetBackground(0,0,0);
	renderer->ResetCamera();

	VTK_USE(vtkRenderWindow, window)
	window->AddRenderer(renderer);
	window->SetSize(800,800);
	VTK_USE(vtkRenderWindowInteractor, interactor)
	interactor->SetRenderWindow(window);
	VTK_USE(vtkProperty,prop)

	VTK_USE(vtkImagePlaneWidget, planeWidgetX)
	VTK_USE(vtkImagePlaneWidget, planeWidgetY)
	VTK_USE(vtkImagePlaneWidget, planeWidgetZ)
	planeWidgetX->SetInteractor(interactor);
	planeWidgetX->GetPlaneProperty()->SetColor(1,0,0);//���ñ�����ɫ
	planeWidgetX->SetTexturePlaneProperty(prop);//<<������������
	planeWidgetX->TextureInterpolateOff();
	planeWidgetX->SetResliceInterpolateToLinear();
	planeWidgetX->SetInputConnection(reader->GetOutputPort());
	planeWidgetX->SetPlaneOrientation(0);
	planeWidgetX->SetSliceIndex(xdim/2);
	planeWidgetX->DisplayTextOn();
	planeWidgetX->SetDefaultRenderer(renderer);
	planeWidgetX->SetWindowLevel(1358, -27);
	planeWidgetX->On();
	planeWidgetX->InteractionOn();

	planeWidgetY->SetInteractor(interactor);
	planeWidgetY->GetPlaneProperty()->SetColor(0, 1, 0);//���ñ�����ɫ
	planeWidgetY->SetTexturePlaneProperty(prop);//<<������������
	planeWidgetY->TextureInterpolateOff();
	planeWidgetY->SetResliceInterpolateToLinear();
	planeWidgetY->SetInputConnection(reader->GetOutputPort());
	planeWidgetY->SetPlaneOrientation(1);
	planeWidgetY->SetSliceIndex(ydim / 2);
	planeWidgetY->DisplayTextOn();
	planeWidgetY->SetDefaultRenderer(renderer);
	planeWidgetY->SetWindowLevel(1358, -27);
	planeWidgetY->On();
	planeWidgetY->InteractionOn();

	planeWidgetZ->SetInteractor(interactor);
	planeWidgetZ->GetPlaneProperty()->SetColor(0, 0, 1);//���ñ�����ɫ
	planeWidgetZ->SetTexturePlaneProperty(prop);//<<������������
	planeWidgetZ->TextureInterpolateOff();
	planeWidgetZ->SetResliceInterpolateToLinear();
	planeWidgetZ->SetInputConnection(reader->GetOutputPort());
	planeWidgetZ->SetPlaneOrientation(2);
	planeWidgetZ->SetSliceIndex(zdim / 2);
	planeWidgetZ->DisplayTextOn();
	planeWidgetZ->SetDefaultRenderer(renderer);
	planeWidgetZ->SetWindowLevel(1358, -27);
	planeWidgetZ->On();
	planeWidgetZ->InteractionOn();

	planeWidgetX->SetLookupTable(colorTable);
	planeWidgetY->SetLookupTable(colorTable);
	planeWidgetZ->SetLookupTable(colorTable);

	VTK_USE(vtkInteractorStyleImage, style)
	VTK_USE(vtkInteractorStyleTrackballCamera,cameraStyle)
	VTK_USE(vtkScalarBarWidget, barWidget)
	barWidget->SetInteractor(interactor);
	barWidget->SetScalarBarActor(scalarBar);
	barWidget->On();
	interactor->SetInteractorStyle(style);
	interactor->SetInteractorStyle(cameraStyle);
	interactor->Initialize();
	interactor->Start();
	return 0;
}