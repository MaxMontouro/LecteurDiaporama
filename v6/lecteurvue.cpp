#include "lecteurvue.h"
#include "ui_lecteurvue.h"

#include <QMessageBox>
#include <QTimer>
#include <cstring>
#include <QDebug>

#include "qsqlquery.h"

typedef vector<unsigned int> resQuery;   // Structure de données permettant de conserver les résultats d'une requête même après la fermeturede la base de données sans utiliser deux fois en même temps une base de données

LecteurVue::LecteurVue(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LecteurVue)
{
    ui->setupUi(this);
    _numDiaporamaCourant = 0;   // =  le lecteur est vide
    _posImageCourante = 0; // Diaporama vide

    this->m_mydb = new BaseDeDonnees();

    setWindowTitle(tr("Lecteur de diaporama"));

    // Font
    QFont fTitreDiapo(tr("Helevtica"),15,QFont::Bold);
    QFont fTitreImage(tr("Arial"),11,QFont::DemiBold);
    QFont fInfoSupImage(tr("Arial Narrow"),10,QFont::Cursive);

    // Image de base
    ui->lImage->setPixmap(QPixmap(":/v2/cartesDisney/Disney_tapis.gif"));

    // Application des fonts
    ui->lTitreDiapo->setFont(fTitreDiapo);
    ui->lTitreImage->setFont(fTitreImage);
    ui->lRangImage->setFont(fInfoSupImage);
    ui->lCategorieImage->setFont(fInfoSupImage);

    connect(ui->actionA_propose_de,SIGNAL(triggered()),this,SLOT(affichageInfosApp()));

    //connect(ui->actionCharger_diaporama,SIGNAL(triggered()),this,SLOT(chargerDiaporama()));
    connect(ui->actionCharger_diaporama,SIGNAL(triggered()),this,SLOT(demandeChangementDiapo()));

    connect(ui->bSuivant,SIGNAL(clicked()),this,SLOT(demandeAvancerFleche()));
    connect(ui->bPrecedent,SIGNAL(clicked()),this,SLOT(demandeReculerFleche()));
    connect(ui->actionEnlever_diaporama,SIGNAL(triggered()),this,SLOT(demandeVider()));
    connect(ui->bLancerDiapo,SIGNAL(clicked()),this,SLOT(demandeLancementDiapo()));
    connect(ui->bArreterDiapo,SIGNAL(clicked()),this,SLOT(demandeArretDiapo()));

    connect(ui->actionVitesse_de_d_filement,SIGNAL(triggered()),this,SLOT(demandeModifVitesse()));

    // Timer
    this->m_timer = new QTimer(this);

    connect(m_timer, SIGNAL(timeout()), this,SLOT(demandeAvancer()));


    //QString sMode = "Mode : Manuel";
    //ui->statusbar->showMessage(sMode);
    this->m_lMode = new QLabel(ui->statusbar);
    this->m_lMode->setIndent(5);
    this->m_lMode->setAlignment(Qt::AlignTop);
    this->mode(false);

    // Récupération des différents diapos
    if (!this->m_mydb->openDataBase())
    {
        QMessageBox::critical(this,"Erreur !","La base de données est inexistante ou inaccessible !");
    }
    else
    {
        QSqlQuery req;
        req.prepare("SELECT idDiaporama FROM Diaporamas;");
        if (!req.exec())
        {
            QMessageBox::critical(this,"Erreur !","Erreur lors de l'exécution de la requête !");
        }
        else
        {
            // Récupération et conservation des résultats de la requête
            unsigned int cpt = 0;
            resQuery resQ;
            for (int i = 0; req.next(); i++)
            {
                resQ.push_back(req.value(0).toUInt());
                cpt++;
            }

            for (unsigned int i = 0; i < cpt; i++)
            {
                Diaporama *diapo = new Diaporama(this->m_mydb);
                diapo->setDiaporama(resQ[i]);
                this->m_donnees.push_back(diapo);
                /*qDebug() << "Diapo n°" << resQ[i];
                qDebug() << diapo->getTitreDiapo();
                qDebug() << diapo->getNbImages();*/
            }
        }
    }
}

LecteurVue::~LecteurVue()
{
    delete ui;
}

void LecteurVue::avancer()
{
    qDebug() << "Avancement de l'image";
    (*this)._posImageCourante = ((*this)._posImageCourante+1)%((*this).nbImages()+1);
    if ((*this)._posImageCourante == 0)
    {
        (*this)._posImageCourante++;
    }
    this->majImage();
}

void LecteurVue::reculer()
{
    qDebug() << "Reculement de l'image";
    (*this)._posImageCourante = ((*this)._posImageCourante-1)%((*this).nbImages()+1);
    if ((*this)._posImageCourante == 0)
    {
        (*this)._posImageCourante=(*this).nbImages();
    }

    this->majImage();
}

void LecteurVue::changerDiaporama(unsigned int pNumDiaporama)
{
    qDebug() << "Changement de diaporama";
    // s'il y a un diaporama courant, le vider, puis charger le nouveau Diaporama
    if (numDiaporamaCourant() > 0)
    {
        viderDiaporama();
    }
    _numDiaporamaCourant = pNumDiaporama;
    this->m_indiceDiapo = pNumDiaporama -1;// car indice dans la liste de pointeur de diaporama m_donnees
    if (numDiaporamaCourant() > 0)
    {
        chargerDiaporama(); // charge le diaporama courant
    }

}

void LecteurVue::chargerDiaporama()
{
    qDebug() << "Chargement du diaporama";

    if ((*this).nbImages() != 0)
    {
        _posImageCourante = 1;
    }
    else
    {
        _posImageCourante = 0;
    }

    cout << "Diaporama num. " << numDiaporamaCourant() << " selectionne. " << endl;
    cout << nbImages() << " images chargees dans le diaporama" << endl;
    this->majImage();
}

void LecteurVue::viderDiaporama()
{
    qDebug() << "Diapo clear";
    if (nbImages () > 0)
    {
        _posImageCourante = 0;
        _numDiaporamaCourant = 0;
    }
    cout << nbImages() << " images restantes dans le diaporama." << endl;

}

void LecteurVue::afficher()
{
    /* affiche les information sur le lecteur :
     * 1) vide (si num. de diaporama = 0) OU BIEN  numéro de diaporama affiché
     * 2) Si un diaporama courant est chargé (num. de diaporama > 0), affiche l'image courante OU BIEN 'diaporama vide'
     *     si ce diaporama n'a aucun image */

    // 1)
    if ((*this)._numDiaporamaCourant == 0)
    {
        ui->lImage->setText("Lecteur vide\n");
    }
    else
    {
        QString t1 = tr("Diaporama num. ");
        QString t2 = tr(to_string(this->numDiaporamaCourant()).c_str());
        QString t3 = tr("\n");
        QString txt = t1 + t2 + t3;
        ui->lImage->setText(txt);
    }

    // 2)
    if ((*this)._numDiaporamaCourant > 0)
    {
        if ((*this).nbImages() != 0) // Diaporama non vide
        {
            (*(*this).imageCourante()).afficher();
        }
        else
        {
            ui->lImage->setText(ui->lImage->text()+"\nDiaporama vide\n");
        }
    }
}

unsigned int LecteurVue::nbImages()
{
    return this->m_donnees[this->m_indiceDiapo]->getNbImages();
}

Image *LecteurVue::imageCourante()
{
    return this->m_donnees[this->m_indiceDiapo]->getImage((*this)._posImageCourante-1);
}

unsigned int LecteurVue::numDiaporamaCourant()
{
    return (*this)._numDiaporamaCourant;
}

void LecteurVue::affichageInfosApp()
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setText("Version de l’application : Version 6\nDate de création : 05/06/2023\nAuteurs : Maxime MONTOURO et Clément MOURGUE");
    msgBox->setWindowTitle("A propos de ...");
    msgBox->exec();
}

void LecteurVue::mode(bool autoMode)
{
    m_timer->stop();
    if (autoMode)
    {
        unsigned int numDiapoOld = this->numDiaporamaCourant();
        this->viderDiaporama();
        this->changerDiaporama(numDiapoOld);
        QString sMode = "Mode : Automatique";
        qDebug() << sMode;
        m_lMode->setText(sMode);

        ui->bArreterDiapo->setEnabled(true);

        ui->actionVitesse_de_d_filement->setEnabled(false);

        m_timer->start(1000*this->m_vitesseDefilement);
    }
    else
    {
        //m_timer->stop();
        QString sMode = "Mode : Manuel";
        qDebug() << sMode;
        m_lMode->setText(sMode);

        ui->bArreterDiapo->setEnabled(false);

        ui->actionVitesse_de_d_filement->setEnabled(true);
    }
}

void LecteurVue::demandeAvancer()
{
    if (numDiaporamaCourant() <= 0)
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Diaporama vide, impossible d'avancer !"),QMessageBox::Ok);
    }
    else
    {
        this->avancer();
    }
}

void LecteurVue::demandeReculer()
{
    if (numDiaporamaCourant() <= 0)
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Diaporama vide, impossible de reculer !"),QMessageBox::Ok);
    }
    else
    {
        this->reculer();
    }
}

void LecteurVue::demandeVider()
{
    if (numDiaporamaCourant() <= 0)
    {
        QMessageBox::critical(this,tr("Erreur"),tr("Diaporama déjà vidé !"),QMessageBox::Ok);
    }
    else
    {
        this->viderDiaporama();
        // Réinitialisation de l'affichage
        ui->lImage->setPixmap(QPixmap("qrc:/v2/cartesDisney/Disney_tapis.gif"));
        ui->lTitreDiapo->setText(tr("titreDiapo")); //(tr("TitreDiapo"));
        ui->lTitreImage->setText(tr("titre")); //(tr("TitreImage"));
        ui->lCategorieImage->setText(tr("Catégorie")); //(tr("CatImage"));
        ui->lRangImage->setText(tr("Rang")); //(tr("RangImage"));
        this->mode(false);

        // Image de base
        ui->lImage->setPixmap(QPixmap(":/v2/cartesDisney/Disney_tapis.gif"));
    }
}

void LecteurVue::demandeLancementDiapo()
{
    if (this->numDiaporamaCourant()>0)
    {
        this->mode(true);
    }
    else
    {
        QMessageBox::critical(this,"Erreur","Aucun diaporama disponible pour un lancement");
    }
}

void LecteurVue::demandeArretDiapo()
{
    this->mode(false);
}

QString LecteurVue::getCheminRscImage(QString cheminAct)
{
    // Récupération du nom de la première image
    string s = cheminAct.toStdString();
    // string delimiter = "\\"; // Jusqu'à V4
    string delimiter = "/"; // Depuis V5

    unsigned int pos = 0;
    string nomImage;
    string tmp;
    while (true)
    {
        pos = s.find(delimiter);
        tmp = s.substr(0, pos);
        if (tmp=="")
        {
            break;
        }
        nomImage = tmp;
        s.erase(0, pos + delimiter.length());
    }
    // Création du chemin réel de l'image dans le pc
    QString prefix = ":/v2/cartesDisney/";
    QString cheminImage = prefix + QString::fromStdString(nomImage);
    return cheminImage;
}

void LecteurVue::demandeAvancerFleche()
{
    //this->mode(false);
    this->demandeArretDiapo();
    this->demandeAvancer();
}

void LecteurVue::demandeReculerFleche()
{
    //->mode(false);
    this->demandeArretDiapo();
    this->demandeReculer();
}

void LecteurVue::majImage()
{
    // Affichage de l'image
    //QString chemImage = this->getCheminRscImage(this->imageCourante()->getChemin());
    QString chemImg = ":/v2";
    chemImg+=this->imageCourante()->getChemin();
    ui->lImage->setPixmap(QPixmap(chemImg)); //chemImage));
    ui->lTitreDiapo->setText(this->m_donnees[this->m_indiceDiapo]->getTitreDiapo());
    ui->lTitreImage->setText(this->imageCourante()->getTitre());
    ui->lCategorieImage->setText(this->imageCourante()->getCategorie());
    QString rAAfficher = "Image n°";
    QString r;
    r.setNum(this->imageCourante()->getRang());
    rAAfficher+=r;
    rAAfficher+=QString::fromStdString("/");
    r.setNum(this->nbImages());
    rAAfficher+=r;
    ui->lRangImage->setText(rAAfficher);
}

void LecteurVue::demandeModifVitesse()
{
    if(this->numDiaporamaCourant()==0)
    {
        QMessageBox::critical(this,"Erreur","Vous n'avez pas de diaporama chargé !");
    }
    else
    {
        this->m_fVitesse = new formVitesseDefilement(this);
        this->m_fVitesse->exec();
        if (this->m_fVitesse->getAccepte())
        {
            this->m_vitesseDefilement = this->m_fVitesse->getVitesseDefilement();
        }
     }
}

void LecteurVue::demandeChangementDiapo()
{
    this->m_fNumDiapo = new formNumDiapo(this,this->m_mydb);
    this->m_fNumDiapo->exec();
    if (this->m_fNumDiapo->getAccepte())
    {
        this->changerDiaporama(this->m_fNumDiapo->getNumDiapo());
    }
}
