#include "image.h"

#include "qsqlquery.h"

#include<QDebug>
#include <QString>

Image::Image(unsigned int pRang, QString pCategorie, QString pTitre, QString pChemin)
{
    _rang = pRang;
    _categorie = pCategorie;
    _titre = pTitre;
    _chemin = pChemin;
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

bool Image::openDataBase()
{
    m_dbI = QSqlDatabase::addDatabase(CONNECT_TYPE);
    m_dbI.setDatabaseName(DATABASE_IMAGE_NAME);
    return m_dbI.open();
}

void Image::closeDataBase()
{
    this->m_dbI.close();
}

void Image::setImage(unsigned int identifiant)
{
    QSqlQuery req;
    QVariant id = identifiant;
    if (!this->m_dbI.isOpen())
    {
        this->openDataBase();
    }
    req.prepare("SELECT Diapos.titrePhoto, Familles.nomFamille, Diapos.uriPhoto FROM Diapos JOIN Familles ON Diapos.idFam = Familles.idFamille WHERE idphoto = ? ;");
    req.addBindValue(id);
    if (!req.exec())
    {
        qDebug() << "Photo indisponible !";
    }
    else
    {
        this->m_id = identifiant;
        this->_chemin = req.value(2).toString();
        this->_categorie = req.value(1).toString();
        this->_titre = req.value(0).toString();
    }

    this->closeDataBase();
}

void Image::setRang(unsigned int r)
{
    this->_rang=r;
}
