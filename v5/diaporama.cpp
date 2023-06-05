#include "diaporama.h"
#include "qsqlquery.h"

#include "image.h"


Diaporama::Diaporama()
{

}

bool Diaporama::openDataBase()
{
    m_db = QSqlDatabase::addDatabase(CONNECT_TYPE);
    m_db.setDatabaseName(DATABASE_DIAPOS_NAME);
    return m_db.open();
}

void Diaporama::closeDataBase()
{
    this->m_db.close();
}

void Diaporama::setDiaporama(unsigned int ident)
{
    QSqlQuery req;
    QVariant id = ident;
    if (!this->m_db.isOpen())
    {
        this->openDataBase();
    }
    req.prepare("SELECT `titre Diaporama`, vitesseDefilement FROM Diaporamas WHERE idDiaporama = ? ;");
    req.addBindValue(id);
    if (!req.exec())
    {
        qDebug() << "Diaporama indisponible !";
    }
    else
    {
        this->m_id = ident;
        this->m_titreDiapo = req.value(0).toString();
        this->m_vitesseDefilementBase = req.value(1).toString();

        req.prepare("SELECT idDiapo, rang FROM DiaposDansDiaporama WHERE idDiaporama = ? ;");
        req.addBindValue(id);
        if (!req.exec())
        {
            qDebug() << "Erreur d'exécution de la requête !";
        }
        else
        {
            this->closeDataBase(); // Fermeture pour éviter l'accès de deux éléments en même temps
            for (int i=0;req.next();i++)
            {
                Image* image;
                image = new Image();
                image->setImage(req.value(0).toUInt());
                image->setRang(req.value(1).toUInt());
                this->m_diapos.push_back(image);
                this->m_nbImage++;
            }

            // Tri bulle
            bool trie;
            for (unsigned int cpt1 = this->m_nbImage-1;cpt1>=1;cpt1--)
            {
                trie = true;
                for (unsigned int cpt2=0;cpt2<=cpt1-1;cpt2++)
                {
                    if (this->m_diapos[cpt2]->getRang() > this->m_diapos[cpt2+1]->getRang())
                    {
                        trie = false;
                        // Échange des deux éléments
                        Image *tmp = this->m_diapos[cpt2];
                        this->m_diapos[cpt2] = this->m_diapos[cpt2+1];
                        this->m_diapos[cpt2+1] = tmp;
                    }
                }
                if (trie)
                {
                    break;
                }
             }
        }
    }

    this->closeDataBase();
}

unsigned int Diaporama::getNbImages()
{
    return this->m_nbImage;
}

Image *Diaporama::getImage(unsigned int indice)
{
    return this->m_diapos[indice];
}

QString Diaporama::getTitreDiapo()
{
    return this->m_titreDiapo;
}
