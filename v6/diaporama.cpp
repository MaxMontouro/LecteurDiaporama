#include "diaporama.h"
#include "qsqlquery.h"

#include "image.h"

#include <tuple>

typedef vector<tuple<unsigned int, unsigned int>> resQueryDiapo;   // Structure de données permettant de conserver les résultats d'une requête même après la fermeturede la base de données sans utiliser deux fois en même temps une base de données

Diaporama::Diaporama(BaseDeDonnees* bd)
{
    this->m_nbImage = 0;

    this->m_dbDiapo = bd;
}

void Diaporama::setDiaporama(unsigned int ident)
{
    QSqlQuery req;
    if (!this->m_dbDiapo->isAlreadyOpen())
    {
        this->m_dbDiapo->openDataBase();
    }
    req.prepare("SELECT `titre Diaporama`, vitesseDefilement FROM Diaporamas WHERE idDiaporama = ? ;");
    req.addBindValue(ident);
    if (!req.exec())
    {
        qDebug() << "Diaporama indisponible !";
    }
    else
    {
        req.next();
        this->m_id = ident;
        this->m_titreDiapo = req.value(0).toString();
        this->m_vitesseDefilementBase = req.value(1).toString();

        req.prepare("SELECT idDiapo, rang FROM DiaposDansDiaporama WHERE idDiaporama = ? ;");
        req.addBindValue(ident);
        if (!req.exec())
        {
            qDebug() << "Erreur d'exécution de la requête !";
        }
        else
        {
            // Récupération et conservation des résultats de la requête
            unsigned int cpt = 0;
            resQueryDiapo resQ;
            tuple<unsigned int, unsigned int> enregistrement;
            for (int i = 0; req.next(); i++)
            {
                enregistrement = make_tuple(req.value(0).toUInt(),req.value(1).toUInt());
                resQ.push_back(enregistrement);
                cpt++;
            }

            for (unsigned int i=0; i < cpt;i++)
            {
                Image* image;
                image = new Image(this->m_dbDiapo);
                image->setImage(get<0>(resQ[i])); // id
                image->setRang(get<1>(resQ[i])); // rang
                this->m_diapos.push_back(image);
                this->m_nbImage++;
                //image->afficher(); // Pour vérifier
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
