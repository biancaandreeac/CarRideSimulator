// Copyright 2019 SD_Homework_Team

#ifndef SOLVER_H_
#define SOLVER_H_
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <list>
#include <utility>
#include <limits>
#include "./Graph.h"
#include "./driver.h"
#include "./slist.h"

class solver {
	int nr_inters, nr_str;
  std::vector<std::string> inters;
  graph<std::string> gr;
  std::string start, stop;
  std::vector<Driver> dr;

 public:
		void prepare_tasks(std::ifstream& fin);

		void task1_solver(std::ifstream& fin, std::ofstream& fout) {
			prepare_tasks(fin);
			int q1;
    	fin >> q1;
    	for (int i = 0; i < q1; ++i) {
  	    fin >> start;
    		fin >> stop;
        // Verifies if there is a road between 2 vertex
    		if(gr.has_road(start, stop)) {
    	    fout << "y\n";
    		} else {
    		  fout << "n\n";
    		}
    	}
		}

		void task2_solver(std::ifstream& fin, std::ofstream& fout) {
			int q2;
    	fin >> q2;
    	for (int i = 0; i < q2; ++i) {
    		fin >> start;
    		fin >> stop;
        // Computes the distance between 2 nodes
    	  fout << gr.compute_distance(start, stop) << "\n";
    	}
		}

		void task3_solver(std::ifstream& fin, std::ofstream& fout) {
			int q3;
      fin >> q3;
      char c;
      int n;
      for (int i = 0; i < q3; ++i){
        fin >> c;
        switch (c) {
          case 'c':
            fin >> start;
            fin >> stop;
            fin >> n;
            switch (n) {
              case 0:
                // adds a road between start and stop
                gr.add_edge(start, stop);
                break;
              case 1:
                // blocks the road in both ways
                gr.remove_edge(start, stop);
                gr.remove_edge(stop, start);
                break;
              case 2:
                // makes the road bidirectional
                gr.add_edge(start, stop);
                gr.add_edge(stop, start);
                break;
              case 3:
                // switches the way of the road
                bool a1 = gr.has_edge(start, stop);
                bool b1 = gr.has_edge(stop, start);
                if (a1 && !b1) {
                  gr.add_edge(stop, start);
                  gr.remove_edge(start, stop);
                } else if (b1 && !a1) {
                  gr.add_edge(start, stop);
                  gr.remove_edge(stop, start);
                }
                break;
            }
            break;
          case 'q':
            fin >> start;
            fin >> stop;
            fin >> n;
            switch (n) {
              case 0:
                // verifies if it has a road
                if (gr.has_road(start, stop))
                  fout << 'y' << "\n";
                else
                  fout << 'n' << "\n";
                break;
              case 1:
                // computes the distance
                fout << gr.compute_distance(start, stop) << "\n";
                break;
              case 2:
                // computes the distance between 2 nodes but with a stop at
                // another node
                std::string inters3;
                fin >> inters3;
                auto a = gr.compute_distance(start, inters3);
                if (a == -1) {
                  fout << -1 << "\n";
                } else {
                  auto b = gr.compute_distance(inters3, stop);
                  if (b == -1)
                    fout << -1 << "\n";
                  else
                    fout << a + b << "\n";
                }
                break;
              }
            break;
        }
      }

      // precalculate the distance between every node
      for (int i = 0; i < nr_inters; ++i) {
        gr.BFS(inters[i]);
      }
		}

		void task4_solver(std::ifstream& fin, std::ofstream& fout) {
      int q4;
      std::string instruction;
      std::string name;
      std::string loc;

      // keep the tops in a sorted list
      Slist<int> top_dis;
      Slist<float> top_rat;
      Slist<int> top_rid;

      fin >> q4;
      for (int i1 = 0; i1 < q4; ++i1) {
        fin >> instruction;

        if (instruction == "d") {
          // insert a new driver or make an existing driver online
          fin >> name;
          fin >> loc;
          bool ok = false;
          for (auto i = dr.begin(); i != dr.end(); ++i) {
            if (i->name == name) {
              i->location = loc;
              i->online();
              ok = true;
              break;
            }
          }

          if (!ok) {
            Driver d(name, loc);
            dr.push_back(d);
            top_rid.add_elem(make_pair(0, name));
            top_rat.add_elem(make_pair(0, name));
            top_dis.add_elem(make_pair(0, name));
          }
        } else if (instruction == "b") {
          // make a driver go offline
          fin >> name;
          for (auto i = dr.begin(); i != dr.end(); ++i) {
            if (i->name == name) {
              i->offline();
              break;
            }
          }
        } else if (instruction == "r") {
          // make a ride
          fin >> start;
          fin >> stop;
          int rating;
          fin >> rating;

          int dist;
          int min_dist = std::numeric_limits<int>::max();
          int i, min_indx;

          // get the closest driver
          for (i = 0; i != (int)dr.size(); ++i) {
            if (!dr[i].on) {
              continue;
            }

            dist = gr.get_distance(dr[i].location, start);

            if (dist == -1) {
              continue;
            }
            if (dist < min_dist) {
              min_dist = dist;
              min_indx = i;
            } else if (dist == min_dist) {
              if (dr[i].rating > dr[min_indx].rating) {
                min_indx = i;
              } else if (dr[i].rating == dr[min_indx].rating) {
                if (dr[i].name < dr[min_indx].name) {
                  min_indx = i;
                }
              }
            }
          }

          // if min_dist wasn't modified => drivers unavaliable
          if (min_dist == std::numeric_limits<int>::max()) {
            fout << "Soferi indisponibili\n";
            continue;
          }

          int dist_min = std::numeric_limits<int>::max();
          std::string locc;
          name = dr[min_indx].name;

          dist_min = gr.get_distance(start, stop);
          if (dist_min != -1) {
            // update driver information
            dr[min_indx].location = stop;
            dr[min_indx].add_rating(rating);
            dr[min_indx].dist += min_dist;
            dr[min_indx].dist += dist_min;

            top_rat.update_elem(make_pair(dr[min_indx].rating, name));
            top_dis.update_elem(make_pair(dr[min_indx].dist, name));
            top_rid.update_elem(make_pair(dr[min_indx].rides, name));
            continue;
          } else {
            dist_min = std::numeric_limits<int>::max();
            std::vector<std::string> roads;
            roads = gr.get_neighbors(stop);
            if (roads.empty()) {
              fout << "Destinatie inaccesibila\n";
              continue;
            } else {
              int j;
              for (j = 0; j < (int)roads.size(); ++j) {
                dist = gr.get_distance(start, roads[j]);
                if (dist != -1) {
                  dist_min = dist;
                  stop = roads[j];
                  break;
                }
              }

              if (dist_min == std::numeric_limits<int>::max()) {
                fout << "Destinatie inaccesibila\n";
                continue;
              }

              // update driver information
              dr[min_indx].location = stop;
              dr[min_indx].add_rating(rating);
              dr[min_indx].dist += min_dist;
              dr[min_indx].dist += dist_min;

              top_rat.update_elem(make_pair(dr[min_indx].rating, name));
              top_dis.update_elem(make_pair(dr[min_indx].dist, name));
              top_rid.update_elem(make_pair(dr[min_indx].rides, name));

              continue;
            }
          }
        } else if (instruction == "top_rating") {
          // print top n drivers after rating
          int n;
          fin >> n;
          top_rat.print_precision(n, fout);
        } else if (instruction == "top_dist") {
          // print top n drivers after dist
          int n;
          fin >> n;
          top_dis.print_n(n, fout);
        } else if (instruction == "top_rides") {
          // print top n drivers after rides
          int n;
          fin >> n;
          top_rid.print_n(n, fout);
        } else if (instruction == "info") {
          // print driver's info
          fin >> name;
          for (auto i = dr.begin(); i != dr.end() ; ++i) {
            if (i->name == name) {
              fout << i->name<< ": " << i->location << " " << i->rating << " ";
              fout << i->rides << " " << i->dist << " ";
              i->on ? fout << "online\n" : fout << "offline\n";
              break;
            }
          }
        }
      }
		}

		void task5_solver(std::ifstream& fin, std::ofstream& fout) {
      int gas;
      std::string name;
      fin >> gas;
      fin >> name;
      int n;
      Driver d;
      std::vector<std::string> inters;
      std::vector<int> dist;
      fin >> n;

      // find the driver
      for (int i = 0; i < (int)dr.size(); ++i) {
        if (dr[i].name == name) {
          d = dr[i];
          break;
        }
      }

      bool ok;
      // find where can the driver go and add the names into a vector
      for (int i = 0; i < n; ++i) {
        fin >> name;
        ok = false;
        for (auto j = inters.begin(); j != inters.end(); ++j) {
          if (*j == name) {
            ok = true;
          }
        }
        if (ok) {
          continue;
        }
        int dis = gr.get_distance(d.location, name);
        if (dis <= gas && dis != -1) {
          inters.push_back(name);
          dist.push_back(dis);
        }
      }

      // sort the vector
      for (int i = 0; i < (int)dist.size() - 1; ++i) {
        for (int j = i; j < (int)dist.size(); ++j) {
          if (dist[i] > dist[j]) {
            auto aux = dist[i];
            dist[i] = dist[j];
            dist[j] = aux;
            auto aux1 = inters[i];
            inters[i] = inters[j];
            inters[j] = aux1;
          } else if (dist[i] == dist[j]) {
            if (inters[i] > inters[j]) {
              auto aux = dist[i];
              dist[i] = dist[j];
              dist[j] = aux;
              auto aux1 = inters[i];
              inters[i] = inters[j];
              inters[j] = aux1;
            }
          }
        }
      }

      // print the result
      for (int i = 0; i < (int)dist.size(); ++i) {
        fout << inters[i] << " ";
      }
      fout << "\n";
		}
};

void solver::prepare_tasks(std::ifstream& fin) {
    fin >> nr_inters;
    fin >> nr_str;
    inters.resize(nr_inters);

    // get intersection names
    for (int i = 0; i < nr_inters; ++i) {
        fin >> inters[i];
        gr.add_vertex(inters[i]);
    }

    // get direct roads
    for (int i = 0; i < nr_str; ++i) {
        fin >> start;
        fin >> stop;
        gr.add_edge(start, stop);
    }
}

#endif  // SOLVER_H_
