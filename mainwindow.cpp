#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv/cv.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <QDebug>
#include <QImage>
#include <QKeyEvent>

using namespace cv;
using namespace std;

static Mat imageOriginal, imagemTBINinv, resultDilat, resultErode, resultOpen, resultClose, imageTransf, se;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Projeto OpenCV 2015 : Operações morfológicas sobre imagens binárias");

    //imagem default
    int type_se;
    imageOriginal = imread( "../imagens/image3.jpg", CV_LOAD_IMAGE_GRAYSCALE );


    threshold(imageOriginal, imagemTBINinv, 90, 255, THRESH_BINARY);
    ui->lbBinText->setText(ui->lbBinText->text().append("Binarização Simples"));

    if (ui->quadrado->isChecked()) type_se = MORPH_RECT;
    if (ui->rect->isChecked()) type_se = MORPH_RECT;
    if (ui->cruz->isChecked()) type_se = MORPH_CROSS;
    if (ui->circulo->isChecked()) type_se = MORPH_ELLIPSE;
    if (ui->elipse->isChecked()) type_se = MORPH_ELLIPSE;

    int x = ui->slider->value();

    if (ui->rect->isChecked()){
        se = getStructuringElement(type_se, Size(3*x,x));
    } else if (ui->elipse->isChecked()){
        se = getStructuringElement(type_se, Size(4*x + 1, 2*x+1), Point( x, x ));
    }else{
        se = getStructuringElement(type_se, Size(x,x));
    }

    if(ui->dillat->isChecked())
        dilate(imagemTBINinv, imageTransf, se);
    if(ui->erosao->isChecked())
        erode(imagemTBINinv, imageTransf, se);
    if(ui->fecho->isChecked()){
        dilate(resultDilat, imageTransf, se);
        erode(imagemTBINinv, resultDilat, se);
    }
    if(ui->abertura->isChecked()){
        erode(resultErode, imageTransf, se);
        dilate(imagemTBINinv, resultErode, se);
    }
    if(ui->top->isChecked()){
        erode(imagemTBINinv, resultErode, se);
        dilate(resultErode, resultOpen, se);
        imageTransf = imagemTBINinv - resultOpen;
    }
    if(ui->black->isChecked()){
        dilate(imagemTBINinv, resultDilat, se);
        erode(resultDilat, resultClose, se);
        imageTransf = resultClose - imagemTBINinv;
    }


    IplImage imagTransf = imageTransf; // convert Mat to OplImage
    IplImage imagInvConv = imagemTBINinv;
    IplImage imagOrg = imageOriginal;

    if(&imagTransf != NULL && &imagInvConv !=NULL && &imagOrg !=NULL){
        QImage qimageTrans = IplImage2QImage(&imagTransf);
        QImage qimageInv = IplImage2QImage(&imagInvConv);
        QImage qimageOrig = IplImage2QImage(&imagOrg);

        ui->lbOrigem->setScaledContents(1);
        ui->lbBin->setScaledContents(1);
        ui->lbDestino->setScaledContents(1);

        ui->lbBin->setPixmap(QPixmap::fromImage(qimageInv));
        ui->lbOrigem->setPixmap(QPixmap::fromImage(qimageOrig));
        ui->lbDestino->setPixmap(QPixmap::fromImage(qimageTrans));
    }
}


void MainWindow::on_pushButton_clicked(){

    int type_se;
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Images (*.png *.xpm *.bmp *.JPG *.jpg *.tif)"));
    dialog.setViewMode(QFileDialog::Detail);
    fileName = QFileDialog::getOpenFileName(this, tr("Open Images"), "/home", tr("Image Files (*.png *.JPG *.xpm *.jpg *.bmp *.tif)"));

    if (!fileName.isEmpty()){

        imageOriginal= imread( fileName.toStdString(), CV_LOAD_IMAGE_GRAYSCALE );

        typeBin = binarizationType();

        threshold(imageOriginal, imagemTBINinv, 90, 255, typeBin);

        if (ui->quadrado->isChecked()) type_se = MORPH_RECT;
        if (ui->rect->isChecked()) type_se = MORPH_RECT;
        if (ui->cruz->isChecked()) type_se = MORPH_CROSS;
        if (ui->circulo->isChecked()) type_se = MORPH_ELLIPSE;
        if (ui->elipse->isChecked()) type_se = MORPH_ELLIPSE;

        int x = ui->slider->value();

        if (ui->rect->isChecked()){
            se = getStructuringElement(type_se, Size(3*x,x));
        } else if (ui->elipse->isChecked()){
            se = getStructuringElement(type_se, Size(4*x + 1, 2*x+1), Point( x, x ));
        }else{
            se = getStructuringElement(type_se, Size(x,x));
        }

        if(ui->dillat->isChecked())
            dilate(imagemTBINinv, imageTransf, se);
        if(ui->erosao->isChecked())
            erode(imagemTBINinv, imageTransf, se);
        if(ui->fecho->isChecked()){
            dilate(resultDilat, imageTransf, se);
            erode(imagemTBINinv, resultDilat, se);
        }
        if(ui->abertura->isChecked()){
            erode(resultErode, imageTransf, se);
            dilate(imagemTBINinv, resultErode, se);
        }
        if(ui->top->isChecked()){
            erode(imagemTBINinv, resultErode, se);
            dilate(resultErode, resultOpen, se);
            imageTransf = imagemTBINinv - resultOpen;
        }
        if(ui->black->isChecked()){
            dilate(imagemTBINinv, resultDilat, se);
            erode(resultDilat, resultClose, se);
            imageTransf = resultClose - imagemTBINinv;
        }

        IplImage imagTransf = imageTransf; // convert Mat to OplImage
        IplImage imagInvConv = imagemTBINinv;
        IplImage imagOrg = imageOriginal;

        if(&imagTransf != NULL && &imagInvConv !=NULL && &imagOrg !=NULL){
            QImage qimageTrans = IplImage2QImage(&imagTransf);
            QImage qimageInv = IplImage2QImage(&imagInvConv);
            QImage qimageOrig = IplImage2QImage(&imagOrg);

            ui->lbOrigem->setScaledContents(1);
            ui->lbBin->setScaledContents(1);
            ui->lbDestino->setScaledContents(1);

            ui->lbBin->setPixmap(QPixmap::fromImage(qimageInv));
            ui->lbOrigem->setPixmap(QPixmap::fromImage(qimageOrig));
            ui->lbDestino->setPixmap(QPixmap::fromImage(qimageTrans));
        }
    }

}


void MainWindow::keyPressEvent(QKeyEvent * event){
    int type_se;

    if (event->key() == Qt::Key_F5){

        if (ui->quadrado->isChecked()) type_se = MORPH_RECT;
        if(ui->rect->isChecked()) type_se = MORPH_RECT;
        if(ui->cruz->isChecked()) type_se = MORPH_CROSS;
        if (ui->circulo->isChecked()) type_se = MORPH_ELLIPSE;
        if (ui->elipse->isChecked()) type_se = MORPH_ELLIPSE;

        int x = ui->slider->value();

        if (ui->rect->isChecked()){
            se = getStructuringElement(type_se, Size(3*x,x));
        } else if (ui->elipse->isChecked()){
            se = getStructuringElement(type_se, Size(4*x + 1, 2*x+1), Point( x, x ));
        }else{
            se = getStructuringElement(type_se, Size(x,x));
        }

        if(ui->dillat->isChecked())
            dilate(imagemTBINinv, imageTransf, se);

        if(ui->erosao->isChecked())
            erode(imagemTBINinv, imageTransf, se);

        if(ui->fecho->isChecked()){
            dilate(imagemTBINinv, resultDilat, se);
            erode(resultDilat, imageTransf, se);
        }

        if(ui->abertura->isChecked()){
            erode(imagemTBINinv, resultErode, se);
            dilate(resultErode, imageTransf, se);
        }

        if(ui->gradient->isChecked()){
            dilate(imagemTBINinv, resultDilat, se);
            erode(imagemTBINinv, resultErode, se);
            imageTransf = resultDilat - resultErode;
        }

        if(ui->top->isChecked()){
            erode(imagemTBINinv, resultErode, se);
            dilate(resultErode, resultOpen, se);
            imageTransf = imagemTBINinv - resultOpen;
        }

        if(ui->black->isChecked()){
            dilate(imagemTBINinv, resultDilat, se);
            erode(resultDilat, resultClose, se);
            imageTransf = resultClose - imagemTBINinv;
        }

        IplImage imagTransf = imageTransf; // convert Mat to OplImage
        IplImage imagInvConv = imagemTBINinv;
        IplImage imagOrg = imageOriginal;

        if(&imagTransf != NULL && &imagInvConv !=NULL && &imagOrg !=NULL){
            QImage qimageTrans = IplImage2QImage(&imagTransf);
            QImage qimageInv = IplImage2QImage(&imagInvConv);
            QImage qimageOrig = IplImage2QImage(&imagOrg);

            ui->lbOrigem->setScaledContents(1);
            ui->lbBin->setScaledContents(1);
            ui->lbDestino->setScaledContents(1);

            ui->lbBin->setPixmap(QPixmap::fromImage(qimageInv));
            ui->lbOrigem->setPixmap(QPixmap::fromImage(qimageOrig));
            ui->lbDestino->setPixmap(QPixmap::fromImage(qimageTrans));
        }
    }
}



void MainWindow::on_pushButton_3_clicked(){

    VideoCapture cap(0); // open the default camera

    typeBin = binarizationType();
    int type_se;
    Mat frame,  frameInv, greyMat;

    while(1){

        cap >> frame;

        cv::cvtColor(frame, greyMat, CV_BGR2GRAY);

        threshold(greyMat, frameInv, 90, 255, typeBin);

        if (ui->quadrado->isChecked()) type_se = MORPH_RECT;
        if (ui->rect->isChecked()) type_se = MORPH_RECT;
        if (ui->cruz->isChecked()) type_se = MORPH_CROSS;
        if (ui->circulo->isChecked()) type_se = MORPH_ELLIPSE;
        if (ui->elipse->isChecked()) type_se = MORPH_ELLIPSE;

        int x = ui->slider->value();

        if (ui->rect->isChecked()){
            se = getStructuringElement(type_se, Size(3*x,x));
        }else if (ui->elipse->isChecked()){
            se = getStructuringElement(type_se, Size(4*x + 1, 2*x+1), Point( x, x ));
        }else{
            se = getStructuringElement(type_se, Size(x,x));
        }

        if(ui->dillat->isChecked())
            dilate(frameInv, imageTransf, se);

        if(ui->erosao->isChecked())
            erode(frameInv, imageTransf, se);

        if(ui->fecho->isChecked()){
            dilate(frameInv, resultDilat, se);
            erode(resultDilat, imageTransf, se);
        }

        if(ui->abertura->isChecked()){
            erode(frameInv, resultErode, se);
            dilate(resultErode, imageTransf, se);
        }

        if(ui->gradient->isChecked()){
            dilate(frameInv, resultDilat, se);
            erode(frameInv, resultErode, se);
            imageTransf = resultDilat - resultErode;
        }

        if(ui->top->isChecked()){
            erode(frameInv, resultErode, se);
            dilate(resultErode, resultOpen, se);
            imageTransf = frameInv - resultOpen;
        }

        if(ui->black->isChecked()){
            dilate(frameInv, resultDilat, se);
            erode(resultDilat, resultClose, se);
            imageTransf = resultClose - frameInv;
        }

        IplImage frameIpl = greyMat;
        IplImage frameInvIpl = frameInv;
        IplImage frameResIpl = imageTransf;

        QImage frameOrig = IplImage2QImage(&frameIpl);
        QImage frameDest = IplImage2QImage(&frameInvIpl);
        QImage frameRest = IplImage2QImage(&frameResIpl);

        ui->lbOrigem->setPixmap(QPixmap::fromImage(frameOrig));
        ui->lbBin->setPixmap(QPixmap::fromImage(frameDest));
        ui->lbDestino->setPixmap(QPixmap::fromImage(frameRest));

        //if (keyevent->key()==Qt::Key_W) break;

        if(waitKey(30) ==27) break;
    }

}

// funcoes auxiliares
int MainWindow::binarizationType(){

    int type; QMessageBox msgBox;
    ui->lbBinText->clear();

    msgBox.setText(trUtf8("Escolha o tipo de binarização a aplicar"));
    QAbstractButton *myYesButton = msgBox.addButton(trUtf8("Binarização Simples"), QMessageBox::YesRole);
    QAbstractButton *myNoButton = msgBox.addButton(trUtf8("Binarização Invertida"), QMessageBox::NoRole);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();

    if(msgBox.clickedButton() == myNoButton){
        type = THRESH_BINARY_INV;
        ui->lbBinText->setText(ui->lbBinText->text().append("Binarização Invertida"));
    }
    if(msgBox.clickedButton() == myYesButton){
        type = THRESH_BINARY;
        ui->lbBinText->setText(ui->lbBinText->text().append("Binarização Simples"));
    }
    return type;
}


// adaptado http://stackoverflow.com/questions/9761392/gui-with-qt-creator-to-play-video
QImage MainWindow::IplImage2QImage(const IplImage *iplImage)
{
    int height = iplImage->height;
    int width = iplImage->width;

    if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 3)
    {
      const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
      QImage img(qImageBuffer, width, height, QImage::Format_RGB888);
      return img.rgbSwapped();
    } else if  (iplImage->depth == IPL_DEPTH_8U && iplImage->nChannels == 1){
    const uchar *qImageBuffer = (const uchar*)iplImage->imageData;
    QImage img(qImageBuffer, width, height, QImage::Format_Indexed8);

    QVector<QRgb> colorTable;
    for (int i = 0; i < 256; i++){
        colorTable.push_back(qRgb(i, i, i));
    }
    img.setColorTable(colorTable);
    return img;
    }else{
      qWarning() << "Image cannot be converted.";
      return QImage();
    }
}

MainWindow::~MainWindow(){
    delete ui;
}
