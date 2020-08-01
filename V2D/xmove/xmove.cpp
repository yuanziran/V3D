#include"qxmove.hpp"
#include<QApplication>
using namespace V3D;
int main(int argc,char**argv)
{
    QApplication app(argc,argv);
    QXMove xmove;
    xmove.show();
    return app.exec();
}