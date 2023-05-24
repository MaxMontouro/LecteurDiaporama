#include "lecteurvue.h"
#include "ui_lecteurvue.h"

#include <QMessageBox>
#include <QTimer>
#include <cstring>
#include <QDebug>

LecteurVue::LecteurVue(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LecteurVue)
{
    ui->setupUi(this);
    _numDiaporamaCourant = 0;   // =  le lecteur est vide
    _posImageCourante = 0; // Diaporama vide

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
    connect(ui->bSuivant,SIGNAL(clicked()),this,SLOT(demandeAvancerFleche()));
    connect(ui->bPrecedent,SIGNAL(clicked()),this,SLOT(demandeReculerFleche()));
    connect(ui->actionEnlever_diaporama,SIGNAL(triggered()),this,SLOT(demandeVider()));
    connect(ui->bLancerDiapo,SIGNAL(clicked()),this,SLOT(demandeLancementDiapo()));
    connect(ui->bArreterDiapo,SIGNAL(clicked()),this,SLOT(demandeArretDiapo()));

    // Timer
    this->m_timer = new QTimer(this);

    //QString sMode = "Mode : Manuel";
    //ui->statusbar->showMessage(sMode);
    this->m_lMode = new QLabel(ui->statusbar);
    this->m_lMode->setIndent(5);
    this->m_lMode->setAlignment(Qt::AlignTop);
    this->mode(false);
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

    /*// Affichage de l'image
    QString chemImage = this->getCheminRscImage(this->imageCourante()->getChemin());
    ui->lImage->setPixmap(QPixmap(chemImage));
    QString tAAfficher = "Diaporama n°";
    QString t;
    t.setNum(this->numDiaporamaCourant());
    tAAfficher+=t;
    ui->lTitreDiapo->setText(tAAfficher);
    ui->lTitreImage->setText(QString::fromStdString(this->imageCourante()->getTitre()));
    ui->lCategorieImage->setText(QString::fromStdString(this->imageCourante()->getCategorie()));
    QString rAAfficher = "Image n°";
        QString r;
    r.setNum(this->imageCourante()->getRang());
    rAAfficher+=r;
    rAAfficher+=QString::fromStdString("/");
    r.setNum(this->nbImages());
    rAAfficher+=r;
    ui->lRangImage->setText(rAAfficher);*/
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

    /*// Affichage de l'image
    QString chemImage = this->getCheminRscImage(this->imageCourante()->getChemin());
    ui->lImage->setPixmap(QPixmap(chemImage));
    QString tAAfficher = "Diaporama n°";
    QString t;
    t.setNum(this->numDiaporamaCourant());
    tAAfficher+=t;
    ui->lTitreDiapo->setText(tAAfficher);
    ui->lTitreImage->setText(QString::fromStdString(this->imageCourante()->getTitre()));
    ui->lCategorieImage->setText(QString::fromStdString(this->imageCourante()->getCategorie()));
    QString rAAfficher = "Image n°";
    QString r;
    r.setNum(this->imageCourante()->getRang());
    rAAfficher+=r;
    rAAfficher+=QString::fromStdString("/");
    r.setNum(this->nbImages());
    rAAfficher+=r;
    ui->lRangImage->setText(rAAfficher);*/
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
    if (numDiaporamaCourant() > 0)
    {
        chargerDiaporama(); // charge le diaporama courant
    }

}

void LecteurVue::chargerDiaporama()
{
    qDebug() << "Chargement du diaporama";
    /* Chargement des images associées au diaporama courant
       Dans une version ultérieure, ces données proviendront d'une base de données,
       et correspondront au diaporama choisi */
    Image* imageACharger;
    imageACharger = new Image(3, "personne", "Blanche Neige", "C:\\cartesDisney\\Disney_2.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(2, "personne", "Cendrillon", "C:\\cartesDisney\\Disney_4.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(4, "animal", "Mickey", "C:\\cartesDisney\\Disney_1.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(1, "personne", "Grincheux", "C:\\cartesDisney\\Disney_10.gif");
    _diaporama.push_back(imageACharger);


    // trier le contenu du diaporama par ordre croissant selon le rang de l'image dans le diaporama
    // À FAIRE
    // Tri bulle
    bool trie;
    for (unsigned int cpt1 = (*this).nbImages()-1;cpt1>=1;cpt1--)
    {
        trie = true;
        for (unsigned int cpt2=0;cpt2<=cpt1-1;cpt2++)
        {
            if ((*this)._diaporama[cpt2]->getRang() > (*this)._diaporama[cpt2+1]->getRang())
            {
                trie = false;
                // Échange des deux éléments
                Image *tmp = (*this)._diaporama[cpt2];
                (*this)._diaporama[cpt2] = (*this)._diaporama[cpt2+1];
                (*this)._diaporama[cpt2+1] = tmp;
            }
        }
        if (trie)
        {
            break;
        }
    }

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

    /*// Récupération du nom de la première image
    QString image = this->getCheminRscImage(this->imageCourante()->getChemin());
    qDebug() << image;
    // Affichage de l'image
    ui->lImage->setPixmap(QPixmap(image));
    QString tAAfficher = "Diaporama n°";
    QString t;
    t.setNum(this->numDiaporamaCourant());
    tAAfficher+=t;
    ui->lTitreDiapo->setText(tAAfficher);
    ui->lTitreImage->setText(QString::fromStdString(this->imageCourante()->getTitre()));
    ui->lCategorieImage->setText(QString::fromStdString(this->imageCourante()->getCategorie()));
    QString rAAfficher = "Image n°";
    QString r;
    r.setNum(this->imageCourante()->getRang());
    rAAfficher+=r;
    rAAfficher+=QString::fromStdString("/");
    r.setNum(this->nbImages());
    rAAfficher+=r;
    ui->lRangImage->setText(rAAfficher);*/
    this->majImage();
}

void LecteurVue::viderDiaporama()
{
    qDebug() << "Diapo clear";
    if (nbImages () > 0)
    {
        unsigned int taille = nbImages();
        for (unsigned int i = 0; i < taille ; i++)
        {
            _diaporama.pop_back(); /* Removes the last element in the vector,
                                      effectively reducing the container size by one.
                                      AND deletes the removed element */
        }
        _posImageCourante = 0;
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
    return static_cast<unsigned int>(_diaporama.size());
}

Image *LecteurVue::imageCourante()
{
    return (*this)._diaporama[(*this)._posImageCourante-1];
}

unsigned int LecteurVue::numDiaporamaCourant()
{
    return (*this)._numDiaporamaCourant;
}

void LecteurVue::affichageInfosApp()
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setStandardButtons(QMessageBox::Ok);
    msgBox->setText("Version de l’application : Version 2\nDate de création : 03/05/2023\nAuteurs : Maxime MONTOURO et Clément MOURGUE");
    msgBox->setWindowTitle("A propos de ...");
    msgBox->exec();
}

void LecteurVue::mode(bool autoMode)
{
    m_timer->stop();
    if (autoMode)
    {
        this->viderDiaporama();
        this->changerDiaporama(this->numDiaporamaCourant());
        QString sMode = "Mode : Automatique";
        qDebug() << sMode;
        m_lMode->setText(sMode);

        ui->bArreterDiapo->setEnabled(true);


        connect(m_timer, SIGNAL(timeout()), this,SLOT(demandeAvancer()));
        m_timer->start(1000*this->m_vitesseDefilement);
    }
    else
    {
        //m_timer->stop();
        QString sMode = "Mode : Manuel";
        qDebug() << sMode;
        m_lMode->setText(sMode);

        ui->bArreterDiapo->setEnabled(false);
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
        ui->lTitreDiapo->setText(tr("TitreDiapo"));
        ui->lTitreImage->setText(tr("TitreImage"));
        ui->lCategorieImage->setText(tr("CatImage"));
        ui->lRangImage->setText(tr("RangImage"));
        this->mode(false);
    }
}

void LecteurVue::demandeLancementDiapo()
{
    this->mode(true);
}

void LecteurVue::demandeArretDiapo()
{
    this->mode(false);
}

QString LecteurVue::getCheminRscImage(string cheminAct)
{
    // Récupération du nom de la première image
    string s = cheminAct;
    string delimiter = "\\";

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
    this->mode(false);
    this->demandeAvancer();
}

void LecteurVue::demandeReculerFleche()
{
    this->mode(false);
    this->demandeReculer();
}

void LecteurVue::majImage()
{
    // Affichage de l'image
    QString chemImage = this->getCheminRscImage(this->imageCourante()->getChemin());
    ui->lImage->setPixmap(QPixmap(chemImage));
    QString tAAfficher = "Diaporama n°";
    QString t;
    t.setNum(this->numDiaporamaCourant());
    tAAfficher+=t;
    ui->lTitreDiapo->setText(tAAfficher);
    ui->lTitreImage->setText(QString::fromStdString(this->imageCourante()->getTitre()));
    ui->lCategorieImage->setText(QString::fromStdString(this->imageCourante()->getCategorie()));
    QString rAAfficher = "Image n°";
    QString r;
    r.setNum(this->imageCourante()->getRang());
    rAAfficher+=r;
    rAAfficher+=QString::fromStdString("/");
    r.setNum(this->nbImages());
    rAAfficher+=r;
    ui->lRangImage->setText(rAAfficher);
}
