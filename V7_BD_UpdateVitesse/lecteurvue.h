#ifndef LECTEURVUE_H
#define LECTEURVUE_H

#include <QMainWindow>

#include "image.h"
#include <vector>
#include "formvitessedefilement.h"

#include "diaporama.h"

#include "formnumdiapo.h"

// Inclusion supplémentaire de modules
#include <QLabel>

#include "basededonnees.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LecteurVue; }
QT_END_NAMESPACE

typedef vector<Diaporama*> Data;   // Structure de données contenant les infos sur les images


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
    Data m_donnees;            // pointeurs vers les diaporamas
    unsigned int _posImageCourante;  /* position, dans le diaporama,
                                        de l'image courante.
                                        Indéfini quand diaporama vide.
                                        Démarre à 0 quand diaporama non vide */
    unsigned int m_vitesseDefilement = 2; // Entier non signé correspondant au coefficient de vitesse de défilement, de base à 2.
    QLabel * m_lMode; // Le label représentant le mode actuel
    QTimer *m_timer; // Le timer permettant le défilement lors du mode auto
    formVitesseDefilement *m_fVitesse; // Le formulaire pour la vitesse de défilement
    formNumDiapo *m_fNumDiapo; // Le formulaire pour le numéro du diaporama

    BaseDeDonnees *m_mydb;

    int m_indiceDiapo; // car indice dans la liste de pointeur de diaporama m_donnees (= _numDiaporamaCourant -1)

private:
    void chargerDiaporama();    // charge dans _diaporama les images du _numDiaporamaCourant
    void viderDiaporama();      // vide _diaporama de tous ses objets image et les delete

    void mode(bool);                // bascule de auto à manuel ou de manuel à auto
    QString getCheminRscImage(QString); // Renvoie le chemin suivant le fichier ressource de l'image fournie en argument
    void majImage(); // Met à jour l'affichage de l'image etdes informations qui y sont liées

public slots :
    void afficher();            // affiche les informations sur lecteur-diaporama et image courante
    void demandeLancementDiapo(); // Demande le lancement du diapo
    void demandeArretDiapo(); // Demande l'arrêt du diapo
    void demandeAvancerFleche(); // Demande d'avancer le diapo d'une image suite à un clique sur une des flèches
    void demandeReculerFleche(); // Demande de reculer le diapo d'une image suite à un clique sur une des flèches
    void affichageInfosApp();   // affiche les informations (version, date de créa., auteurs) de l'application
    void demandeVider();             // Demande de vider le diaporama actuel
    void demandeModifVitesse(); // Demande et lance le formulaire de modification de la vitesse de défilement

    void demandeChangementDiapo(); // Demande le changement du diaporama (spécifique à la v4)

private slots :
    void demandeAvancer();      // Demande d'avancer d'une image
    void demandeReculer();      // Demande de reculer d'une image
};
#endif // LECTEURVUE_H
