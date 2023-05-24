#ifndef LECTEURVUE_H
#define LECTEURVUE_H

#include <QMainWindow>

#include "image.h"
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class LecteurVue; }
QT_END_NAMESPACE

typedef vector<Image*> Diaporama;   // Structure de données contenant les infos sur les images

class LecteurVue : public QMainWindow
{
    Q_OBJECT

public:
    LecteurVue(QWidget *parent = nullptr);
    ~LecteurVue();
    void avancer();             // incrémente _posImageCourante, modulo nbImages()
    void reculer();             // décrémente _posImageCourante, modulo nbImages()
    void changerDiaporama(unsigned int pNumDiaporama);    // permet de choisir un diaporama, 0 si aucun diaporama souhaité
    unsigned int nbImages();    // affiche la taille de _diaporama
    Image* imageCourante();     // retourne le pointeur vers l'image courante
    unsigned int numDiaporamaCourant();

private:
    Ui::LecteurVue *ui;
    unsigned _numDiaporamaCourant;   // numéro du diaporama courant, par défaut 0
    Diaporama _diaporama;            // pointeurs vers les images du diaporama
    unsigned int _posImageCourante;  /* position, dans le diaporama,
                                        de l'image courante.
                                        Indéfini quand diaporama vide.
                                        Démarre à 0 quand diaporama non vide */
    bool m_modeAuto = false;        // Booléen indiquant si le mode auto est activé ou non
private:
    void chargerDiaporama();    // charge dans _diaporama les images du _numDiaporamaCourant
    void viderDiaporama();      // vide _diaporama de tous ses objets image et les delete

    void mode(bool);                // bascule de auto à manuel ou de manuel à auto
    QString getCheminRscImage(string); // Renvoie le chemin suivant le fichier ressource de l'image fournie en argument

public slots :
    void afficher();            // affiche les informations sur lecteur-diaporama et image courante
    void demandeChangementMode(); // Demande le changement de mode
    void affichageInfosApp();   // affiche les informations (version, date de créa., auteurs) de l'application
    void demandeAvancer();      // Demande d'avancer d'une image
    void demandeReculer();      // Demande de reculer d'une image
    void demandeVider();             // Demande de vider le diaporama actuel
};
#endif // LECTEURVUE_H
