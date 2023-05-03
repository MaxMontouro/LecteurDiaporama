#include "lecteurvue.h"
#include "ui_lecteurvue.h"

#include <QMessageBox>

LecteurVue::LecteurVue(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LecteurVue)
{
    ui->setupUi(this);
    _numDiaporamaCourant = 0;   // =  le lecteur est vide
    _posImageCourante = 0; // Diaporama vide

    connect(ui->actionA_propose_de,SIGNAL(triggered()),this,SLOT(affichageInfosApp()));

    QString sMode = " Manuel";
    QString msgMode = "Mode :";
    ui->statusbar->showMessage(msgMode + sMode);
}

LecteurVue::~LecteurVue()
{
    delete ui;
}

void LecteurVue::avancer()
{
    (*this)._posImageCourante = ((*this)._posImageCourante+1)%(*this).nbImages();
    if ((*this)._posImageCourante == 0)
    {
        (*this)._posImageCourante++;
    }
}

void LecteurVue::reculer()
{
    (*this)._posImageCourante = ((*this)._posImageCourante-1)%(*this).nbImages();
    if ((*this)._posImageCourante == 0)
    {
        (*this)._posImageCourante=(*this).nbImages();
    }
}

void LecteurVue::changerDiaporama(unsigned int pNumDiaporama)
{
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
    /* Chargement des images associées au diaporama courant
       Dans une version ultérieure, ces données proviendront d'une base de données,
       et correspondront au diaporama choisi */
    Image* imageACharger;
    imageACharger = new Image(3, "personne", "Blanche Neige", "C:\\cartesDisney\\carteDisney2.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(2, "personne", "Cendrillon", "C:\\cartesDisney\\carteDisney4.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(4, "animal", "Mickey", "C:\\cartesDisney\\carteDisney1.gif");
    _diaporama.push_back(imageACharger);
    imageACharger = new Image(1, "personne", "Grincheux", "C:\\cartesDisney\\carteDisney1.gif");
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

}

void LecteurVue::viderDiaporama()
{
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

void LecteurVue::mode(bool autoOuManuel)
{
    if (autoOuManuel)
    {
        //
    }
}
