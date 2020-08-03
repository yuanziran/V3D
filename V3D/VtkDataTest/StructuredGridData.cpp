#include"v3d.hpp"
#include<vtkPoints.h>
#include<vtkStructuredGrid.h>
#include<vtkStructuredGridWriter.h>
#include"getopt.h"
#include<iostream>
#include<string>
#include<cstring>
#include<cstdlib>

int main(int argc,char**argv)
{
    const char* help="StructuredGridData \n"
	"-n,--name:file name.\n"
	"-x,--xdim:vtk data xdim.\n"
	"-y,--ydim:vtk data ydim.\n"
	"-z,--zdim:vtk data zdim.\n"
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
			case 'n':
				fileName=optarg;
				break;
			default:
				std::cerr<<help<<std::endl;
				exit(EXIT_FAILURE);
		}
	}

	VTK_USE(vtkPoints,points)
	for(int i=0;i<xdim;++i)
		for(int j=0;j<ydim;++j)
			for (int k = 0; k < zdim; ++k)
			{
				points->InsertNextPoint(i,j,k);
			}
	VTK_USE(vtkStructuredGrid, grid)
	grid->SetDimensions(xdim,ydim,zdim);
	grid->SetPoints(points);
	VTK_USE(vtkStructuredGridWriter, writer)
	writer->SetFileName(fileName.c_str());
	writer->SetInputData(grid);
	writer->Write();
	return 0;
}