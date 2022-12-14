#include "ejemplo1.h"

ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	
	count=0;
    periodo=1;
	cronometro = new QTimer(this);
	//Conexion del cronometro para que se vaya incrementando su valor
	connect(cronometro, SIGNAL(timeout()),this,SLOT(fTimer()));
	//Conexion del boton STOP
	connect(button, SIGNAL(clicked()), this, SLOT(doButton()) );
	//Conexion del boton Reanudar
	connect(start, SIGNAL(clicked()), this, SLOT(reanudar()) );
	//Conexion del boton Resetear
	connect(reset, SIGNAL(clicked()), this, SLOT(resetear()) );
	//Conexion de la barra horizontal
	connect(cambiarPeriodo, SIGNAL(valueChanged(int)), this, SLOT(cambioPeriodo(int)) );
	
}

void ejemplo1::doButton()
{
	qDebug() << "click on button STOP";

	cronometro->stop();
}

//Esta funcion se ejecuta cada vez que tiene que cambiar de valor el cronometro
void ejemplo1::fTimer()
{
	lcdNumber->display(count);
	count++;
}

void ejemplo1::reanudar()
{
	qDebug() << "click on button Reanudar";
	cronometro->start(periodo);
}

void ejemplo1::resetear()
{
	qDebug() << "click on button Resetear";
	count=0;
	lcdNumber->display(count);
	cronometro->stop();
}

void ejemplo1::cambioPeriodo(int valor)
{
	periodo=valor;
}


