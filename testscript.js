function printArray(arr) {
    for (i = 0; i < arr.length; i++) {
        print(arr[i]);
    }
}

function m3uEntry(file, extinf) {
    this.file = file;
    this.extinf = extinf;
}

//var musicDir = '\\\\192.168.0.194/music/Ivar';
var musicDir = 'C:/Users/Public/Music/Sample Music';
var outPutDir = 'C:/Playlist-Generator/output_test';
var extensions = ["*.mp3", "*.ogg", "*.ape", "*.flac", "*.wav", "*.m4a"];

res = writeFile(extensions, "C:/Playlist-Generator/output_test/test.txt");
print('writeFile result: ', res);

files = getDirContent(musicDir, extensions, true);
var artistList = new Array();
for (i in files) {
    file = files[i];
    print('Processing file ', file);
    var tag = new Tag(file);
    var artist = tag.artist().toLowerCase();
    if (artist == '') {
        artist = 'unknown';
    }
    if (typeof (artistList[artist]) == "undefined") {
        artistList[artist] = new Array();
    }
    artistList[artist].push({
        file: file,
        extinf: tag.length() + "," + artist + " - " + tag.album()
    });
    print(file, tag.length() + "," + artist + " - " + tag.album());
}

for (var artist in artistList) {
    m3uEntries = artistList[artist];
    var playList = new Array();
    playList.push("#EXTM3U");
    for (i in m3uEntries) {
        playList.push("#EXTINF:" + m3uEntries[i].extinf);
        playList.push(m3uEntries[i].file);
    }
    playListFile = outPutDir + "/" + artist + ".m3u";
    print('Writing file ', playListFile);
    ok = writeFile(playList, playListFile);
    if (!ok) {
        print('Error printing ', playListFile);
    }
}



/*
res = copyFiles(files, "C:/Playlist-Generator/output_test", false, true);
print('
        Result of file copy: ', res[0]);
print('
        Log from file copy: ', res[1]);
*/



/*
    var frames = tag.iD3V2Frames();   
 for (var i in frames) {
        //print('
        key is: ' + i + ',
        value is: ' + frames[i]);
        var arr = frames[i];
        for (var j in arr) {
            print('
        key is: ' + i + ',
        value ' + j + '
        is: ' + arr[j]);
        }
    }
*/