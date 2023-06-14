#include "image.h"

#include "qsqlquery.h"

#include<QDebug>
#include <QString>

Image::Image(BaseDeDonnees* bd,
             unsigned int pRang, QString pCategorie, QString pTitre, QString pChemin)
{
    _rang = pRang;
    _categorie = pCategorie;
    _titre = pTitre;
    _chemin = pChemin;
    this->m_dbI = bd;
}

unsigned int Image::getRang()
{
    return _rang;
}

QString Image::getCategorie()
{
    return _categorie;
}

QString Image::getTitre()
{
    return _titre;
}

QString Image::getChemin()
{
    return _chemin;
}

void Image::afficher()
{
    string titre = getTitre().toStdString();
    string cat = getCategorie().toStdString();
    string chem = getChemin().toStdString();
    cout << "image( rang:" << getRang() << ", titre:" << titre << ", categorie:"
         << cat << ", chemin:"<< chem << ")" << endl;
}

void Image::setImage(unsigned int identifiant)
{
    QSqlQuery req;
    if (!this->m_dbI->isAlreadyOpen())
    {
        this->m_dbI->openDataBase();
    }
    req.prepare("SELECT Diapos.titrePhoto, Familles.nomFamille, Diapos.uriPhoto FROM Diapos JOIN Familles ON Diapos.idFam = Familles.idFamille WHERE Diapos.idphoto = ? ;");
    req.addBindValue(identifiant);
    if (!req.exec())
    {
        qDebug() << "Photo indisponible !";
    }
    else
    {
        req.next();
        this->m_id = identifiant;
        this->_titre = req.value(0).toString();
        this->_categorie = req.value(1).toString();
        this->_chemin = req.value(2).toString();

    }
}

void Image::setRang(unsigned int r)
{
    this->_rang=r;
}
