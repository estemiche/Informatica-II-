#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "bitwise_op.h"

using namespace std;

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString archivoEntrada = "I_D_R1.bmp";
    QString archivoMascara = "M.bmp";
    QString archivoSalida  = "I_D_R2.bmp";

    int width1 = 0, height1 = 0;
    int width2 = 0, height2 = 0;

    // Imprimir tamaños de ambas imágenes
    QImage imgMascarada(archivoEntrada);
    QImage imgMascara(archivoMascara);
    cout << "Tamaño I_D_R1.bmp: " << imgMascarada.width() << "x" << imgMascarada.height() << endl;
    cout << "Tamaño M.bmp:      " << imgMascara.width() << "x" << imgMascara.height() << endl;

    // Cargar las imágenes
    unsigned char* imagen1 = loadPixels(archivoEntrada, width1, height1);
    unsigned char* imagen2 = loadPixels(archivoMascara, width2, height2);

    if (!imagen1 || !imagen2) {
        cerr << "Error al cargar las imágenes." << endl;
        delete[] imagen1;
        delete[] imagen2;
        return -1;
    }

    // Aplicar XOR con patrón de máscara repetido
    unsigned char* resultado = new unsigned char[width1 * height1 * 3];
    for (int y = 0; y < height1; ++y) {
        for (int x = 0; x < width1; ++x) {
            int pos = (y * width1 + x) * 3;
            int maskY = y % height2;
            int maskX = x % width2;
            int maskPos = (maskY * width2 + maskX) * 3;

            resultado[pos]     = applyXOR(&imagen1[pos],     imagen2[maskPos]);
            resultado[pos + 1] = applyXOR(&imagen1[pos + 1], imagen2[maskPos + 1]);
            resultado[pos + 2] = applyXOR(&imagen1[pos + 2], imagen2[maskPos + 2]);
        }
    }

    // Exportar imagen resultante
    bool exportado = exportImage(resultado, width1, height1, archivoSalida);
    cout << "Exportacion: " << (exportado ? "Exitosa" : "Fallida") << endl;

    delete[] imagen1;
    delete[] imagen2;
    delete[] resultado;

    return 0;
}

unsigned char* loadPixels(QString input, int &width, int &height){
    QImage imagen(input);
    if (imagen.isNull()) {
        cout << "Error: No se pudo cargar la imagen BMP." << endl;
        return nullptr;
    }
    imagen = imagen.convertToFormat(QImage::Format_RGB888);
    width  = imagen.width();
    height = imagen.height();

    int dataSize = width * height * 3;
    unsigned char* pixelData = new unsigned char[dataSize];
    for (int y = 0; y < height; ++y) {
        const uchar* srcLine = imagen.scanLine(y);
        unsigned char* dstLine = pixelData + y * width * 3;
        memcpy(dstLine, srcLine, width * 3);
    }
    return pixelData;
}

bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida){
    QImage outputImage(width, height, QImage::Format_RGB888);
    for (int y = 0; y < height; ++y) {
        memcpy(outputImage.scanLine(y), pixelData + y * width * 3, width * 3);
    }
    if (!outputImage.save(archivoSalida, "BMP")) {
        cout << "Error: No se pudo guardar la imagen BMP modificada.";
        return false;
    } else {
        cout << "Imagen BMP modificada guardada como "
             << archivoSalida.toStdString() << endl;
        return true;
    }
}
