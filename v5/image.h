#ifndef IMAGE_H
#define IMAGE_H
#include <iostream>
using namespace std;

#include <QSqlDatabase>

#define DATABASE_IMAGE_NAME "Cmourgue_BD_Image_S201" // Le nom du DSN permettant d'accéder à la BD
#define CONNECT_TYPE "QODBC" // Indique que l'on veut utiliser le driver odbc

class Image
{
public:
    Image(unsigned int pRang=0,
              QString pCategorie="", QString pTitre="", QString pChemin = "");
    unsigned int getRang();
    QString getCategorie();
    QString getTitre();
    QString getChemin();
    void afficher();           // affiche tous les champs de l'image

    void setRang(unsigned int);
    void setImage(unsigned int);

private:
    unsigned int _rang;         /* rang de l'image au sein du diaporama
                                   auquel l'image est associée */
    QString _titre;              // intitulé de l'image
    QString _categorie;          // catégorie de l'image (personne, animal, objet)
    QString _chemin;             // chemin complet vers le dossier où se trouve l'image
    QSqlDatabase m_dbI;
    unsigned int m_id;

private :
    bool openDataBase();
    void closeDataBase();
};

#endif // IMAGE_H
