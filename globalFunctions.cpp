
#include "globalFunctions.h"

namespace Global{


    bool caseInsensitiveLessThan( const playList &s1, const playList &s2 ){
        return s1.name().toLower() < s2.name().toLower();
    }

    QString getRuleName( const rule::ruleType &type ){

	QString ans="";
        if( type==rule::FILENAME_CONTAINS ){
            ans = "File name contains";
        }else if( type==rule::FILENAME_EQUALS ){
            ans = "File name equals";
        }else if( type==rule::TAG_TITLE_CONTAINS ){
            ans = "Title tag contains";
        }else if( type==rule::TAG_TITLE_EQUALS ){
            ans = "Title tag equals";
        }else if( type==rule::TAG_ARTIST_CONTAINS ){
            ans = "Artist tag contains";
        }else if( type==rule::TAG_ARTIST_EQUALS ){
            ans = "Artist tag equals";
        }else if( type==rule::TAG_ALBUM_CONTAINS ){
            ans = "Album tag contains";
        }else if( type==rule::TAG_ALBUM_EQUALS ){
            ans = "Album tag equals";
        }else if( type==rule::TAG_YEAR_IS ){
            ans = "Year tag is";
        }else if( type==rule::TAG_COMMENT_CONTAINS ){
            ans = "Comment tag contains";
        }else if( type==rule::TAG_COMMENT_EQUALS ){
            ans = "Comment tag equals";
        }else if( type==rule::TAG_TRACK_IS ){
            ans = "Track tag is";
        }else if( type==rule::TAG_GENRE_CONTAINS ){
            ans = "Genre tag contains";
        }else if( type==rule::TAG_GENRE_EQUALS ){
            ans = "Genre tag equals";
        }else if( type==rule::AUDIO_BITRATE_IS ){
            ans = "Audio bitrate is";
        }else if( type==rule::AUDIO_SAMPLERATE_IS ){
            ans = "Audio samplerate is";
        }else if( type==rule::AUDIO_CHANNELS_IS ){
            ans = "Audio channels is";
        }else if( type==rule::AUDIO_LENGTH_IS ){
            ans = "Audio lenght is";
	}
	
	return ans;

    }


    QString getRuleExample( const rule::ruleType &type ){


	QString ans="";
        if( type==rule::FILENAME_CONTAINS || type==rule::FILENAME_EQUALS ){
            ans = "pantera";
        }else if( type==rule::TAG_TITLE_CONTAINS || type==rule::TAG_TITLE_EQUALS ){
            ans = "acdc";
        }else if( type==rule::TAG_ARTIST_CONTAINS || type==rule::TAG_ARTIST_EQUALS ){
            ans = "acdc";
        }else if( type==rule::TAG_ALBUM_CONTAINS || type==rule::TAG_ALBUM_EQUALS ){
            ans = "live";
        }else if( type==rule::TAG_YEAR_IS ){
            ans = "Single nr.: '2001', or a range: '1990-1998'";
        }else if( type==rule::TAG_COMMENT_CONTAINS || type==rule::TAG_COMMENT_EQUALS ){
            ans = "whatever...";
        }else if( type==rule::TAG_TRACK_IS ){
            ans = "Single nr.: '1', or a range: '7-12'";
        }else if( type==rule::TAG_GENRE_CONTAINS || type==rule::TAG_GENRE_EQUALS ){
            ans = "'rock'";
        }else if( type==rule::AUDIO_BITRATE_IS ){
            ans = "Single nr.: '128', or a range: '128-196'";
        }else if( type==rule::AUDIO_SAMPLERATE_IS ){
            ans = "Single nr.: '44100', or a range: '44100-9999999'";
        }else if( type==rule::AUDIO_CHANNELS_IS ){
            ans = "Single nr.: '2', or a range: '2-6'";
        }else if( type==rule::AUDIO_LENGTH_IS ){
            ans = "Single nr.: '300', or a range: '500-700' (use seconds) ";
	}
	
	return ans;

    }


    bool checkIntValue( QVector<int> *intvals, const QString &val ){
	//val should be a single value (e.g. 500),
	//or a range: 500-1000
	bool ok; int tmp; int tmp1; int tmp2;
	QStringList vals = val.split("-");
	intvals->clear();	
	if(vals.size()==1){
            tmp = val.toInt( &ok, 10 );
            //qDebug()<<"single value: "<<tmp;
            if(ok){
                intvals->append(tmp);
            }
	}else if( vals.size()==2 ){
            bool ok1;
            bool ok2;
            tmp1 = vals[0].toInt( &ok1, 10 );
            tmp2 = vals[1].toInt( &ok2, 10 );
            ok = ok1 && ok2;
            if(ok){
                intvals->append(tmp1);
                intvals->append(tmp2);
            }
            //qDebug()<<"range: "<<tmp1<<" - "<<tmp2;
	}else{
            ok = false;
	}
	//qDebug()<<"ok: "<<ok;
	return ok;
    }


}

uint qHash( const m3uEntry &key ){
    //QString tmp = key.file;
    uint val = qHash( key.file() );
    return val;
}

uint qHash( const QFileInfo &key ){

    uint val = qHash( key.absoluteFilePath() );
    return val;
}
