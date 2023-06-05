#ifndef DIAPORAMA_H
#define DIAPORAMA_H

#include "image.h"

#include <QSqlDatabase>

#define DATABASE_DIAPOS_NAME "Cmourgue_BD_Diapos_S201" // Le nom du DSN permettant d'accéder à la BD
#define CONNECT_TYPE "QODBC" // Indique que l'on veut utiliser le driver odbc

typedef vector<Image*> diapos;

class Diaporama
{
public:
    Diaporama();
    bool openDataBase();
    void closeDataBase();
    void setDiaporama(unsigned int);
    unsigned int getNbImages();
    Image *getImage(unsigned int);

    QString getTitreDiapo();

private :
    QSqlDatabase m_db;
    unsigned int m_idDiapo;
    QString m_titreDiapo;
    QString m_vitesseDefilementBase;
    diapos m_diapos;
    unsigned int m_id;
    unsigned int m_nbImage;
};

#endif // DIAPORAMA_H
