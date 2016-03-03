// Thomas Rousseau
// TP INF229
// Télécom Paristech 2A

Bar slide;

//initialisation function
void setup() {
  size(800, 800);
  readData();
  PFont myFont = loadFont("Amiri-Regular-48.vlw");
  textFont(myFont,32);
  slide = new Bar();
}

//global variables for the population bounds
int minPopulationToDisplay;
int maxPopulationToDisplay;

//general draw function
void draw() {
    background(255);
    fill(0,39,74);
    textSize(32);
    text("FRANCE",10,40);
    textSize(20);   
    text("Plage de population : " + minPopulationToDisplay + " ~ " + maxPopulationToDisplay,10,725);
    int pop = -1;
    //display of the information of the clicked city
    for (int i = 0; i < numPoints-2; ++i) {
      if(places[i].isSelected){
        fill(0,39,74);
        text(places[i].name,10,625);
        text("Code postal : "+ places[i].postalCode,10,650);
        text("Population : " + (int)places[i].population,10,675);
        text("Densité : " + places[i].density,10,700);}
      places[i].draw();     
      if(!places[i].isHovered)
        places[i].drawCircles();
    }
    for (int i = 0; i < numPoints-2; ++i)
      if(places[i].isHovered){
        pop = (int)(places[i].population);
        places[i].drawHighlighted();}
    slide.draw(pop); 
}

//update the selected flag of the clicked city or the selected slider of the bar
void mousePressed(){
  slide.goodclick(mouseX,mouseY);
  pick(mouseX,mouseY,0);
}

//update the flags of the bar 
void mouseReleased(){
  slide.isLeftDragged = false;
  slide.isRightDragged = false; 
}

//update the hover flags of the cities
void mouseMoved(){
  pick(mouseX,mouseY,1);
}

//update both the selected and hover flags of the cities
void pick(int xx, int yy,int mode){
  for (int i = numPoints-3; i >= 0; i--) {
    if(mode == 0){
      if(places[i].contains(mouseX,mouseY)){
        places[i].isSelected = true;}
      else
        places[i].isSelected = false;}
    else if(mode == 1){
      if(places[i].contains(mouseX,mouseY)){
        places[i].isHovered = true;}
      else
        places[i].isHovered = false;
    }
  }
}

//manage the keyboard inputs -> O,P,L,M
void keyPressed(){
  int step = 1000;
  if(keyCode==76 && minPopulationToDisplay-step>=0){
    minPopulationToDisplay=max(0,minPopulationToDisplay-step);
    slide.currentminpos = slide.fpopinv(int(minPopulationToDisplay));
  }
  else if(keyCode==77 && minPopulationToDisplay+step<=maxPopulationToDisplay){
    minPopulationToDisplay=min(minPopulationToDisplay+step,maxPopulationToDisplay);
    slide.currentminpos = slide.fpopinv(int(minPopulationToDisplay));
  }
  else if(keyCode==79 && maxPopulationToDisplay-step>=minPopulationToDisplay){
    maxPopulationToDisplay=max(minPopulationToDisplay,maxPopulationToDisplay-step);
    slide.currentmaxpos = slide.fpopinv(int(maxPopulationToDisplay));
  }
  else if(keyCode==80 && maxPopulationToDisplay+step<=max_pop){
    maxPopulationToDisplay=min(maxPopulationToDisplay+step,(int)max_pop);
    slide.currentmaxpos = slide.fpopinv(int(maxPopulationToDisplay));
  }
  redraw(); 
}

//update the sliders position of the bar if necessary
void mouseDragged(){
  if(slide.isLeftDragged && mouseX <= slide.currentmaxpos - 12)
    slide.setmin(mouseX);
  else if(slide.isRightDragged && mouseX >= slide.currentminpos + 12)
    slide.setmax(mouseX);
}

Place places[];
float max_pop,min_pop,min_dens,max_dens;

//parsing function -> should not rely on telecom-paristech.fr network
void readData() {
    String[] lines = loadStrings("population.tsv");
    parseInfo(lines[0]);
    places = new Place[numPoints];
    for (int i = 2; i < numPoints; ++i) {
      String pieces[] = split(lines[i], '\t');
      float alphax = 800.0/(maxX - minX);
      float alphay = 800.0/(maxY - minY);
      float betax = -alphax*minX;
      float betay = -alphay*minY;
      places[i-2] = new Place(Integer.parseInt(pieces[0]),(String)pieces[4],((int)(alphax*float(pieces[1])+betax)),800-((int)(alphay*float(pieces[2])+betay)),float(pieces[5]),float(pieces[6]));
    }
    //min,max popualtion detection
    min_pop = 100000000;
    max_pop = 0;
    for (int i = 2; i < numPoints; ++i){
      if(places[i-2].population>max_pop)
        max_pop = places[i-2].population;
      if(places[i-2].population<min_pop)
        min_pop = places[i-2].population;
    }
    minPopulationToDisplay = (int)min_pop;
    maxPopulationToDisplay = (int)max_pop;    
    //min,max density detection
    min_dens = 100000000;
    max_dens = 0;
    for (int i = 2; i < numPoints; ++i){
      places[i-2].set_rayon();
      if(places[i-2].density>max_dens)
        max_dens = places[i-2].density;
      if(places[i-2].density<min_dens)
        min_dens = places[i-2].density;
    }
}

float minX, maxX; // store the bounding box of all points
float minY, maxY;
int numPoints; // total number of places seen

void parseInfo(String line) { // Parse one line
    String infoString = line.substring(2); // remove the #
    String[] pieces = split(infoString, ',');
    numPoints = int(pieces[0]);
    minX = float(pieces[1]);
    maxX = float(pieces[2]);
    minY = float(pieces[3]);
    maxY = float(pieces[4]);
}
