#ifndef ejemplo1_H
#define ejemplo1_H

#include <QtGui>
#include "ui_counterDlg.h"

class ejemplo1 : public QWidget, public Ui_Counter
{
    Q_OBJECT
    public:
        ejemplo1();

    public slots:
        void doButton();
        void fTimer();
        void reanudar();
        void resetear();
        void cambioPeriodo(int);
        
    private:
    	QTimer *cronometro;
    	int count;
    	int periodo;
};

#endif // ejemplo1_H
