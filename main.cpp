#include <fstream>
#include <iostream>
#include <QCoreApplication>
#include <QImage>
#include "bitwise_op.h"

using namespace std;

unsigned char* loadPixels(QString input, int &width, int &height);
bool exportImage(unsigned char* pixelData, int width, int height, QString archivoSalida);
unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels);

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString archivoEntrada = "I_D_R2.bmp";
    QString archivoSalida  = "I_D_R3.bmp";
    QString archivoM2 = "M2.txt";

    int width = 0, height = 0;

    // Cargar imagen desenmascarada parcialmente (I_D_R2)
    unsigned char* pixelData = loadPixels(archivoEntrada, width, height);
    if (!pixelData) {
        cerr << "Error al cargar la imagen '" << archivoEntrada.toStdString() << "'" << endl;
        return -1;
    }

    // Cargar el archivo de enmascaramiento M2.txt
    int seed = 0;
    int n_pixels = 0;
    unsigned int* maskingData = loadSeedMasking(archivoM2.toStdString().c_str(), seed, n_pixels);

    if (maskingData != nullptr) {
        for (int i = 0; i < n_pixels; ++i) {
            int index = (seed + i) % (width * height);  // Índice del píxel
            int pos = index * 3;
            pixelData[pos]     = applyXOR(&pixelData[pos], maskingData[i * 3]);       // R
            pixelData[pos + 1] = applyXOR(&pixelData[pos + 1], maskingData[i * 3 + 1]); // G
            pixelData[pos + 2] = applyXOR(&pixelData[pos + 2], maskingData[i * 3 + 2]); // B
        }

        delete[] maskingData;
    } else {
        cerr << "No se pudo cargar el archivo de enmascaramiento M2.txt" << endl;
    }

    // Exportar la imagen resultante
    bool exportado = exportImage(pixelData, width, height, archivoSalida);
    cout << "Exportacion: " << (exportado ? "Exitosa" : "Fallida") << endl;

    delete[] pixelData;
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
        cout << "Imagen BMP modificada guardada como " << archivoSalida.toStdString() << endl;
        return true;
    }
}

unsigned int* loadSeedMasking(const char* nombreArchivo, int &seed, int &n_pixels){
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return nullptr;
    }

    archivo >> seed;
    int r, g, b;
    while (archivo >> r >> g >> b) {
        n_pixels++;
    }
    archivo.close();

    archivo.open(nombreArchivo);
    if (!archivo.is_open()) {
        cout << "Error al reabrir el archivo." << endl;
        return nullptr;
    }

    unsigned int* RGB = new unsigned int[n_pixels * 3];
    archivo >> seed;
    for (int i = 0; i < n_pixels * 3; i += 3) {
        archivo >> r >> g >> b;
        RGB[i]     = r;
        RGB[i + 1] = g;
        RGB[i + 2] = b;
    }

    archivo.close();
    cout << "Semilla: " << seed << endl;
    cout << "Cantidad de pixeles leidos: " << n_pixels << endl;
    return RGB;
}
