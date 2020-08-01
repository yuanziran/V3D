#pragma once
#include<vtkSmartPointer.h>
#define VTK_USE(type,name) vtkSmartPointer<type> name=vtkSmartPointer<type>::New();
