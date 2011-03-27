#include "settingsClass.h"

 
settingsClass::settingsClass(){
    
	defaultOutputFolder="C:\\";
	defaultExtensions = "*.mp3;*.wma;*.wav;*.ogg;*.aac;*.ac3";
	format = "%artist% - %title%";
	//openmp = false;
	showLog = true;
	artistEmpty=true;
	titleEmpty=true;
	albumEmpty=false;
	commentEmpty=false;
	genreEmpty=false;
	trackEmpty=false;
	yearEmpty=false;
	
}

QStringList settingsClass::readSettingsStringList( QString settingsFile ){
	
	QStringList list;
	
	QFile file( settingsFile );
	QString line;
	if (!file.open(QIODevice::ReadOnly)){		
		return list;				
	}
    QTextStream t( &file );	
	line = t.readLine(); list.append(line);
    while ( !line.isNull() ) {
		line = t.readLine();
		if(!line.isNull()){
			list.append(line);
		}
	}
	return list;
}


QList<playList> settingsClass::readSettings( QString settingsFile ){

	QList<playList> playLists;
	
	QString line;
	QStringList list = readSettingsStringList( settingsFile );

	if (list.size()==0){		
		return playLists;				
	}
	bool ok;
	
	int k=0;
    while ( k<list.size() ) {
		
		line = list[k];
		//qDebug()<<"line "<<line;
		
		if(line=="defaultOutputFolder"){
			k++;line = list[k];
			defaultOutputFolder = line;
			//qDebug()<<"defaultOutputFolder: "<<defaultOutputFolder;
		}else if(line=="defaultExtensions"){
			k++;line = list[k];
			defaultExtensions = line;
			//qDebug()<<"defaultExtensions: "<<defaultExtensions;
		}else if(line=="format"){
			k++;line = list[k];
			format = line;
			//qDebug()<<"format: "<<format;
		//}else if(line=="openmp"){
		//	k++;line = list[k];
			//openmp = (bool)line.toInt(&ok,10);
			////qDebug()<<"openmp: "<<openmp;
		}else if(line=="showLog"){
			k++;line = list[k];
			showLog = (bool)line.toInt(&ok,10);
			//qDebug()<<"showLog: "<<showLog;
		}else if(line=="artistEmpty"){
			k++;line = list[k];
			artistEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"artistEmpty: "<<artistEmpty;
		}else if(line=="titleEmpty"){
			k++;line = list[k];
			titleEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"titleEmpty: "<<titleEmpty;
		}else if(line=="albumEmpty"){
			k++;line = list[k];
			albumEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"albumEmpty: "<<albumEmpty;
		}else if(line=="commentEmpty"){
			k++;line = list[k];
			commentEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"commentEmpty: "<<commentEmpty;
		}else if(line=="genreEmpty"){
			k++;line = list[k];
			genreEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"genreEmpty: "<<genreEmpty;
		}else if(line=="trackEmpty"){
			k++;line = list[k];
			trackEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"trackEmpty: "<<trackEmpty;
		}else if(line=="yearEmpty"){
			k++;line = list[k];
			yearEmpty = (bool)line.toInt(&ok,10);
			//qDebug()<<"yearEmpty: "<<yearEmpty;
		}else if(line.left(8)=="playlist"){
			playList tmp;tmp.name = line.remove(0,9);
			//qDebug()<<"FOUND PLAYLIST: "<<tmp.name;
			//read folders
			while( 1 ){
				k++;line = list[k];
				if( line.isEmpty() ){
					//qDebug()<<"break! "<<line;
					break;
				}
				if( line=="ENDFOLDERS" ){
					k++;line = list[k];
					tmp.extensions = line;
					k++;line = list[k];
					tmp.randomize = (bool)line.toInt(&ok,10);
					k++;line = list[k];
					tmp.includeSubFolders = (bool)line.toInt(&ok,10);
					k++;line = list[k];
					tmp.allRulesTrue = (bool)line.toInt(&ok,10);					
					k++;line = list[k];
					tmp.includeExtInf = (bool)line.toInt(&ok,10);
					k++;line = list[k];
					tmp.relativePath = (bool)line.toInt(&ok,10);
					k++;line = list[k];
					tmp.makeUnique = (bool)line.toInt(&ok,10);
					break;
				}else{
					tmp.folders.append(line);
				}
			}				
			while( 1 ){
				//qDebug()<<line;
				k++;line = list[k];
				if( line.isEmpty() || line=="startindividualfiles" || line.left(8)=="playlist" ){
					break;
				}
				rule r;
				r.type = (ruleType)line.toInt(&ok,10);
				//qDebug()<<"rule type: "<<r.type;
				k++;line = list[k];
				r.value = line;
				//qDebug()<<"rule value"<<r.value;
				k++;line = list[k];
				r.shouldBeTrue = (bool)line.toInt(&ok,10);
				//qDebug()<<"rule value"<<r.shouldBeTrue;
				tmp.rules.append( r );
			}		
			if( line=="startindividualfiles" ){			
				while(1){
					//qDebug()<<line;
					k++;line = list[k];					
					if( line=="endindividualfiles" || line.isEmpty() || line.left(8)=="playlist" ){
						break;
					}
					tmp.individualFiles.append( line );
				}
			}
			playLists.append( tmp );
		}else{			
			k++;
			//line = list[k];
		}
		
		////qDebug()<<"line: "<<line;
		
    }
	qDebug()<<"finished reading settings";
    
	return playLists;
}


QStringList settingsClass::createSettingsStringList( QString settingsFile, QList<playList> playLists ){

	QStringList ts;
	ts.append("defaultOutputFolder");
	ts.append(defaultOutputFolder);
	ts.append("defaultExtensions");
	ts.append(defaultExtensions);		
	ts.append("format");
	ts.append(format);		
	//ts.append("openmp");
	//ts.append(openmp);	
	ts.append("showLog");
	ts.append(QString::number((int)showLog));
	ts.append("artistEmpty");
	ts.append(QString::number((int)artistEmpty));
	ts.append("titleEmpty");
	ts.append(QString::number((int)titleEmpty));
	ts.append("albumEmpty");
	ts.append(QString::number((int)albumEmpty));
	ts.append("commentEmpty");
	ts.append(QString::number((int)commentEmpty));
	ts.append("genreEmpty");
	ts.append(QString::number((int)genreEmpty));
	ts.append("trackEmpty");
	ts.append(QString::number((int)trackEmpty));
	ts.append("yearEmpty");
	ts.append(QString::number((int)yearEmpty));	
	
	for(int i=0;i<playLists.size();i++){
		ts.append("playlist="+playLists[i].name);
		for(int j=0;j<playLists[i].folders.size();j++){
			ts.append(playLists[i].folders[j]);
		}
		ts.append("ENDFOLDERS");
		ts.append(playLists[i].extensions);		
		ts.append(QString::number((int)playLists[i].randomize));		
		ts.append(QString::number((int)playLists[i].includeSubFolders));	
		ts.append(QString::number((int)playLists[i].allRulesTrue));				
		ts.append(QString::number((int)playLists[i].includeExtInf));		
		ts.append(QString::number((int)playLists[i].relativePath));	
		ts.append(QString::number((int)playLists[i].makeUnique));	
		
		for(int j=0;j<playLists[i].rules.size();j++){
			ts.append(QString::number((int)playLists[i].rules[j].type)) ;
			ts.append(playLists[i].rules[j].value);
			ts.append(QString::number((int)playLists[i].rules[j].shouldBeTrue));
		}
		ts.append("startindividualfiles");
		for(int j=0;j<playLists[i].individualFiles.size();j++){
			ts.append(playLists[i].individualFiles[j] );			
		}
		ts.append("endindividualfiles");
	}
	
	return ts;

}

bool settingsClass::settingsHaveChanged( QString settingsFile, QList<playList> playLists ){
	
	QStringList current  = createSettingsStringList( settingsFile, playLists );
	QStringList existing = readSettingsStringList( settingsFile );
	
	if( current.size()!=existing.size() ){
		qDebug()<<current.size()<<existing.size();
		return true;
	}
	
	bool result = false;
	for(int i=0;i<current.size();i++){
		qDebug()<<current[i];
		qDebug()<<existing[i];
		if( existing[i]!=current[i] ){
			result = true;
			break;
		}
	}
	
	return result;
	
}


void settingsClass::saveSettings( QString settingsFile, QList<playList> playLists ){

	QFile f( settingsFile );

	if( !f.open( QIODevice::WriteOnly ) ){
		QMessageBox::warning(0, "",
		"Could not open "+settingsFile+" for writing",
		QMessageBox::Ok, QMessageBox::Ok);
	    return;
	}
	QTextStream ts( &f );
	
	QStringList out = createSettingsStringList( settingsFile, playLists );
	for(int i=0;i<out.size();i++){
		ts << out[i];
		if(i!=out.size()-1){
			ts << endl;
		}
	}
	
	
	f.close();
}











