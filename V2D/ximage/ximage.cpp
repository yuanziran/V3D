#include"qximage.hpp"
#include<QApplication>
using namespace V3D;

int main(int argc,char**argv)
{
    QApplication app(argc,argv);
    QXimage ximage;
    ximage.show();
    return app.exec();
}