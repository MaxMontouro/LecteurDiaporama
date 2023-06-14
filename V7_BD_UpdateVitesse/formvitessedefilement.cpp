#include "formvitessedefilement.h"
#include "ui_formvitessedefilement.h"

formVitesseDefilement::formVitesseDefilement(QWidget *parent,unsigned int vitesseDeBase) :
    QDialog(parent),
    ui(new Ui::formVitesseDefilement)
{
    ui->setupUi(this);
    connect(ui->bBValidation,SIGNAL(accepted()),this,SLOT(demandeAccepter()));

    setWindowTitle(tr("Formulaire de modification de vitesse"));
    ui->sBVitesse->setValue(vitesseDeBase);
}

formVitesseDefilement::~formVitesseDefilement()
{
    delete ui;
}

unsigned int formVitesseDefilement::getVitesseDefilement()
{
    return ui->sBVitesse->value();
}

bool formVitesseDefilement::getAccepte()
{
    return this->m_accepte;
}

void formVitesseDefilement::demandeAccepter()
{
    this->m_accepte=true;
}
