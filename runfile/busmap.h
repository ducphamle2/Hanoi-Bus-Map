#ifndef	_BUSMAP_H_
#define	_BUSMAP_H_

#include "jrb.h"
#include "dllist.h"
#include "jval.h"

typedef struct {
	JRB edges;
	JRB vertices;
} Graph;

Graph createGraph();

void addVertex(Graph graph, int id, char* name);

char* getVertex(Graph graph, int id);

void addEdge(Graph graph, int v1, int v2, char* weight);

int hasEdge(Graph graph, int v1, int v2);

char* getEdgeValue(Graph g, int v1, int v2);

void setEdgeValue(Graph g, int v1, int v2, char* new_weight);

int shortestPath(Graph graph, int start, int stop, JRB sp_distance, JRB sp_parent);

void printShortestPath(Graph g, int start, int stop, int *size, int *path);

int indegree(Graph graph, int v, int* output);

int outdegree(Graph graph, int v, int* output);

void addStations(Graph g, char *station, int *vertexID);

void getVertexID(Graph g, int *id1, int *id2, char* station1, char* station2);

void setBus(JRB busTable, char *station_id);

void addEdges(Graph g, JRB bus, char* station1, char* station2, int bus_num);

void makeBusTable(JRB bus, int size, char* buses[]);

JRB createBusTable();

void adjustBusVal(JRB bus);

void readFile(FILE *f1, FILE *f2);

void dataProcess(Graph g, JRB bus);

int checkStationName(Graph g, char *station);

void freeGraph(Graph graph);

#endif
