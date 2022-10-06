//Name: Semih
//Surname: Bağ
//Purpose: Write a program that suggests a route to get from a starting point to a target point on a metro line.

// Main function is at bottom

#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <math.h>
#define SIZE 10



// Structs
typedef struct MetroStation {
	char name[20];
	double x;
	double y;
} MetroStation;

typedef struct MetroLine {
	char color[20];
	MetroStation MetroStations[SIZE];
} MetroLine;

typedef struct MetroSystem {
	char name[20];
	MetroLine MetroLines[SIZE];
} MetroSystem;

// Functions
int equals(MetroStation s1, MetroStation s2);
void addStation(MetroLine *mL, MetroStation mS);
void addStationToArray(MetroStation mL[], MetroStation mS);
void removeStationFromArray(MetroStation mS, int index);
int hasStation(MetroLine mL, MetroStation mS);
MetroStation getFirstStop(MetroLine mL);
MetroStation getPreviousStop(MetroLine mL, MetroStation mS);
MetroStation getNextStop(MetroLine mL, MetroStation mS);
void addLine(MetroSystem *mSy, MetroLine mL);
void printLine(MetroLine mL);
void printPath(MetroStation mS[]);
double calculateDistance(int x0, int y0, int x1, int y1);
double getDistanceTravelled(MetroStation mS[]);
MetroStation findNearestStation(MetroSystem mSy, double x, double y);
void getNeighboringStations(MetroSystem mSy, MetroStation mS, MetroStation neigboringStations[]);
void createDuplicatePath(MetroStation original[],MetroStation copy[]);
void findPath(MetroStation start, MetroStation finish, MetroStation path[]);
void recursiveFindPath(MetroStation start, MetroStation finish, MetroStation partialPath[], MetroStation bestPath[]);
int isContain(MetroStation mS[], MetroStation station);
void resetPath(MetroStation mS[]);


//Declare a MetroSystem with the name of istanbul and an empty content.
MetroSystem istanbul = {"istanbul", '\0'};


// Check if they are same
int equals(MetroStation s1, MetroStation s2){
	int x = strcmp(s1.name,s2.name);
	
	// return 1 if equal, 0 otherwise
	if (x == 0){
		return 1;
	}
	else {
		return 0;
	}

}


// Add station to the metro line
void addStation(MetroLine *mL, MetroStation mS){
	int i;
	
	for(i = 0 ; i < SIZE ; i++){
		if (mL->MetroStations[i].name[0] == '\0'){
			break;
		}
	}
	
	mL->MetroStations[i] = mS;
}


// Add station to any array
void addStationToArray(MetroStation mL[], MetroStation mS){
	int i;
	
	for(i = 0 ; i < SIZE ; i++){
		if (mL[i].name[0] == '\0'){
			break;
		}
	}
	
	mL[i] = mS;
}


// Remove station from any array
void removeStationFromArray(MetroStation mS[], int index){
	int i;
	for (i = 0; i < SIZE ; i++){
		if(i > index){
			mS[i-1] = mS[i];
		}
	}
	mS[SIZE -1] = {'\0'};
}


// Check if array contain the station
int isContain(MetroStation mS[], MetroStation station){
	int i = 0;
	int c = 0;
	
	while (mS[i].name[0] != '\0'){
		if(equals(mS[i], station) == 1){
			c =1;
		}
		i++;
	}
	
	return c;
}


// Check if metro line has any metro station
int hasStation(MetroLine mL, MetroStation mS){
	int i;
	
	for(i = 0 ; i < SIZE ; i++){
		if(strcmp(mL.MetroStations[i].name,mS.name) == 0){
			return 1;
		}
	}
	return 0 ;
}


// Get first station of the metro line
// If line does not have any station than return empty station
MetroStation getFirstStop(MetroLine mL){
	if(mL.MetroStations[0].name[0] == '\0'){
		MetroStation empyt;
		return empyt;
	}
	else {
		return mL.MetroStations[0];
	}
}


// Get previous station of the metro line
// If current station is the firs stop than return empty station
MetroStation getPreviousStop(MetroLine mL, MetroStation mS){
	int i;
	MetroStation empty;
	for (i = 0 ; i < SIZE ; i++){
		if (strcmp(mL.MetroStations[i].name, mS.name) == 0){
			if(i == 0){
				return empty;
			}
			else{
				return mL.MetroStations[i-1];
			}
		}
	}
}


// Get next station of the metro line
// If current station is the last stop than return empty station
MetroStation getNextStop(MetroLine mL, MetroStation mS){
	int i;
	MetroStation empty;
	for (i = 0 ; i < SIZE ; i++){
		if (strcmp(mL.MetroStations[i].name, mS.name) == 0){
			if(i == SIZE-1){
				return empty;
			}
			else{
				return mL.MetroStations[i+1];
			}
		}
	}
}


// Add metro line to the metro system
void addLine(MetroSystem *mSy, MetroLine mL){
	int i;
	
	for(i = 0 ; i < SIZE ; i++){
		if(mSy->MetroLines[i].color[0] == '\0'){
			break;
		}
	}
	
	mSy->MetroLines[i] = mL;
}


// Print color of metro line and elemnths of metro line's metro station array
void printLine(MetroLine mL){
	int i;
	printf("Metroline %s:\t",mL.color);
	
	for(i = 0 ; i < SIZE ; i++){
		if(mL.MetroStations[i].name[0] != '\0'){
			if (i == 0){
				printf("%s",mL.MetroStations[i].name);
			}
			else{
				printf(", %s",mL.MetroStations[i].name);
			}
		}
	}
	
	printf(".\n");
}


// Print elemnths of path which a metro station array line by line
void printPath(MetroStation mS[]){
	int i = 0;
	
	while((mS[i].name[0] != '\0') && (i < 10)){
		printf("\t\t%d. %s\n", i+1, mS[i].name);
		i++;
	}
}


// Calculate the distance between two point in coordinate system
double calculateDistance(int x0, int y0, int x1, int y1){
	return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
}


// Calculate the distance between metro stations step by step and than sum the distances
// Return total distanec
double getDistanceTravelled(MetroStation mS[]){
	double sum = 0;
	int i = 0;
	while (mS[i].name[0] != '\0'){
		if(i > 0){
			sum += calculateDistance(mS[i].x, mS[i].y,  mS[i-1].x, mS[i-1].y);
		}
	i++;
	}
	
	if(i > 0){
		return sum;
	}
	else{
		return 0.0;
	}
}


// Find the nearest station to given pionts in coordinate system
// Finally return the nearest metro station
MetroStation findNearestStation(MetroSystem mSy, double x, double y){
	MetroStation station;
	int tempi = 0;
	int tempj = 0;
	int i = 0;
	int j = 0;	
	double distance = calculateDistance(x, y, mSy.MetroLines[0].MetroStations[0].x, mSy.MetroLines[0].MetroStations[0].y);
	
	while (mSy.MetroLines[tempi].color[0] != '\0'){
		while (mSy.MetroLines[tempi].MetroStations[tempj].name[0] != '\0'){
			if (calculateDistance(x, y, mSy.MetroLines[tempi].MetroStations[tempj].x, mSy.MetroLines[tempi].MetroStations[tempj].y) < distance){
				i = tempi;
				j = tempj;
				distance = calculateDistance(x, y, mSy.MetroLines[tempi].MetroStations[tempj].x, mSy.MetroLines[tempi].MetroStations[tempj].y);
			}
			tempj++;
		}
		tempi++;
		tempj=0;
	}
	
	return mSy.MetroLines[i].MetroStations[j];
}


// Find the neighboring metro station of current metro station and add them to neighboringStations array
void getNeighboringStations(MetroSystem mSy, MetroStation mS, MetroStation neigboringStations[]){
	int i = 0;
	int j = 0;
	int index = 0;
	
	while (mSy.MetroLines[i].color[0] != '\0'){
		while (mSy.MetroLines[i].MetroStations[j].name[0] != '\0'){
			if (equals(mSy.MetroLines[i].MetroStations[j], mS) == 1){
				if (j == 0){
					neigboringStations[index] = mSy.MetroLines[i].MetroStations[j+1];
					index++;
				}
				else if (j == (SIZE-1)){
					neigboringStations[index] = mSy.MetroLines[i].MetroStations[j-1];
					index++;
				}
				else{
					neigboringStations[index] = mSy.MetroLines[i].MetroStations[j-1];
					index++;
					if (mSy.MetroLines[i].MetroStations[j+1].name[0] != '\0'){
						neigboringStations[index] = mSy.MetroLines[i].MetroStations[j+1];
						index++;
					}
				}
			}
			j++;
		}
		i++;
		j=0;
	}
}


// Reset all elemenths of the given metro station array
void resetPath(MetroStation mS[]){
	int i;
	MetroStation empty[SIZE] = {'\0'};
	
	for(i = 0 ; i < SIZE ; i++){
		strcpy(mS[i].name, empty[i].name);
		mS[i].x = empty[i].x;
		mS[i].y = empty[i].y;
	}
}


// Copy the contents of original array to copy array (array type is MetroStation)
void createDuplicatePath(MetroStation original[],MetroStation copy[]){
	int i;
	resetPath(copy);
	for(i = 0 ; i < SIZE ; i++){
		if(original[i].name[0]=='\0'){
			break;
		}
		strcpy(copy[i].name,original[i].name);
		copy[i].x = original[i].x;
		copy[i].y = original[i].y;
	}
}
	

// Create partialPath which is metro station array and call the recursiveFindPath function with this
void findPath(MetroStation start, MetroStation finish, MetroStation path[]){
	MetroStation partialPath[SIZE] = {'\0'};
	return recursiveFindPath(start, finish, partialPath, path);
}


// Find the best path which has smallest distance
// To find best path call itself again and again until it stop or find best path
void recursiveFindPath(MetroStation start, MetroStation finish, MetroStation partialPath[], MetroStation bestPath[]){
	int i;
	int j = 0;
	double distance = getDistanceTravelled(partialPath);

	for (i = 0; i < SIZE ; i++){
		if (strcmp(start.name, partialPath[i].name) == 0){
			return ;
		}
	}
	
	if (equals(start,finish) == 1){
		addStationToArray(partialPath, start);
		if (bestPath[0].name[0] == '\0'){
			createDuplicatePath(partialPath, bestPath);			
		}	

		if (getDistanceTravelled(partialPath) < getDistanceTravelled(bestPath)){

			createDuplicatePath(partialPath, bestPath);		
		}
		return;
	}
	else{	
		MetroStation neighbors[SIZE] = {'\0'};
		getNeighboringStations(istanbul,start,neighbors); 
		while (neighbors[j].name[0] != '\0'){
			MetroStation duplicatePath[20] = {'\0'};
			createDuplicatePath(partialPath, duplicatePath);
			if (isContain(duplicatePath, start) == 0){
				addStationToArray(duplicatePath, start);
			}
			recursiveFindPath(neighbors[j], finish, duplicatePath, bestPath);
		j++;
		}
	}
}



// MAIN FUNCTION
int main()
{
	int i;
	double myX=1, myY=2; 
	double goalX=62, goalY=45; 
	
	// define 3 metro lines, 9 metro stations, and an empty myPath
	MetroLine red={'\0'}, blue={'\0'}, green={'\0'};
	MetroStation s1, s2, s3, s4, s5, s6, s7, s8, s9;
	MetroStation myPath[SIZE]={'\0'};
	
	strcpy(red.color, "red"); 
	strcpy(blue.color, "blue");
	strcpy(green.color, "green");

	
	strcpy(s1.name, "Haydarpasa"); 		s1.x=0; 	s1.y=0;
	strcpy(s2.name, "Sogutlucesme"); 	s2.x=10; 	s2.y=5;
	strcpy(s3.name, "Goztepe"); 		s3.x=20; 	s3.y=10;
	strcpy(s4.name, "Kozyatagi"); 		s4.x=30; 	s4.y=35;
	strcpy(s5.name, "Bostanci"); 		s5.x=45; 	s5.y=20;
	strcpy(s6.name, "Kartal"); 			s6.x=55; 	s6.y=20;
	strcpy(s7.name, "Samandira"); 		s7.x=60; 	s7.y=40;
	strcpy(s8.name, "Icmeler"); 		s8.x=70; 	s8.y=15;
	
	//Add several metro stations to the given metro lines.
	addStation(&red, s1); addStation(&red, s2); addStation(&red, s3); addStation(&red, s4); addStation(&red, s5); addStation(&red, s8);
	
	addStation(&blue, s2); addStation(&blue, s3); addStation(&blue, s4); addStation(&blue, s6); addStation(&blue, s7);
	
	addStation(&green, s2); addStation(&green, s3); addStation(&green, s5); addStation(&green, s6); addStation(&green, s8);
	
	// Add red, blue, green metro lines to the Istanbul metro system.
	addLine(&istanbul, red);
	addLine(&istanbul, blue);
	addLine(&istanbul, green);
	
	// print the content of the red, blue, green metro lines
	printLine(red);
	printLine(blue);
	printLine(green);
		
		
	// find the nearest stations to the current and target locations
	MetroStation nearMe = findNearestStation(istanbul, myX, myY);
	MetroStation nearGoal = findNearestStation(istanbul, goalX, goalY);
	
	printf("\n");
	
	printf("The best path from %s to %s is:\n", nearMe.name, nearGoal.name);
	
	// if the nearest current and target stations are the same, then print a message and exit.
	if(equals(nearMe, nearGoal)){
		printf("It is better to walk!\n");
		return 0;
	}
	
	// Calculate and print the myPath with the minimum distance travelled from start to target stations.
	findPath(nearMe, nearGoal, myPath);
	
	if(strlen(myPath[0].name) == 0)
		printf("There is no path on the metro!\n");
	else{
		printPath(myPath);
	}
	
	return 0;

}
