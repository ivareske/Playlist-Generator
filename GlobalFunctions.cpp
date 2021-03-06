
#include "GlobalFunctions.h"

namespace Global {


/*!
 \brief

 \param aPath
 \return QList<QFileInfo>
*/
QList<QFileInfo> getDirContent(const QString& aPath, bool includeSubFolders, const QStringList &extensions, bool *canceled)  {

    // append the filtered files to this list

    //decide to include subfolder or not
    QDirIterator::IteratorFlag subdirFlag;
    if (includeSubFolders) {
        subdirFlag = QDirIterator::Subdirectories;
    }
    else {
        subdirFlag = QDirIterator::NoIteratorFlags;
    }

    // set dir iterator
    QDirIterator* dirIterator = new QDirIterator(aPath,
                                                 extensions,
                                                 QDir::Files | QDir::NoSymLinks,
                                                 subdirFlag);


    QList<QFileInfo> fileInfo;
    QProgressDialog p("Locating files...", "Abort", 0, 0, 0);
    p.setWindowModality(Qt::WindowModal);

    while (dirIterator->hasNext()) {
        if(p.wasCanceled()){
            if(canceled!=0){
                *canceled=true;
            }
            return fileInfo;
        }
        dirIterator->next();
        fileInfo.append(dirIterator->fileInfo());

    }
    p.close();
    delete dirIterator;
    qDebug() << "getDirContent: searched "<<aPath<<", found "<<fileInfo.size()<<" files. Extensions: "<<extensions.join(",");
    return fileInfo;
}

/*!
      \brief
      \param milliSeconds format
    */
QString timeString( uint milliSeconds, const QString &format ){
    QDateTime d1(QDate::currentDate(),QTime(0,0,0,0));
    d1 = d1.addMSecs(milliSeconds);
    return d1.toString(format)+" "+format;
}


/*!
     \brief

     \param in
     \param ok
     \param log
     \return QString
    */
QString versionCheck(QDataStream* in, bool* ok, QString* log) {
    //check Qt version of datastream, and version of application
    //The datastream must be at a position in the stream where the two version
    //number should be located (first in stream)
    //The Qt QDatastream version is checked and set, the application version is returned
    //ok is set to false if the QDataStream version found is larger than
    //the current Qt version

    bool res = true;
    quint32 qversion;
    in->operator >>(qversion);
    QDataStream::Version v = static_cast<QDataStream::Version>(qversion);
    if (v > in->version()) {
        log->append("\nError using the QDataStream operators, the datastream you are trying to read(" + QString::number(v) + ") was created with a greater version of Qt than the current(" + QString::number(in->version()) + ")");
        res = false;
    }
    else {
        in->setVersion(v);
    }
    QString version;

    (*in) >> version;

    if (ok != 0) {
        *ok = res;
    }

    return version;

}


/*!
     \brief

     \return PMSettings *
    */
PMSettings* guiSettings(QObject * parent) {

    return new PMSettings(qApp->applicationDirPath() + "/settings" + Global::ext, PMSettings::IniFormat, parent);

}

/*!
     \brief

     \param s1
     \param s2
     \return bool
    */
bool stringLessThan(const QString& s1, const QString& s2) {
    return naturalCompare(s1, s2, Qt::CaseSensitive) < 0 ;
}


/*!
     \brief

     \param intvals
     \param val
     \return bool
    */
bool checkIntValue(QVector<int> *intvals, const QString& val) {
    //val should be a single value (e.g. 500),
    //or a range: 500-1000
    bool ok;
    int tmp;
    int tmp1;
    int tmp2;
    QStringList vals = val.split("-");
    intvals->clear();
    if (vals.size() == 1) {
        tmp = val.toInt(&ok, 10);
        //qDebug()<<"single value: "<<tmp;
        if (ok) {
            intvals->append(tmp);
        }
    }
    else if (vals.size() == 2) {
        bool ok1;
        bool ok2;
        tmp1 = vals[0].toInt(&ok1, 10);
        tmp2 = vals[1].toInt(&ok2, 10);
        ok = ok1 && ok2;
        if (ok) {
            intvals->append(tmp1);
            intvals->append(tmp2);
        }
        //qDebug()<<"range: "<<tmp1<<" - "<<tmp2;
    }
    else {
        ok = false;
    }
    //qDebug()<<"ok: "<<ok;
    return ok;
}



/*!
     \brief

     \param _a
     \param _b
     \param caseSensitivity
     \return int
    */
int naturalCompare(const QString& _a, const QString& _b, Qt::CaseSensitivity caseSensitivity) {
    // This method chops the input a and b into pieces of
    // digits and non-digits (a1.05 becomes a | 1 | . | 05)
    // and compares these pieces of a and b to each other
    // (first with first, second with second, ...).
    //
    // This is based on the natural sort order code code by Martin Pool
    // http://sourcefrog.net/projects/natsort/
    // Martin Pool agreed to license this under LGPL or GPL.

    // FIXME: Using toLower() to implement case insensitive comparison is
    // sub-optimal, but is needed because we compare strings with
    // localeAwareCompare(), which does not know about case sensitivity.
    // A task has been filled for this in Qt Task Tracker with ID 205990.
    // http://trolltech.com/developer/task-tracker/index_html?method=entry&id=205990
    QString a;
    QString b;
    if (caseSensitivity == Qt::CaseSensitive) {
        a = _a;
        b = _b;
    }
    else {
        a = _a.toLower();
        b = _b.toLower();
    }

    const QChar* currA = a.unicode(); // iterator over a
    const QChar* currB = b.unicode(); // iterator over b

    if (currA == currB) {
        return 0;
    }

    int lean = 0; // comparison of first ignored difference, to be used if all else is equal

    while (!currA->isNull() && !currB->isNull()) {
        const QChar* begSeqA = currA; // beginning of a new character sequence of a
        const QChar* begSeqB = currB;
        if (currA->unicode() == QChar::ObjectReplacementCharacter) {
            return 1;
        }

        if (currB->unicode() == QChar::ObjectReplacementCharacter) {
            return -1;
        }

        if (currA->unicode() == QChar::ReplacementCharacter) {
            return 1;
        }

        if (currB->unicode() == QChar::ReplacementCharacter) {
            return -1;
        }

        // find sequence of characters ending at the first non-character
        while (!currA->isNull() && !currA->isDigit() && !currA->isPunct() && !currA->isSpace()) {
            ++currA;
        }

        while (!currB->isNull() && !currB->isDigit() && !currB->isPunct() && !currB->isSpace()) {
            ++currB;
        }

        // compare these sequences
        const QStringRef& subA(a.midRef(begSeqA - a.unicode(), currA - begSeqA));
        const QStringRef& subB(b.midRef(begSeqB - b.unicode(), currB - begSeqB));
        const int cmp = QStringRef::localeAwareCompare(subA, subB);
        if (cmp != 0) {
            return cmp < 0 ? -1 : +1;
        }

        if (currA->isNull() || currB->isNull()) {
            break;
        }

        // find sequence of characters ending at the first non-character
        while ((currA->isPunct() || currA->isSpace()) && (currB->isPunct() || currB->isSpace())) {
            if (*currA != *currB) {
                // accept difference in punctuation for now ('_' or '.' used instead of ' ').
                if (lean == 0)
                    lean = (*currA < *currB) ? -1 : +1;
            }
            ++currA;
            ++currB;
        }

        // now some digits follow...
        if ((*currA == '0') || (*currB == '0')) {
            // one digit-sequence starts with 0 -> assume we are in a fraction part
            // do left aligned comparison (numbers are considered left aligned)
            while (1) {
                if (!currA->isDigit() && !currB->isDigit()) {
                    break;
                }
                else if (!currA->isDigit()) {
                    return +1;
                }
                else if (!currB->isDigit()) {
                    return -1;
                }
                else if (*currA < *currB) {
                    return -1;
                }
                else if (*currA > *currB) {
                    return + 1;
                }
                ++currA;
                ++currB;
            }
        }
        else {
            // No digit-sequence starts with 0 -> assume we are looking at some integer
            // do right aligned comparison.
            //
            // The longest run of digits wins. That aside, the greatest
            // value wins, but we can't know that it will until we've scanned
            // both numbers to know that they have the same magnitude.

            bool isFirstRun = true;
            int weight = 0;
            while (1) {
                if (!currA->isDigit() && !currB->isDigit()) {
                    if (weight != 0) {
                        return weight;
                    }
                    break;
                }
                else if (!currA->isDigit()) {
                    if (isFirstRun) {
                        return *currA < *currB ? -1 : +1;
                    }
                    else {
                        return -1;
                    }
                }
                else if (!currB->isDigit()) {
                    if (isFirstRun) {
                        return *currA < *currB ? -1 : +1;
                    }
                    else {
                        return +1;
                    }
                }
                else if ((*currA < *currB) && (weight == 0)) {
                    weight = -1;
                }
                else if ((*currA > *currB) && (weight == 0)) {
                    weight = + 1;
                }
                ++currA;
                ++currB;
                isFirstRun = false;
            }
        }
        continue;
    }

    if (currA->isNull() && currB->isNull()) {
        return lean;
    }

    return currA->isNull() ? -1 : + 1;
}


}



/*!
 \brief

 \param key
 \return uint
*/
uint qHash(const QFileInfo& key) {

    uint val = qHash(key.absoluteFilePath());
    return val;
}

/*!
 \brief

 \param dir
 \return int
*/
int qHash(const QDir& dir) {

    return qHash(dir.absolutePath());
}
