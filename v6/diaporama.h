#ifndef DIAPORAMA_H
#define DIAPORAMA_H

#include "image.h"

#include "basededonnees.h"

typedef vector<Image*> diapos;

class Diaporama
{
public:
    Diaporama(BaseDeDonnees* = new BaseDeDonnees());
    void setDiaporama(unsigned int);
    unsigned int getNbImages();
    Image *getImage(unsigned int);

    QString getTitreDiapo();

private :
    BaseDeDonnees *m_dbDiapo;
    unsigned int m_idDiapo;
    QString m_titreDiapo;
    QString m_vitesseDefilementBase;
    diapos m_diapos;
    unsigned int m_id;
    unsigned int m_nbImage;
};

#endif // DIAPORAMA_H
