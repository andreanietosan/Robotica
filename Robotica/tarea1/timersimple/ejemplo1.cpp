#include "ejemplo1.h"


ejemplo1::ejemplo1(): Ui_Counter()
{
	setupUi(this);
	show();
	periodo=1;

    connect(button, SIGNAL(clicked()), this, SLOT(doButton()));
    connect(cambiarPeriodo, SIGNAL(valueChanged(int)), this, SLOT(cambioPeriodo(int)) );

	mytimer.connect(std::bind(&ejemplo1::cuenta, this));
    mytimer.start(periodo);
}

ejemplo1::~ejemplo1()
{}

void ejemplo1::doButton()
{
	static bool stopped = false;
	stopped = !stopped;
	if(stopped)
		mytimer.stop();
	else
		mytimer.start(periodo);
	qDebug() << "click on button";
}

void ejemplo1::cuenta()
{
    lcdNumber->display(++cont);
	trick++;
}

void ejemplo1::cambioPeriodo(int valor)
{
    periodo=valor;
}

