											------BUS MAP REPORT------
NAME: PHAM LE DUC
CLASS: ICT.01 - K61

Description: This report is created to explain briefly about how i made the bus map which finds the shortest path between two bus stations. 
1) DATA STORAGE: Data is taken from this source: "http://timbus.vn/fleets.aspx?fbclid=IwAR05i5hh3dgVqzhBXHZJwXT1dCZ-lM-rnVDeHxhSZsEDl6f0LYaRBVpo7-g"
		 My application contains two .txt files, one is "go.txt" and the other is "back.txt". Both have a similar format in which buses' names and buses' lines are ordered respectively. For example, in the file "go.txt", the first line is "Xi Nghiep Xe Buyt Ha Noi" - bus name, and after that there are n-forwarded station lines that this bus takes, in this case is three (Can add more if you prefer - but it must strictly follow the format). Next, after covering all the forwarded path, a different bus name is written, in this case is "Xi nghiep Xe buyt nhanh BRT Ha Noi". The file goes on and on like that if you wish to add more station lines and buses. "back.txt" functions similarly, however, the stations lines are the backward paths that these buses go. Please note that you must follow strictly to the files, else the application will not work.

2) DATA STRUCTURE & ALGORITHM USED IN THIS APP: I use some basic data structures to handle information, which are jrb tree, directed graph with weight, array of pointers and 2D matrix
				    While reading the data, i create a 2D array which stores the number of station lines each buses go. Buses' names are mapped to different ids, running from 1 to n. Moreover, i also store each lines in both files into three distinct char* array of pointers to print them out.
				    After reading, i handle data by using strtok and other methods to add n distinct stations into a directed graph as distinct vertices, and their ids are from 0 to n. In handling the edges, let's me take an example here: we have station i and i + 1 in which i connects i + 1. In order to go from i to i + 1, we can take among n buses, k buses that can lead us to station i + 1. So the weight of i & i + 1 edge stores the buses' ids that from i we can go to i + 1. And from this, i also create a bus table using JRB tree to map these edges to buses' names for output print.
				    For shortest path algorithm, i use a modified BFS for directed graph.

3) RUNTIME: The main functionality of this app is to find a shortest path (the path that has a minimum number of stations used to reach the final destination). Another option is printing out information about the bus map.

4) HOW TO COMPILE & RUN THIS APP: This program includes jrb.c, jval.c, dllist.c, BusMap.c and app.c.  There is libfdr.a that contains all except BusMap.c & app.c => "gcc -o app app.c BusMap.c libfdr.a" is an option
				  In order to run this app, two arguments must be included: go.txt & back.txt => "./app go.txt back.txt" is an option.


REMARK: Due to limited knowledge and skills, the program may contain bugs that i did not know while testing, so please don't mind about them. 



								THANK YOU FOR READING AND USING THIS SMALL APPLICATION, HAVE A NICE DAY !!
