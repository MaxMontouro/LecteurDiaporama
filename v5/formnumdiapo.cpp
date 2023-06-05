#include "formnumdiapo.h"
#include "ui_formnumdiapo.h"

#include "qsqlquery.h"

#include <QTableWidgetItem>

formNumDiapo::formNumDiapo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formNumDiapo)
{
    ui->setupUi(this);

    this->majTable();

    connect(ui->bValidation,SIGNAL(accepted()),this,SLOT(demandeAccepter()));

    connect(ui->tDiapo,SIGNAL(cellClicked(int,int)),this,SLOT(majNumDiapo(int,int)));

    setWindowTitle(tr("Formulaire de choix de diaporama à charger"));
}

formNumDiapo::~formNumDiapo()
{
    delete ui;
}

bool formNumDiapo::openDataBase()
{
    m_db = QSqlDatabase::addDatabase(CONNECT_TYPE);
    m_db.setDatabaseName(DATABASE_FORM_NAME);
    return m_db.open();
}

void formNumDiapo::closeDataBase()
{
    this->m_db.close();
}

void formNumDiapo::majTable()
{
    if(!this->m_db.isOpen())
    {
        this->openDataBase();
    }

    // Num Diapo et titre
    QSqlQuery requete;
    requete.prepare("SELECT idDiaporama, `titre Diaporama` FROM Diaporamas ORDER BY idDiaporama ASC;");
    if (!requete.exec())
    {
        qDebug() << "Erreur lors de l'exécution de la requête !";
    }
    else
    {
        for(int i = 0; requete.next(); i++)
        {
            ui->tDiapo->insertRow(ui->tDiapo->rowCount());
            for (int col = 0;col < ui->tDiapo->columnCount()-1;col++)
            {
                ui->tDiapo->setItem(ui->tDiapo->rowCount()-1,
                                    col,
                                    new QTableWidgetItem(requete.value(col).toString()));
            }
        }
    }

    // Nb image
    requete.prepare("SELECT idDiaporama, COUNT(idDiapo) FROM DiaposDansDiaporama GROUP BY idDiaporama ORDER BY idDiaporama ASC;");
    if (!requete.exec())
    {
        qDebug() << "Erreur lors de l'exécution de la requête !";
    }
    else
    {
        for(int i = 0; requete.next(); i++)
        {
                ui->tDiapo->setItem(i,
                                    ui->tDiapo->columnCount()-1,
                                    new QTableWidgetItem(requete.value(1).toString()));
         }
    }

    this->closeDataBase();
}

unsigned int formNumDiapo::getNumDiapo()
{
    return this->m_numDiapo;
}

bool formNumDiapo::getAccepte()
{
    return this->m_accepte;
}

void formNumDiapo::demandeAccepter()
{
    this->m_accepte=true;
}

void formNumDiapo::majNumDiapo(int ligne, int colonne)
{
    colonne = 0;
    this->m_numDiapo = ui->tDiapo->item(ligne,colonne)->text().toUInt();
}
