function printArray(arr) {
    for (i = 0; i < arr.length; i++) {
        print(arr[i]);
    }
}

function m3uEntry(file, extinf) {
    this.file = file;
    this.extinf = extinf;
}

var musicDir = 'C:/Users/Public/Music/Sample Music';
var outPutDir = 'C:/Playlist-Generator/output_test';
var extensions = ["*.mp3", "*.ogg", "*.ape", "*.flac", "*.wav", "*.m4a"];

res = writeFile(extensions, "C:/Playlist-Generator/output_test/test.txt");
print('writeFile result: ', res);

files = getDirContent(musicDir, extensions, true);
var artistList = new Array();
for (files in files) {
    print('Processing file ', file);
    var tag = new Tag(file);
    tag.readTags();
    tag.readFrames();
    if (typeof (artistList[tag.artist().toLowerCase()]) == "undefined") {
        artistList[tag.artist().toLowerCase()] = new Array();
    }
    var entry = m3uEntry(file, tag.length() + "," + tag.artist() + " - " + tag.album());
    artistList[tag.artist().toLowerCase()].push(entry);
}

for (i in artistList) {
    m3uEntries = artistList[i];
    var playList = new Array();
    playList.push("#EXTM3U");
    for (i in m3uEntries) {
        entry = m3uEntries[i];
        playList.push("#EXTINF:" + entry.extinf);
    }
    playListFile = outPutDir + "/" + i + ".m3u";
    ok = writeFile(playList, playListFile);
    if (!ok) {
        print('Error printing ', playListFile);
    }
}

//res = copyFiles(files, "C:/Playlist-Generator/output_test", false, true);
//print('Result of file copy:', res[0]);
//print('Log from file copy:', res[1]);








/*
    var frames = tag.iD3V2Frames();   
 for (var i in frames) {
        //print('key is: ' + i + ', value is: ' + frames[i]);
        var arr = frames[i];
        for (var j in arr) {
            print('key is: ' + i + ', value ' + j + ' is: ' + arr[j]);
        }
    }
*/