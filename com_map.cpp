#include "cOM_Map.h"

void cOM_Map::loadMap(QString newMapString)
{
    // Convert to StringList then pass to StringList Handler
    loadMap(newMapString.split("\n",QString::KeepEmptyParts));
}

void cOM_Map::loadMap(QFileInfo newMapPath)
{
    // Convert to StringList then pass to StringList Handler
    QStringList mapStringList;
    QFile newMapFile;

    newMapFile.setFileName(newMapPath.filePath());
    if (newMapFile.open(QFile::ReadOnly)) {
        QTextStream mapStream(&newMapFile);
        while (!mapStream.atEnd()) {
            mapStringList.append(mapStream.readLine());
        }
    } else {
        qDebug() << "Map cannot be opened. \n"
                 << "Map Path: "
                 << newMapFile.fileName();
    }

    loadMap(mapStringList);
}

cOM_Map::cOM_Map()
{
    // Load Default Map
    audioFileName    = "audio.mp3"         ;
    specialStyle     = false               ;
    widescreen       = false               ;
    distanceSpacing  = 1.0                 ;
    beatDivisor      = 4                   ;
    gridSize         = 4                   ;
    timelineZoom     = 1.0                 ;
    title            = "unknownTitle"      ;
    unicodeTitle     = "unknownUTitle"     ;
    artist           = "unknownArtist"     ;
    unicodeArtist    = "unknownUArtist"    ;
    creator          = "unknownCreator"    ;
    difficultyName   = "unknownDifficulty" ;
    source           = ""                  ;
    tags             = QStringList("")     ;
    beatmapID        = 0                   ;
    beatmapSetID     = -1                  ;
    HP               = 5                   ;
    CS               = 5                   ;
    OD               = 5                   ;
    AR               = 5                   ;
    sliderMult       = 1.0                 ;
    sliderTick       = 1                   ;
    bgFileName       = "bg.jpg"            ;
    OM_HOList        = cOM_HOList()        ;
    OM_TPList        = cOM_TPList()        ;
}

void cOM_Map::loadMap(QStringList newMapStringList)
{
    QList<int> indexMapStringList;
    indexMapStringList = findMapSettings(newMapStringList);

    // We use the default value if there is nothing in the QString from QStringList

    audioFileName    = "audio.mp3"         ;
    specialStyle     = false               ;
    widescreen       = false               ;
    distanceSpacing  = 1.0                 ;
    beatDivisor      = 4                   ;
    gridSize         = 4                   ;
    timelineZoom     = 1.0                 ;
    title            = "unknownTitle"      ;
    unicodeTitle     = "unknownUTitle"     ;
    artist           = "unknownArtist"     ;
    unicodeArtist    = "unknownUArtist"    ;
    creator          = "unknownCreator"    ;
    difficultyName   = "unknownDifficulty" ;
    source           = ""                  ;
    tags             = QStringList("")     ;
    beatmapID        = 0                   ;
    beatmapSetID     = -1                  ;
    HP               = 5                   ;
    CS               = 5                   ;
    OD               = 5                   ;
    AR               = 5                   ;
    sliderMult       = 1.0                 ;
    sliderTick       = 1                   ;
    bgFileName       = "bg.jpg"            ;
    OM_HOList        = cOM_HOList()        ;
    OM_TPList        = cOM_TPList()        ;


    if (indexMapStringList[0]  != -1) { audioFileName
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified(); }
    if (indexMapStringList[1]  != -1) { audioLeadIn
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[2]  != -1) { previewTime
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[3]  != -1) { countdown
                = bool (newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[4]  != -1) { sampleSet
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[5]  != -1) { stackLeniency
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[6]  != -1) { gameMode
                = newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[7]  != -1) { letterbox
                = bool (newMapStringList[indexMapStringList[0]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[8]  != -1) { specialStyle
                = bool (newMapStringList[indexMapStringList[8]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[9]  != -1) { widescreen
                = bool (newMapStringList[indexMapStringList[9]].split(":")[1].simplified().toInt()); }
    if (indexMapStringList[10] != -1) { distanceSpacing
                = newMapStringList[indexMapStringList[10]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[11] != -1) { beatDivisor
                = newMapStringList[indexMapStringList[11]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[12] != -1) { gridSize
                = newMapStringList[indexMapStringList[12]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[13] != -1) { timelineZoom
                = newMapStringList[indexMapStringList[13]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[14] != -1) { title
                =newMapStringList[indexMapStringList[14]].right // We do .right() instead of split just in case parameter contains ':'
                (
                    newMapStringList[indexMapStringList[14]].length() -newMapStringList[indexMapStringList[14]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[15] != -1) { unicodeTitle
                =newMapStringList[indexMapStringList[15]].right
                (
                    newMapStringList[indexMapStringList[15]].length() -newMapStringList[indexMapStringList[15]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[16] != -1) { artist
                =newMapStringList[indexMapStringList[16]].right
                (
                    newMapStringList[indexMapStringList[16]].length() -newMapStringList[indexMapStringList[16]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[17] != -1) { unicodeArtist
                =newMapStringList[indexMapStringList[17]].right
                (
                    newMapStringList[indexMapStringList[17]].length() -newMapStringList[indexMapStringList[17]].indexOf(":") - 1
                ).simplified();
    }
    if (indexMapStringList[18] != -1) { creator
                = newMapStringList[indexMapStringList[18]].split(":")[1].simplified();  }
    if (indexMapStringList[19] != -1) { difficultyName
                = newMapStringList[indexMapStringList[19]].right
                  (
                     newMapStringList[indexMapStringList[19]].length() -newMapStringList[indexMapStringList[19]].indexOf(":") - 1
                  ).simplified();
    }
    if (indexMapStringList[20] != -1) { source
                = newMapStringList[indexMapStringList[20]].right
                  (
                     newMapStringList[indexMapStringList[20]].length() -newMapStringList[indexMapStringList[20]].indexOf(":") - 1
                  ).simplified();
    }
    if (indexMapStringList[21] != -1) { //tags
        QString trimTag;

        trimTag =newMapStringList[indexMapStringList[21]].right
                  (
                     newMapStringList[indexMapStringList[21]].length() -newMapStringList[indexMapStringList[21]].indexOf(":") - 1
                  ).simplified();

        trimTag.replace(",", " "); // Just in case tags are separated by ','
        tags             = trimTag.split(" ",QString::SkipEmptyParts);
    }
    if (indexMapStringList[22] != -1) { beatmapID
                = newMapStringList[indexMapStringList[22]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[23] != -1) { beatmapSetID
                = newMapStringList[indexMapStringList[23]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[24] != -1) { HP
                = newMapStringList[indexMapStringList[24]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[25] != -1) { CS
                = newMapStringList[indexMapStringList[25]].split(":")[1].simplified().toDouble();
                }
    if (indexMapStringList[26] != -1) { OD
                = newMapStringList[indexMapStringList[26]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[27] != -1) { AR
                = newMapStringList[indexMapStringList[27]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[28] != -1) { sliderMult
                = newMapStringList[indexMapStringList[28]].split(":")[1].simplified().toDouble(); }
    if (indexMapStringList[29] != -1) { sliderTick
                = newMapStringList[indexMapStringList[29]].split(":")[1].simplified().toInt(); }
    if (indexMapStringList[30] != -1) { //bgFileName
        QString lineBGFileName,
                trimBGFileName;
        int firstQuoteIndex,
            secondQuoteIndex;

        lineBGFileName = newMapStringList[indexMapStringList[30] + 1].simplified();

        firstQuoteIndex = lineBGFileName.indexOf("\"");
        secondQuoteIndex = lineBGFileName.indexOf("\"", firstQuoteIndex + 1);

        trimBGFileName = lineBGFileName.mid(firstQuoteIndex + 1, secondQuoteIndex - firstQuoteIndex - 1);

        bgFileName = trimBGFileName;
                 }
    if (indexMapStringList[31] != -1) { //videoFileName
        QString lineVideoFileName;
        int firstQuoteIndex,
            secondQuoteIndex;

        lineVideoFileName = newMapStringList[indexMapStringList[31]].simplified();

        firstQuoteIndex  = lineVideoFileName.indexOf("\"");
        secondQuoteIndex = lineVideoFileName.indexOf("\"", firstQuoteIndex + 1);

        videoFileName = lineVideoFileName.mid(firstQuoteIndex + 1, secondQuoteIndex - firstQuoteIndex - 1);
                 }
    if (indexMapStringList[32] != -1) { //BreakP

        int startBreakPIndex,
            endBreakPIndex;

        startBreakPIndex = endBreakPIndex = indexMapStringList[32] + 1;

        for (int temp = 0;
             temp < 50; // We assume there are no maps with more than 50 breaks.
             temp ++)
        {
            if (newMapStringList[temp + startBreakPIndex].left(1) == "/"){
                endBreakPIndex = temp + startBreakPIndex - 1;
                break;
            }
        }

        for (int temp = startBreakPIndex; temp <= endBreakPIndex; temp ++)
        {
            if (newMapStringList[temp].simplified() != ""){
                breakPList.append(cOM_BreakP(newMapStringList[temp]));
            }
        }
                 }
    if (indexMapStringList[33] != -1) { //OM_TPList
        int startOM_TPIndex,
            endOM_TPIndex;

        startOM_TPIndex = indexMapStringList[31] + 1;
        endOM_TPIndex = indexMapStringList[32] - 1;

        qDebug() << "startOM_TPIndex: " << startOM_TPIndex;
        qDebug() << "endOM_TPIndex: "   << endOM_TPIndex;

        for (int temp = startOM_TPIndex; temp <= endOM_TPIndex; temp ++)
        {
            if (newMapStringList[temp].simplified() != ""){
                OM_TPList.append(cOM_TP(newMapStringList[temp]));
            }
        }

    }
    if (indexMapStringList[34] != -1) { //OM_HOList
        int startOM_HOIndex,
            endOM_HOIndex;

        startOM_HOIndex = indexMapStringList[32] + 1;
        endOM_HOIndex = newMapStringList.length();

        qDebug() << "startOM_HOIndex: " << startOM_HOIndex;
        qDebug() << "endOM_HOIndex: "   << endOM_HOIndex;

        for (int temp = startOM_HOIndex; temp < endOM_HOIndex; temp ++)
        {
            if (newMapStringList[temp].simplified() != ""){
                OM_HOList.append(cOM_HO(newMapStringList[temp], CS));
            }
        }
    }
}

void cOM_Map::getInfo()
{
    qDebug() << "\r\n"
             << "[---- Map Info ----]" << "\r\n"
             << "AUDIOFILENAME    : " << audioFileName        << "\r\n"
             << "AUDIOLEADIN      : " << audioLeadIn          << "\r\n"
             << "PREVIEWTIME      : " << previewTime          << "\r\n"
             << "COUNTDOWN        : " << countdown            << "\r\n"
             << "SAMPLESET        : " << sampleSet            << "\r\n"
             << "STACKLENIENCY    : " << stackLeniency        << "\r\n"
             << "GAMEMODE         : " << gameMode             << "\r\n"
             << "LETTERBOX        : " << letterbox            << "\r\n"
             << "SPECIALSTYLE     : " << specialStyle         << "\r\n"
             << "WIDESCREEN       : " << widescreen           << "\r\n"
             << "DISTANCESPACING  : " << distanceSpacing      << "\r\n"
             << "BEATDIVISOR      : " << beatDivisor          << "\r\n"
             << "GRIDSIZE         : " << gridSize             << "\r\n"
             << "TIMELINEZOOM     : " << timelineZoom         << "\r\n"
             << "TITLE            : " << title                << "\r\n"
             << "UNICODETITLE     : " << unicodeTitle         << "\r\n"
             << "ARTIST           : " << artist               << "\r\n"
             << "UNICODEARTIST    : " << unicodeArtist        << "\r\n"
             << "CREATOR          : " << creator              << "\r\n"
             << "DIFFICULTYNAME   : " << difficultyName       << "\r\n"
             << "SOURCE           : " << source               << "\r\n"
             << "TAGS             : " << tags                 << "\r\n"
             << "BEATMAPID        : " << beatmapID            << "\r\n"
             << "BEATMAPSETID     : " << beatmapSetID         << "\r\n"
             << "HP               : " << HP                   << "\r\n"
             << "CS               : " << CS                   << "\r\n"
             << "OD               : " << OD                   << "\r\n"
             << "AR               : " << AR                   << "\r\n"
             << "SLIDERMULT       : " << sliderMult           << "\r\n"
             << "SLIDERTICK       : " << sliderTick           << "\r\n"
             << "BGFILENAME       : " << bgFileName           << "\r\n"
             << "VIDEOFILENAME    : " << videoFileName        << "\r\n"
             << "BreakPList <SIZE>: " << breakPList.getSize() << "\r\n"
             << "OM_TPLIST  <SIZE>: " << OM_TPList.getSize()  << "\r\n"
             << "OM_HOLIST  <SIZE>: " << OM_HOList.getSize()  << "\r\n";
}

QList<int> cOM_Map::findMapSettings(QStringList &mapSList)
{
    QList<QRegExp> settingsRegList;
    QList<bool> flagRegList;
    QList<int> output({-1, -1, -1, -1, -1, // 35 elements
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1,
                       -1, -1, -1, -1, -1});
    QRegExp REG_audioFileName  ("AudioFilename:.*"),
            REG_audioLeadIn    ("AudioLeadIn:.*"),
            REG_previewTime    ("PreviewTime:.*"),
            REG_countdown      ("Countdown:.*"),
            REG_sampleSet      ("SampleSet:.*"),
            REG_stackLeniency  ("StackLeniency:.*"),
            REG_gameMode       ("Mode:.*"),
            REG_letterbox      ("LetterboxInBreaks:.*"),
            REG_specialStyle   ("SpecialStyle:.*"),
            REG_widescreen     ("WidescreenStoryboard:.*"),
            REG_distanceSpacing("DistanceSpacing:.*"),
            REG_beatDivisor    ("BeatDivisor:.*"),
            REG_gridSize       ("GridSize:.*"),
            REG_timelineZoom   ("TimelineZoom:.*"),
            REG_title          ("Title:.*"),
            REG_unicodeTitle   ("TitleUnicode:.*"),
            REG_artist         ("Artist:.*"),
            REG_unicodeArtist  ("ArtistUnicode:.*"),
            REG_creator        ("Creator:.*"),
            REG_difficultyName ("Version:.*"),
            REG_source         ("Source:.*"),
            REG_tags           ("Tags:.*"),
            REG_beatmapID      ("BeatmapID:.*"),
            REG_beatmapSetID   ("BeatmapSetID:.*"),
            REG_HP             ("HPDrainRate:.*"),
            REG_CS             ("CircleSize:.*"),
            REG_OD             ("OverallDifficulty:.*"),
            REG_AR             ("ApproachRate:.*"),
            REG_sliderMult     ("SliderMultiplier:.*"),
            REG_sliderTick     ("SliderTickRate:.*"),
            REG_bgFileName     ("//Background and Video events"),
            REG_videoFileName  ("Video,.*"),
            REG_breakPList     ("//Break Periods"),
            REG_OM_TPList      (".*TimingPoints.*"),
            REG_OM_HOList      (".*HitObjects.*");

    settingsRegList = {REG_audioFileName  ,
                       REG_audioLeadIn    ,
                       REG_previewTime    ,
                       REG_countdown      ,
                       REG_sampleSet      ,
                       REG_stackLeniency  ,
                       REG_gameMode       ,
                       REG_letterbox      ,
                       REG_specialStyle   ,
                       REG_widescreen     ,
                       REG_distanceSpacing,
                       REG_beatDivisor    ,
                       REG_gridSize       ,
                       REG_timelineZoom   ,
                       REG_title          ,
                       REG_unicodeTitle   ,
                       REG_artist         ,
                       REG_unicodeArtist  ,
                       REG_creator        ,
                       REG_difficultyName ,
                       REG_source         ,
                       REG_tags           ,
                       REG_beatmapID      ,
                       REG_beatmapSetID   ,
                       REG_HP             ,
                       REG_CS             ,
                       REG_OD             ,
                       REG_AR             ,
                       REG_sliderMult     ,
                       REG_sliderTick     ,
                       REG_bgFileName     ,
                       REG_videoFileName  ,
                       REG_breakPList     ,
                       REG_OM_TPList      ,
                       REG_OM_HOList      };

    bool    FLAG_audioFileName   = false,
            FLAG_audioLeadIn     = false,
            FLAG_previewTime     = false,
            FLAG_countdown       = false,
            FLAG_sampleSet       = false,
            FLAG_stackLeniency   = false,
            FLAG_gameMode        = false,
            FLAG_letterbox       = false,
            FLAG_specialStyle    = false,
            FLAG_widescreen      = false,
            FLAG_distanceSpacing = false,
            FLAG_beatDivisor     = false,
            FLAG_gridSize        = false,
            FLAG_timelineZoom    = false,
            FLAG_title           = false,
            FLAG_unicodeTitle    = false,
            FLAG_artist          = false,
            FLAG_unicodeArtist   = false,
            FLAG_creator         = false,
            FLAG_difficultyName  = false,
            FLAG_source          = false,
            FLAG_tags            = false,
            FLAG_beatmapID       = false,
            FLAG_beatmapSetID    = false,
            FLAG_HP              = false,
            FLAG_CS              = false,
            FLAG_OD              = false,
            FLAG_AR              = false,
            FLAG_sliderMult      = false,
            FLAG_sliderTick      = false,
            FLAG_bgFileName      = false,
            FLAG_videoFileName   = false,
            FLAG_breakPList      = false,
            FLAG_OM_TPList       = false,
            FLAG_OM_HOList       = false;

    flagRegList = {FLAG_audioFileName  ,
                   FLAG_audioLeadIn    ,
                   FLAG_previewTime    ,
                   FLAG_countdown      ,
                   FLAG_sampleSet      ,
                   FLAG_stackLeniency  ,
                   FLAG_gameMode       ,
                   FLAG_letterbox      ,
                   FLAG_specialStyle   ,
                   FLAG_widescreen     ,
                   FLAG_distanceSpacing,
                   FLAG_beatDivisor    ,
                   FLAG_gridSize       ,
                   FLAG_timelineZoom   ,
                   FLAG_title          ,
                   FLAG_unicodeTitle   ,
                   FLAG_artist         ,
                   FLAG_unicodeArtist  ,
                   FLAG_creator        ,
                   FLAG_difficultyName ,
                   FLAG_source         ,
                   FLAG_tags           ,
                   FLAG_beatmapID      ,
                   FLAG_beatmapSetID   ,
                   FLAG_HP             ,
                   FLAG_CS             ,
                   FLAG_OD             ,
                   FLAG_AR             ,
                   FLAG_sliderMult     ,
                   FLAG_sliderTick     ,
                   FLAG_bgFileName     ,
                   FLAG_videoFileName  ,
                   FLAG_breakPList     ,
                   FLAG_OM_TPList      ,
                   FLAG_OM_HOList      };

    for (int tempS = 0; tempS < mapSList.length(); tempS ++)
    {
        for (int tempR = 0; tempR < settingsRegList.length(); tempR ++) {

            if (flagRegList[tempR] == true) // When it is true, that means the parameter has been found
            {
                continue;
            }

            else if (settingsRegList[tempR].exactMatch(mapSList[tempS])) {
                settingsRegList[tempR].setPatternSyntax(QRegExp::Wildcard);

                qDebug() << "Pattern : " << settingsRegList[tempR].pattern();
                qDebug() << "Match to: " << mapSList[tempS];
                qDebug() << "Found at: " << tempS << "\n";

                flagRegList[tempR] = true; // Set to true to skip this parameter next loop
                output[tempR] = tempS; // We return the line number where the parameter is located
            }
        }
    }

    return output;
}
