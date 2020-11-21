///
/// application for visualizing grphics
/// 
/// 
#include "windows.h"
#include <QtWidgets/QApplication>



int main(int argc, char *argv[]) {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout << 8;

    QApplication app(argc, argv);
    MyWindow *mywindow = new MyWindow;
    
    
    return app.exec();
}
