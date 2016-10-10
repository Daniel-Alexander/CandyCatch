///////// Multimodal Interaction ////////


// class: cDetectFace
//
// brief: Klasse um initial das Gesicht aus dem Bild zu extrahieren
//
class cDetectFace
{
	// method: getFacePosition
	//
	// brief: Methode zum extrahieren der Gesichter
	//
	// param: oImg:            Pointer auf das aktuelle Bild 
	//        rctFacePosition: Referenz auf die die aktuelle Position geschrieben wird
	//
	// return: negative int if failed
	virtual int getFacePosition(cv::Mat * oImg, cv::Rect &rctFacePosition)
};


// class: cTrackFace
//
// brief: Klasse um Gesichtern im Video zu folgen
//
class cTrackFace
{
	// method: getNextPosition
	//
	// brief: Methode berechnet aus der aktuell extrahierten Position (kann NULL sein wenn kein Gesicht detektiert wird) 
	//        die Position des Gesichts
	//
	// param: rctPosition: berechnete Position des Gesichts
	//        rctCurrentDetection: detektierte Position kann NULL sein wenn nichts detektiert, dann soll Klass die Position berechnen
    //
	// return: negative int if failed
	virtual int getNextPosition(cv::Rect &rctPosition, cv::Rect rctCurrentDetection);
	
	// method: plotFace
	//
	// brief: Plottet die aktuell berechnete Position ins Bild
	//
	// param: oImg: Pointer auf das Bild in welches geplottet wird
	//
	// return: negative int if failed
	virtual int plotFace(cv::Mat * oImg);
};


// class: cStoreCandy
//
// brief: Klasse zum verwalten der Candys
//
class cStoreCandy
{
	// method: setUpCandy
	//
	// brief: Erstellt ein neues Candy
	//
	// param: pntPosition: Position an der das candy erscheinen soll
	//
	// return: negative int if failed
	virtual int setUpCandy(cv::Point pntPosition);
	
	// method: checkCollition
	//
	// brief: Prüft ob Gesicht das Candy berührt und löscht die Position dann
	//
	// param: rctFacePosition: Position an der das Gesicht ist
	//
	// return: negative int if failed
	virtual int checkCollition(cv::Rect rctFacePosition);
	
	// method: plotCandy
	//
	// brief: Plottet die Candys ins Bild
	//
	// param: oImg: Pointer auf das Bild in welches geplottet werden soll
	//
	// return: negative int if failed
	virtual int plotCandy(cv::Mat * oImg);
};