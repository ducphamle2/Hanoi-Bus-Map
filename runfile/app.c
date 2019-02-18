#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX 100

#include "jrb.h"
#include "jval.h"
#include "dllist.h"
#include "busmap.h"

char *buses[MAX]; //keep buses' names
char *readHolder1[MAX]; //keep chieu di
char *readHolder2[MAX]; //keep chieu ve
int busLine[MAX][MAX]; //keep the number of lines that a bus takes
int sizeHolder = 0; //size of above vars
int sizeBuses = 0; //number of buses
int row = 0, col = 0;

void copyData(char *bus[], char* file1[], char* file2[]) {
	for (int i = 0; i < sizeHolder; i++) {
		file1[i] = strdup(readHolder1[i]);
		file2[i] = strdup(readHolder2[i]);
	}
	for (int i = 0; i < sizeBuses; i++) {
		bus[i] = strdup(buses[i]);
	}
}

void trim(char *line) {
	int i = strlen(line);
	while (i > 0 && isspace(line[i - 1])) {
		--i;
	}
	line[i] = '\0';
	i = strspn ( line, " \t\n\v" );
	memmove ( line, line + i, strlen ( line + i ) + 1 );
}

void readFile(FILE *f1, FILE *f2) {
	int i = 0, k = 0, flag = 0;
	char *reader = (char*)malloc(sizeof(char));
	int fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
	buses[k] = strdup(reader);
	k++;
	busLine[row][col] = k;
	while (fscanfChecker == 1) {
		flag = 0;
		fscanfChecker = fscanf(f1, "%[^\n]%*c", reader);
		if (fscanfChecker != 1) break;
		if (strchr(strdup(reader), '-') != NULL) {
			readHolder1[i] = strdup(reader);
			flag = 1;
			i++;
			busLine[row][col + 1] = busLine[row][col + 1] + 1;
		}
		if (flag == 0) {
			buses[k] = strdup(reader);
			row++;
			k++;
			busLine[row][col] = k;
		}
	}
	sizeHolder = i; sizeBuses = k;
	i = 0;
	fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
	while (fscanfChecker == 1) {
		fscanfChecker = fscanf(f2, "%[^\n]%*c", reader);
		if (fscanfChecker != 1) break;
		if (strchr(strdup(reader), '-') != NULL) {
			readHolder2[i] = strdup(reader);
			i++;
		}
	}
}

void dataProcess(Graph g, JRB bus) {
	int i = 0, count = 0;
	int vertexID = 0;
	char *linesplitter[MAX];
	char *temp = "-";
	for (i = 0; i <= row; i++) {
		for (int m = 0; m < busLine[i][col + 1]; m++) {
			int j = 0, k = 0;
			linesplitter[j] = strtok(readHolder1[count], temp);
			while (linesplitter[j] != NULL) {
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			linesplitter[j] = strtok(readHolder2[count], temp);
			while (linesplitter[j] != NULL) {
				j++;
				linesplitter[j] = strtok(NULL, temp);
			}
			
			char weight[10];
			weight[0] = (i + 1) + '0';
			weight[1] = '\0';
			for (int k = 0; k < j; k++) {
				trim(linesplitter[k]);
				addStations(g, linesplitter[k], &vertexID);
			}
			
			for (int k = 0; k < j - 1; k++) {
				addEdges(g, bus, linesplitter[k], linesplitter[k + 1], i);
			}
			count++;
		}
		
	}
}

void printFileData(char *bus[], char* file1[], char* file2[]) {
	int count = 0;
	for (int i = 0; i < sizeBuses; i++) {
		printf("BUS: %s\n", bus[i]);
		for (int j = 0; j < busLine[i][col + 1]; j++) {
			printf("CHIEU DI: %s\n\n", file1[count]);
			printf("CHIEU VE: %s\n\n", file2[count]);
			count++;
		}
	}
	
	/*for (int i = 0; i <= row; i++) {
		printf("%d - %d\n", busLine[i][col], busLine[i][col + 1]);
	}
	*/
}

void printBuses(JRB bus) {
	JRB node;
	jrb_traverse(node, bus) {
		printf("%s - %s\n", jval_s(node->key), jval_s(node->val));
	}
}

void printPath(Graph g, JRB busTable, int *sizeOutput, int *output) {
	if (*sizeOutput <= 0)
		return;
	int i = *sizeOutput - 1;;
	for (i = *sizeOutput - 1; i > 0; i--) {
		char*station1 = getVertex(g, output[i]);
		if (station1 == NULL) {
			printf("station1 at printPath is null\n");
			exit(0);
		}
		char*edgeValue = getEdgeValue(g, output[i], output[i - 1]);
		if (edgeValue == NULL) {
			printf("Edge value is null\n");
			exit(0);
		}
		printf("%s (%s) --> ", station1, jval_s(jrb_find_str(busTable, strdup(edgeValue))->val));
	}
	printf("%s\n", getVertex(g, output[i]));
}

void printVertices(Graph g) {
	JRB node;
	jrb_traverse(node, g.vertices) {
		printf("%d - %s\n", jval_i(node->key), jval_s(node->val));
	}
}

void printEdges(Graph g) {
	JRB node;
	JRB node2;
	jrb_traverse(node, g.vertices) {
		jrb_traverse(node2, g.vertices) {
			char *temp = getEdgeValue(g, jval_i(node->key), jval_i(node2->key));
			if (temp != NULL)
				printf("%s\n", temp);
		}
	}
}

void printShortestPath(Graph g, int start, int stop, int *size, int *path) {
	JRB sp_distance = make_jrb();
	JRB sp_parent = make_jrb();
	int shortest_path = shortestPath(g, start, stop, sp_distance, sp_parent);
	if (shortest_path == 0) 
		printf("\nGiven source and destination are not connected\n");
	else {
		int current = 0, parent = 0;
		current = stop;
		parent = jval_i(jrb_find_int(sp_parent, current)->val);
		path[*size] = current;
		while (current != parent) {
			path[*size] = current;
			current = parent;
			(*size)++;
			parent = jval_i(jrb_find_int(sp_parent, current)->val);
			path[*size] = current;
		}
		(*size)++;
		path[*size] = start;
		for (int j = (*size) - 1; j >= 0; j--) {
			printf("%d ", path[j]);
		}
		printf("\n");
	}
	jrb_free_tree(sp_distance);
	jrb_free_tree(sp_parent);
}

int main(int argc, char* argv[]) {
	Graph g = createGraph();
	JRB busTable = createBusTable();
	JRB lineTable = make_jrb();
	int choice = 0, id1 = 0, id2 = 0, flag = 0;
	char *name1 = (char*)malloc(sizeof(char));
	char *name2 = (char*)malloc(sizeof(char));
	char *bus[MAX];
	char *file1[MAX];
	char *file2[MAX];
	FILE *f1, *f2;
	f1 = fopen(argv[1], "r"); f2 = fopen(argv[2], "r");
	if (f1 == NULL || f2 == NULL) {
		printf("Cant read the file\n");
		exit(0);
	}
	
	readFile(f1, f2);
				
	copyData(bus, file1, file2);
	
	makeBusTable(busTable, sizeBuses, buses);
				
	dataProcess(g, busTable);
	
	adjustBusVal(busTable);
	do {
		printf("\n----HANOI BUS LINE----\n\n");
		printf("1) Print all the buses and lines containing different stations.\n");
		printf("2) Type your current location, and your destination to find the shortest path.\n");
		printf("3) Exit.\n");
		printf("Choose one: ");
		scanf("%d", &choice);
		switch(choice) {
		case 1:
			printFileData(bus, file1, file2);
			printBuses(busTable);
			break;
		case 2:
			printVertices(g);

			printf("Type in your current location(type id): ");
			scanf("%d", &id1); getchar();
			if (!isdigit(id1)) {
				printf("Input must be a number, not letter.\n");
				break;
			}
			if (getVertex(g, id1) == NULL) 
				printf("Your location is not found!\n");
			else {
				printf("Type in your destination(type id): ");
				scanf("%d", &id2); getchar();
				if (!isdigit(id2)) {
					printf("Input must be a number, not letter.\n");
					break;
				}
				if (getVertex(g, id2) == NULL) 
					printf("Your destination is not found\n");
				else if (id1 == id2) 
					printf("\nYour current location and destination are identical\n");
				else {
					int output[MAX];
					int sizeOutput = 0;
					printShortestPath(g, id1, id2, &sizeOutput, output);
					printPath(g, busTable, &sizeOutput, output);
				}
			}
			break;
		case 3:
			printf("Bye!\n");
			break;
		default:
			printf("Pls choose between 1 & 3 only");
			break;
		}
	}while (choice != 3);
	jrb_free_tree(busTable);
	freeGraph(g);
	return 0;
}
